#include "SimpleReactorMailbox.h"
#include <log4cxx/logger.h>

using namespace log4cxx;
using namespace messages;

static LoggerPtr logger = Logger::getLogger("SimpleReactorMailbox");

CSimpleReactorMailbox::CSimpleReactorMailbox(MainloopPtr mainloop) : CReactorMailbox(mainloop)
{
}

void CSimpleReactorMailbox::HandleMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg)
{
    LOG4CXX_DEBUG(logger, "Got message " << msg->type);
    if (msg->IsType(MESSAGE_TYPE_NOTIFICATION)) {
        CNotificationPtr notification = boost::dynamic_pointer_cast<CNotification>(msg);
        if (notification) {
            LOG4CXX_DEBUG(logger, "HandleNotification");
            HandleNotification(source, sender, destination, notification);
        }
    } else if (msg->IsType(MESSAGE_TYPE_REQUEST)) {
        CRequestPtr request = boost::dynamic_pointer_cast<CRequest>(msg);
        if (request) {
            LOG4CXX_DEBUG(logger, "HandleRequest");
            HandleRequest(source, sender, destination, request);
        }
    } else if (msg->IsType(MESSAGE_TYPE_RESPONSE)) {
        CResponsePtr response = boost::dynamic_pointer_cast<CResponse>(msg);
        if (response) {
            LOG4CXX_DEBUG(logger, "HandleResponse");
            HandleResponse(source, sender, destination, response);
        }
    } else if (msg->IsType(MESSAGE_TYPE_ERROR)) {
        CErrorPtr error = boost::dynamic_pointer_cast<CError>(msg);
        if (error) {
            LOG4CXX_DEBUG(logger, "HandleError");
            HandleError(source, sender, destination, error);
        }
    }
}
