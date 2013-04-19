#pragma once
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

#include <string>
#include <boost/signals2/signal.hpp>
#include <boost/shared_ptr.hpp>

#include "Locks.h"
#include "Variant.h"
#include "IMainloop.h"

class CServiceBase
{
public:
    CServiceBase(MainloopPtr mainloop);
    virtual ~CServiceBase();

    typedef boost::function<void ()> voidFunction;
    typedef boost::signals2::signal<void ()> voidSignal;

    typedef boost::signals2::signal<void (std::string, CVariant)> propertySignal;
    typedef boost::function<void (std::string, CVariant)> PropertyChangedFunction;
    void attachOnPropertyChange(PropertyChangedFunction callback);

    void GetProperty(const std::string &name, const CVariant &fallback, PropertyChangedFunction callback);
    void GetProperty(const std::string &name, PropertyChangedFunction callback);

protected:
    void SetProperty(const std::string &name, const CVariant &variant);

    MainloopPtr m_mainloop;

private:
    propertySignal onPropertyChange;

    void _GetProperty(std::string name, CVariant fallback, PropertyChangedFunction callback);
    void _SetProperty(const std::string &name, const CVariant &variant);

    typedef std::map<std::string, CVariant> PropertyMap;
    PropertyMap m_properties;
};
