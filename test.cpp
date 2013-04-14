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
#include "SimpleMainloop.h"

CSimpleMainloopPtr mainloop = CSimpleMainloopPtr(new CSimpleMainloop());

void onPropertyChange(std::string key, CVariant value)
{
    std::cout << "onPropertyChange(" << key << ", " << (std::string)value << ")" << std::endl;
}

void onShutdown()
{
    std::cout << "onShutdown" << std::endl;
    mainloop->Quit();
}

void onSleep()
{
    std::cout << "onSleep" << std::endl;
}

class CFirstCallback
{
public:
    void onPropertyChange(std::string key, CVariant value)
    {
        std::cout << "CFirstCallback::onPropertyChange(" << key << ", " << (std::string)value << ")" << std::endl;
    }

    void onShutdown()
    {
        std::cout << "CFirstCallback::onShutdown" << std::endl;
    }

    void onSleep()
    {
        std::cout << "CFirstCallback::onSleep" << std::endl;
    }
};

typedef boost::shared_ptr<CFirstCallback> CFirstCallbackPtr;

/*
 * Notice that using boost::signals2::trackable is discouraged due to it not always being threadsafe.
 */
class CSecondCallback : public boost::signals2::trackable
{
public:
    void onPropertyChange(std::string key, CVariant value)
    {
        std::cout << "CSecondCallback::onPropertyChange(" << key << ", " << (std::string)value << ")" << std::endl;
    }

    void onShutdown()
    {
        std::cout << "CSecondCallback::onShutdown" << std::endl;
    }

    void onSleep()
    {
        std::cout << "CSecondCallback::onSleep" << std::endl;
    }
};

int main() {
    CPowerService pm(mainloop);
    pm.onPropertyChange.connect(&onPropertyChange);
    pm.onShutdown.connect(onShutdown);
    pm.onSleep.connect(onSleep);

    CFirstCallbackPtr ptr(new CFirstCallback());
    pm.onPropertyChange.connect(CServiceBase::propertySignal::slot_type(&CFirstCallback::onPropertyChange, ptr.get(), _1, _2).track(ptr));
    pm.onShutdown.connect(CServiceBase::voidSignal::slot_type(&CFirstCallback::onShutdown, ptr.get()).track(ptr));
    pm.onSleep.connect(CServiceBase::voidSignal::slot_type(&CFirstCallback::onSleep, ptr.get()).track(ptr));

    CSecondCallback *ptrTwo = new CSecondCallback();
    pm.onPropertyChange.connect(boost::bind(&CSecondCallback::onPropertyChange, ptrTwo, _1, _2));
    pm.onShutdown.connect(boost::bind(&CSecondCallback::onShutdown, ptrTwo));
    pm.onSleep.connect(boost::bind(&CSecondCallback::onSleep, ptrTwo));

    std::cout << "==Calling sleep==" << std::endl;
    pm.Sleep();

    delete ptrTwo;

    boost::unique_future<CVariant> f = pm.GetProperty("CanPowerdown", false);
    std::cout << "Has value for CanPowerdown: " << f.has_value() << std::endl;

    std::cout << "==Calling shutdown==" << std::endl;
    pm.Shutdown();

    mainloop->run();

    std::cout << "Has value for CanPowerdown: " << f.has_value() << " and it is " << f.get().asBoolean() << std::endl;
}
