#pragma once
/*
 *      Copyright (C) 2011 Tobias Arrskog
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
#include <typeinfo>
#include <map>

std::map<std::string, void *> service_data;

template<class S> class CService
{
private:
  S *m_ptr;
public:
  CService() : m_ptr(0)
  {
    std::map<std::string, void *>::iterator itr = service_data.find(typeid(S).name());
    if (itr != service_data.end())
      m_ptr = (S *)itr->second;
    else
      throw;
  }

  S *operator-> ()
  {
    return m_ptr;
  }

  static void AddService(void *service)
  {
    service_data[typeid(S).name()] = service;
  }
};
