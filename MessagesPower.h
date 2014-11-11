#pragma once

#include "MessagesCore.h"

#define MESSAGE_TYPE_ON_SHUTDOWN    (0x100)
#define MESSAGE_TYPE_ON_SLEEP       (0x200)
#define MESSAGE_TYPE_ON_WAKE        (0x400)

#define MESSAGE_TYPE_SHUTDOWN       (0x800)
#define MESSAGE_TYPE_SLEEP          (0x1000)

namespace messages
{
    class COnShutdown : public CMessage
    {
    public:
        COnShutdown() : CMessage(MESSAGE_TYPE_ON_SHUTDOWN) { };
        virtual ~COnShutdown() { };
    };

    class COnSleep : public CMessage
    {
    public:
        COnSleep() : CMessage(MESSAGE_TYPE_ON_SLEEP) { };
        virtual ~COnSleep() { };
    };

    class COnWake : public CMessage
    {
    public:
        COnWake() : CMessage(MESSAGE_TYPE_ON_WAKE) { };
        virtual ~COnWake() { };
    };

    class CShutdown : public CMessage
    {
    public:
        CShutdown(unsigned int id) : CMessage(MESSAGE_TYPE_SHUTDOWN, id) { };
        virtual ~CShutdown() { };
    };

    class CSleep : public CMessage
    {
    public:
        CSleep(unsigned int id) : CMessage(MESSAGE_TYPE_SLEEP, id) { };
        virtual ~CSleep() { };
    };

    typedef boost::shared_ptr<COnShutdown> COnShutdownPtr;
    typedef boost::shared_ptr<COnSleep> COnSleepPtr;
    typedef boost::shared_ptr<COnWake> COnWakePtr;

    typedef boost::shared_ptr<CShutdown> CShutdownPtr;
    typedef boost::shared_ptr<CSleep> CSleepPtr;
}
