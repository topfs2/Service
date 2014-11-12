/*
 *      Copyright (C) 2013 Tobias Arrskog
 *      https://github.com/topfs2/Service
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/signals2/deconstruct.hpp>
#include <boost/shared_ptr.hpp>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>

#include "PowerService.h"
#include "Variant.h"

#include "ProcessMainloop.h"
#include "ThreadedMainloop.h"

#include "ReactorMailbox.h"
#include "SimpleReactorMailbox.h"
#include "MonitorMailbox.h"

#include "ServiceBase.h"
#include "MessageRouter.h"

#include "MessagesCore.h"
#include "MessagesProperties.h"
#include "MessagesPower.h"

#include "MessagesUtils.h"

using namespace messages;
using namespace log4cxx;

static LoggerPtr logger = Logger::getLogger("Test");

void debug_message(MessagePtr msg) {
  if (msg->IsType(MESSAGE_TYPE_NOTIFICATION)) {
    CNotificationPtr m = boost::dynamic_pointer_cast<CNotification>(msg);
    LOG4CXX_INFO(logger, "notification { " << m->method << " }");
  } else if (msg->IsType(MESSAGE_TYPE_REQUEST)) {
    CRequestPtr m = boost::dynamic_pointer_cast<CRequest>(msg);
    LOG4CXX_INFO(logger, "request { " << m->method << ", " << m->id << " }");
  } else if (msg->IsType(MESSAGE_TYPE_RESPONSE)) {
    CResponsePtr m = boost::dynamic_pointer_cast<CResponse>(msg);
    LOG4CXX_INFO(logger, "response { " << m->id << " }");
  } else if (msg->IsType(MESSAGE_TYPE_ERROR)) {
    CErrorPtr m = boost::dynamic_pointer_cast<CError>(msg);
    LOG4CXX_INFO(logger, "error { " << m->code << ", " << m->message << " }");
  } else {
    LOG4CXX_INFO(logger, "message { " << msg->type << " }");
  }
}

CProcessMainloopPtr mainloop = CProcessMainloopPtr(new CProcessMainloop());

class CPingPongService : public CServiceBase
{
public:
    CPingPongService(MainloopPtr mainloop) : CServiceBase(mainloop) { }

protected:
    virtual void HandleMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg)
    {
        LOG4CXX_INFO(logger, "CPinPongService::HandleMessage sender=" << sender << " destination=" << destination);
        debug_message(msg);

        if (msg->IsType(MESSAGE_TYPE_REQUEST)) {
            CRequestPtr request = boost::dynamic_pointer_cast<CRequest>(msg);

            if (request->method == "Ping" && request->id) {
              MessagePtr reply(new CTemplateResponse<std::string>("Pong", request->id));
              source->PostMessage(shared_from_this(), destination, sender, reply);
            }
        } else {
            CServiceBase::HandleMessage(source, sender, destination, msg);
        }
    }
};

class CTestService : public CReactorMailbox
{
public:
    CTestService(MainloopPtr mainloop) : CReactorMailbox(mainloop) { }

protected:
    virtual void HandleMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg)
    {
        LOG4CXX_INFO(logger, "CTestService::HandleMessage sender=" << sender << " destination=" << destination);
        debug_message(msg);

        CVariant result;
        if (GetResult<CVariant>(msg, result)) {
            LOG4CXX_INFO(logger, "Result" << result.asBoolean());
        }
    }
};

class CTestSimpleService : public CSimpleReactorMailbox
{
public:
    CTestSimpleService(MainloopPtr mainloop) : CSimpleReactorMailbox(mainloop) { }

protected:
    virtual void HandleResponse(MailboxPtr source, std::string sender, std::string destination, CResponsePtr response)
    {
        LOG4CXX_INFO(logger, "CTestSimpleService::HandleResponse sender=" << sender << " destination=" << destination);
        debug_message(response);

        CVariant result;
        if (GetResult<CVariant>(response, result)) {
            LOG4CXX_INFO(logger, "Result" << result.asBoolean());
        }
    }
};

class CSimpleRequester : public CReactorMailbox
{
public:
    CSimpleRequester(CMessageRouterPtr router, MainloopPtr mainloop) : CReactorMailbox(mainloop), m_router(router)
    {
      m_address = router->RegisterMailbox(shared_from_this());

      std::cout << "Registered SimplRequester at " << m_address;
    }

protected:
    CMessageRouterPtr m_router;
    std::string m_address;
};

void quit_defer()
{
    LOG4CXX_INFO(logger, "quit_defer");
    mainloop->quit();
}

const unsigned int LOG4CXX_POLL_INTERVAL = 60;
const char *LOG4CXX_PROPERTY_FILE = "log4cxx.properties";

int main() {
    BasicConfigurator::configure();
    PropertyConfigurator::configureAndWatch(LOG4CXX_PROPERTY_FILE, LOG4CXX_POLL_INTERVAL);

    CThreadedMainloopPtr offthread(new CThreadedMainloop());

    MailboxPtr ping_service(new CPingPongService(offthread));
    MailboxPtr power_service(new CPowerService(offthread));

    MailboxPtr ts(new CTestSimpleService(mainloop));
    //MailboxPtr ts(new CMonitorMailbox());

    LOG4CXX_INFO(logger, "test " << ts);
    LOG4CXX_INFO(logger, "ping " << ping_service);
    LOG4CXX_INFO(logger, "power " << power_service);

    CMessageRouterPtr router(new CMessageRouter());

    router->RegisterMailbox("ping",  ping_service);
    router->RegisterMailbox("power", power_service);
    std::string address = router->RegisterMailbox(ts);
    LOG4CXX_INFO(logger, "Registered test at " << address);

    if (true) {
        MessagePtr msg(new messages::CRequest("Ping", 12));
        router->PostMessage(ts, address, "ping", msg);
#ifdef MONITORMAILBOX
        boost::shared_ptr<CMonitorMailbox> hmm = boost::dynamic_pointer_cast<CMonitorMailbox>(ts);
        std::cout << "Result " << hmm->GetResult<std::string>(12);
#endif
    }

    try {
        MessagePtr msg(new messages::CTemplateRequest<std::string>("GetProperty", "CanShutdown", 12));
        router->PostMessage(ts, address, "power", msg);
#ifdef MONITORMAILBOX
        boost::shared_ptr<CMonitorMailbox> hmm = boost::dynamic_pointer_cast<CMonitorMailbox>(ts);
        std::cout << "Result " << hmm->GetResult<std::string>(12);
#endif

        msg = MessagePtr(new messages::CTemplateRequest<KeyValue>("SetProperty", KeyValue("CanShutdown", false), 12));
        router->PostMessage(ts, address, "power", msg);

        msg = MessagePtr(new messages::CTemplateRequest<std::string>("GetProperty", "CanShutdown", 12));
        router->PostMessage(ts, address, "power", msg);
    } catch (std::string e) {
        std::cout << e << std::endl;
    }

    mainloop->schedule(quit_defer, 100);
    mainloop->run();

    return 0;
}
