#pragma once

#include "Message.h"

#define MESSAGE_TYPE_ERROR          (0x0)
#define MESSAGE_TYPE_NOTIFICATION   (0x1)
#define MESSAGE_TYPE_REQUEST        (0x2)
#define MESSAGE_TYPE_RESPONSE       (0x4)

namespace messages
{
    class CNotification : public CMessage
    {
    public:
        CNotification(int message_type) : CMessage(message_type | MESSAGE_TYPE_NOTIFICATION) { };
        virtual ~CNotification() { };
    };

    class CRequest : public CMessage
    {
    public:
        CRequest(int message_type, unsigned int id) : CMessage(message_type | MESSAGE_TYPE_REQUEST), id(id) { };
        virtual ~CRequest() { };
    };

    class CResponse : public CMessage
    {
    public:
        CResponse(int message_type, unsigned int id) : CMessage(message_type | MESSAGE_TYPE_RESPONSE), id(id) { };
        virtual ~CResponse() { };
    };

    class CError : public CMessage
    {
    public:
        CError(int code, std::string &message, unsigned int id = 0) : CMessage(MESSAGE_TYPE_ERROR), code(code), message(message), id(id) { };
        virtual ~CError() { };

        const unsigned int code;
        const std::string message;
    };

    typedef boost::shared_ptr<CError> CErrorPtr;
    typedef boost::shared_ptr<CNotification> CNotificationPtr;
    typedef boost::shared_ptr<CRequest> CRequestPtr;
    typedef boost::shared_ptr<CResponse> CResponsePtr;
}
