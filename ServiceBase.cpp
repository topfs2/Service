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

#include "ServiceBase.h"

using namespace threading;
using namespace boost;

CServiceBase::CServiceBase(MainloopPtr mainloop)
{
    m_mainloop = mainloop;
}

CServiceBase::~CServiceBase()
{
}

unique_future<CVariant> CServiceBase::GetProperty(const std::string &name, const CVariant &fallback)
{
    CVariantPromisePtr p = CVariantPromisePtr(new CVariantPromise());
    m_mainloop->ExecuteOnIdle(boost::bind(&CServiceBase::_GetProperty, this, name, fallback, p));
    return p->get_future();
}

unique_future<CVariant> CServiceBase::operator[](const std::string &name)
{
    return GetProperty(name);
}

void CServiceBase::_GetProperty(std::string name, CVariant fallback, CVariantPromisePtr p)
{
    PropertyMap::const_iterator itr = m_properties.find(name);
    if (itr == m_properties.end())
        p->set_value(fallback);
    else
        p->set_value(itr->second);
}

void CServiceBase::SetProperty(const std::string &name, const CVariant &variant)
{
    m_mainloop->ExecuteOnIdle(boost::bind(&CServiceBase::_SetProperty, this, name, variant));
}

void CServiceBase::_SetProperty(const std::string &name, const CVariant &variant)
{
    PropertyMap::iterator itr = m_properties.find(name);
    if (itr == m_properties.end() || !variant.Equals(itr->second))
    {
        m_properties[name] = variant;
        onPropertyChange(name, variant);
    }
}
