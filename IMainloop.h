#pragma once
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

class IMainloop
{
public:
//    typedef void (*RunFunction)(void);
    typedef boost::function<void ()> RunFunction;

    virtual ~IMainloop() { };

    template <typename R>
    R execute(boost::function<R ()> f)
    {
      R r;
      execute(boost::bind(executeWrapper<R>, &r, f));
      return r;
    }

    template <typename R>
    void schedule(boost::function<R ()> f, boost::function<void (R)> callback, boost::shared_ptr<IMainloop> mainloop, uint32_t inMS = 0)
    {
      if (mainloop)
        schedule(boost::bind(scheduleResultWrapper<R>, f, callback, mainloop), inMS);
    }

    template <typename R>
    void scheduleAt(boost::function<R ()> f, boost::function<void (R)> callback, boost::shared_ptr<IMainloop> mainloop, uint32_t atMS)
    {
      if (mainloop)
        scheduleAt(boost::bind(scheduleResultWrapper<R>, f, callback, mainloop), atMS);
    }

    void schedule(RunFunction f, RunFunction callback, boost::shared_ptr<IMainloop> mainloop, uint32_t inMS = 0)
    {
      if (mainloop)
        schedule(boost::bind(scheduleWrapper, f, callback, mainloop), inMS);
    }

    void scheduleAt(RunFunction f, RunFunction callback, boost::shared_ptr<IMainloop> mainloop, uint32_t atMS)
    {
      if (mainloop)
        scheduleAt(boost::bind(scheduleWrapper, f, callback, mainloop), atMS);
    }

    /* execute blocks until the function has executed */
    virtual void execute(RunFunction f) = 0;

    /* schedule returns imediately and the function will execute as close to the timing possible */
    virtual void schedule(RunFunction f, uint32_t inMS = 0) = 0;
    virtual void scheduleAt(RunFunction f, uint32_t atMS) = 0;
    /* schedulePeriodical will execute as close to everyMS as possible */
    virtual void schedulePeriodical(RunFunction f, uint32_t everyMS) = 0;

    virtual void quit() = 0;

private:
    template <typename R>
    static void executeWrapper(R *result, boost::function<R ()> f)
    {
      *result = f();
    }

    static void scheduleWrapper(RunFunction f, RunFunction callback, boost::shared_ptr<IMainloop> mainloop)
    {
      f();
      mainloop->schedule(callback);
    }

    template <typename R>
    static void scheduleResultWrapper(boost::function<R ()> f, boost::function<void (R)> callback, boost::shared_ptr<IMainloop> mainloop)
    {
      R r = f();
      mainloop->schedule(boost::bind(callback, r));
    }
};

typedef boost::shared_ptr<IMainloop> MainloopPtr;

/*
    Anything extending ISingleThreadedMainloop promise that nothing will execute in parallell.
*/
class ISingleThreadedMainloop : public IMainloop
{
public:
    virtual ~ISingleThreadedMainloop() { }
};

typedef boost::shared_ptr<ISingleThreadedMainloop> SingleThreadedMainloopPtr;

/*
    Anything extending IExclusiveThreadMainloop promise that every execution will be done by the same thread.
*/
class IExclusiveThreadMainloop : public ISingleThreadedMainloop
{
public:
    virtual ~IExclusiveThreadMainloop() { }
};

typedef boost::shared_ptr<IExclusiveThreadMainloop> ExclusiveThreadMainloopPtr;
