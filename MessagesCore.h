#pragma once

#include "Message.h"

#define MESSAGE_TYPE_ERROR          (0x0)
#define MESSAGE_TYPE_NOTIFICATION   (0x1)
#define MESSAGE_TYPE_REQUEST        (0x2)
#define MESSAGE_TYPE_RESPONSE       (0x4)

namespace messages
{
    class IPayload
    {
      virtual ~IPayload() { };
    };

    typedef boost::shared_ptr<IPayload> CPayloadPtr;

    class CNotification : public CMessage
    {
    public:
        CNotification(const std::string &method) : CMessage(MESSAGE_TYPE_NOTIFICATION), method(method) { };
        virtual ~CNotification() { };

        const std::string method;
    };

    class CRequest : public CMessage
    {
    public:
        CRequest(const std::string &method, unsigned int id = 0) : CMessage(MESSAGE_TYPE_REQUEST), method(method), id(id) { };
        virtual ~CRequest() { };

        const std::string method;
        const unsigned int id;
    };

    class CResponse : public CMessage
    {
    public:
        CResponse(unsigned int id) : CMessage(MESSAGE_TYPE_RESPONSE), id(id) { };
        virtual ~CResponse() { };

        const unsigned int id;
    };

    template <class T>
    class CTemplateNotification : public CNotification
    {
    public:
        CTemplateNotification(const std::string &method, const T &arguments) : CNotification(method), arguments(arguments) { };
        virtual ~CTemplateNotification() { };

        const T arguments;
    };

    template <class T>
    class CTemplateRequest : public CRequest
    {
    public:
        CTemplateRequest(const std::string &method, const T &arguments, unsigned int id = 0) : CRequest(method, id), arguments(arguments) { };
        virtual ~CTemplateRequest() { };

        const T arguments;
    };

    template <class T>
    class CTemplateResponse : public CResponse
    {
    public:
        CTemplateResponse(const T &result, unsigned int id) : CResponse(id), result(result) { };
        virtual ~CTemplateResponse() { };

        const T result;
    };

    class CError : public CMessage
    {
    public:
        CError(int code, const std::string &message, unsigned int id = 0) : CMessage(MESSAGE_TYPE_ERROR), code(code), message(message), id(id) { };
        virtual ~CError() { };

        const unsigned int code;
        const std::string message;
        const unsigned int id;
    };

    typedef boost::shared_ptr<CError> CErrorPtr;
    typedef boost::shared_ptr<CNotification> CNotificationPtr;
    typedef boost::shared_ptr<CRequest> CRequestPtr;
    typedef boost::shared_ptr<CResponse> CResponsePtr;
}
