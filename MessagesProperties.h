#pragma once

#include "Variant.h"
#include "MessagesCore.h"

#define MESSAGE_TYPE_GET_PROPERTY      (0x10)
#define MESSAGE_TYPE_SET_PROPERTY      (0x20)

namespace messages
{
    class CGetProperty : public CMessage
    {
    public:
        CGetProperty(const std::string &key, unsigned int id = 0) : key(key), CMessage(MESSAGE_TYPE_GET_PROPERTY, id) { };
        virtual ~CGetProperty() { };

        const std::string key;
    };

    class CSetProperty : public CMessage
    {
    public:
        CSetProperty(const std::string &key, const CVariant &value, unsigned int id = 0) : key(key), value(value), CMessage(MESSAGE_TYPE_SET_PROPERTY, id) { };
        virtual ~CSetProperty() { };

        const std::string key;
        const CVariant value;
    };

    typedef boost::shared_ptr<CGetProperty> CGetPropertyPtr;
    typedef boost::shared_ptr<CSetProperty> CSetPropertyPtr;
}
