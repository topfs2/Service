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

#include "PowerService.h"
#include "Variant.h"
#include "ProcessMainloop.h"

#include "ReactorMailbox.h"
#include "ServiceBase.h"
#include "MessageRouter.h"

#include "MessagesCore.h"
#include "MessagesProperties.h"
#include "MessagesPower.h"

#define MESSAGE_TYPE_PING (0x4)
#define MESSAGE_TYPE_PONG (0x8)

void debug_message(MessagePtr msg) {
    std::cout << "message {" << std::endl
              << "\ttype="                  << msg->Type() << std::endl << std::endl

              << "\tis_notification="       << msg->IsType(MESSAGE_TYPE_NOTIFICATION) << std::endl
              << "\tis_error="              << msg->IsType(MESSAGE_TYPE_ERROR) << std::endl
              << "\tis_request="            << msg->IsType(MESSAGE_TYPE_REQUEST) << std::endl
              << "\tis_response="           << msg->IsType(MESSAGE_TYPE_RESPONSE) << std::endl

              << "\tis_ping="               << msg->IsType(MESSAGE_TYPE_PING) << std::endl
              << "\tis_pong="               << msg->IsType(MESSAGE_TYPE_PONG) << std::endl

              << "\tis_get_property="       << msg->IsType(MESSAGE_TYPE_GET_PROPERTY) << std::endl
              << "\tis_set_property="       << msg->IsType(MESSAGE_TYPE_SET_PROPERTY) << std::endl
              << "\tis_change_property="    << msg->IsType(MESSAGE_TYPE_CHANGE_PROPERTY) << std::endl

              << "\tis_on_shutdown="        << msg->IsType(MESSAGE_TYPE_ON_SHUTDOWN) << std::endl
              << "\tis_on_sleep="           << msg->IsType(MESSAGE_TYPE_ON_SLEEP) << std::endl
              << "\tis_on_wake="            << msg->IsType(MESSAGE_TYPE_ON_WAKE) << std::endl

              << "\tis_shutdown="           << msg->IsType(MESSAGE_TYPE_SHUTDOWN) << std::endl
              << "\tis_sleep="              << msg->IsType(MESSAGE_TYPE_SLEEP) << std::endl
              << "}" << std::endl;
}

CProcessMainloopPtr mainloop = CProcessMainloopPtr(new CProcessMainloop());

class CPingPongService : public CServiceBase
{
public:
    CPingPongService(MainloopPtr mainloop) : CServiceBase(mainloop) { }

protected:
    virtual void HandleMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg)
    {
        std::cout << "CPinPongService::HandleMessage sender=" << sender << " destination=" << destination << std::endl;
        debug_message(msg);

        if (msg->IsType(MESSAGE_TYPE_PING)) {
            messages::CRequestPtr request = boost::dynamic_pointer_cast<messages::CRequest>(msg);

            MessagePtr reply(new messages::CResponse(MESSAGE_TYPE_PONG, request->id));
            source->PostMessage(shared_from_this(), destination, sender, reply);
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
        std::cout << "CTestService::HandleMessage sender=" << sender << " destination=" << destination << std::endl;
        debug_message(msg);
    }
};

void quit_defer()
{
    mainloop->quit();
}

int main() {
    MailboxPtr pingpong(new CPingPongService(mainloop));
    MailboxPtr ts(new CTestService(mainloop));
    MailboxPtr ps(new CPowerService(mainloop));

    CMessageRouterPtr router(new CMessageRouter());
    router->RegisterMailbox("ping", pingpong);
    router->RegisterMailbox("power", ps);
    std::string address = router->RegisterMailbox(ts);

    std::cout << "Registered test at " << address << std::endl;
/*
    {
        MessagePtr msg(new messages::CRequest(MESSAGE_TYPE_PING, 0));
        router.PostMessage(ts, address, "ping", msg);
    }
*/
/*
    { // Test broadcast
        MessagePtr msg(new CMessage(0));
        router.PostMessage(ts, address, "", msg);
    }
*/

/*
    MessagePtr msg(new messages::CGetProperty("foo", 12));
    router.PostMessage(ts, address, "ping", msg);
*/

    MessagePtr msg(new messages::CShutdown(14));
    router->PostMessage(ts, address, "power", msg);

    mainloop->schedule(quit_defer, 10);
    mainloop->run();

    return 0;
}
