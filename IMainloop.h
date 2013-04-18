#pragma once
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

class IMainloop
{
public:
//    typedef void (*RunFunction)(void);
    typedef boost::function<void ()> RunFunction;

    virtual ~IMainloop() { };

    virtual void ExecuteOnIdle(RunFunction f) = 0;
/*
    virtual void ExecutePeriodical(RunFunction f, uint32_t everyMS) = 0;
    virtual void ExecuteAt(RunFunction f, uint32_t atMS) = 0;
*/

    virtual void Quit() = 0;
};

typedef boost::shared_ptr<IMainloop> MainloopPtr;
