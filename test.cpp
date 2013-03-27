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

#include "Variant.h"
#include "ServiceProxy.h"
#include "PowerService.h"
#include <iostream>

using namespace std;

class MyTestCallback : public CPowerServiceCallback
{
public:
  MyTestCallback()
  {
    CServiceProxy<CPowerService> service;
    service->AttachCallback((CPowerServiceCallback *)this);
  }

  ~MyTestCallback()
  {
    CServiceProxy<CPowerService> service;
    service->DetachCallback((CPowerServiceCallback *)this);
  }

  virtual void OnPropertyChange(const std::string &name, const CVariant &property)
  {
    cout << "OnPropertyChange(" << name << ", " << (bool)property << ");" << endl;
  }
};

int main()
{
  CServiceProxy<CPowerService> pm;
  bool canPowerdown = pm->GetProperty("CanPowerdown");
  cout << canPowerdown << endl;

  MyTestCallback test;
  
  pm->Test();
  pm->Test();
  
  // Alternative instead of CVariant properties, use Property<T>(const std::string &name, T defaultValue) in CServiceBase and enforce static typing.
  // With this we can do a static_cast to the given property value in the map and have the following API:
  //pm.GetProperty<bool>("CanPowerdown");

  return 0;
}
