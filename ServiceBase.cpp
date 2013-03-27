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

CServiceBase::CServiceBase()
{
}

CServiceBase::~CServiceBase()
{
}

CVariant CServiceBase::GetProperty(const std::string &name, const CVariant &fallback) const
{
    PropertyMap::const_iterator itr = m_properties.find(name);
    if (itr == m_properties.end())
      return fallback;
    else
      return itr->second;
}

CVariant CServiceBase::operator[](const std::string &name) const
{
    return GetProperty(name);
}

void CServiceBase::SetProperty(const std::string &name, const CVariant &variant)
{
    PropertyMap::iterator itr = m_properties.find(name);
    if (itr == m_properties.end() || !variant.Equals(itr->second))
    {
        m_properties[name] = variant;
        onPropertyChange(name, variant);
    }
}
