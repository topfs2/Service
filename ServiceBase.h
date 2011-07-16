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

#include "Variant.h"
#include "ServiceProxy.h"
#include <map>
#include <vector>

template<class S, class C> class CServiceBaseCallback;

// S needs to inherit CServiceBase and C needs to inherit CServiceBaseCallback, otherwise it will fail to compile.
template<class S, class C> class CServiceBase
{
  #define VOID_SIGNAL for (typename CallbackVector::iterator itr = m_callbacks.begin(); itr != m_callbacks.end(); itr++) (*itr)->
  
private:
  typedef std::map<std::string, CVariant> PropertyMap;

public:
  virtual ~CServiceBase() { }

  const CVariant &GetProperty(const std::string &name, const CVariant &fallback = CVariant::ConstNullVariant) const
  {
    PropertyMap::const_iterator itr = m_properties.find(name);
    if (itr == m_properties.end())
      return fallback;
    else
      return itr->second;
  }
  
  inline const CVariant &operator[](const std::string &name) const
  {
    return GetProperty(name);
  }

protected:
  void SetProperty(const std::string &name, const CVariant &variant)
  {
    PropertyMap::iterator itr = m_properties.find(name);
    if (itr == m_properties.end() || !variant.Equals(itr->second))
    {
      m_properties[name] = variant;
      VOID_SIGNAL OnPropertyChange(name, variant);
    }
  }

private:
  bool AttachCallback(C *callback)
  {
    if (callback == NULL)
      return false;
      
    m_callbacks.push_back(callback);
    return true;
  }

  bool DetachCallback(C *callback)
  {
    for (typename CallbackVector::iterator itr = m_callbacks.begin(); itr != m_callbacks.end(); itr++)
    {
      if (*itr == callback)
      {
        m_callbacks.erase(itr);
        return true;
      }
    }

    return false;
  }

protected:
  typedef std::vector<C *> CallbackVector;
  CallbackVector m_callbacks;
private:
  PropertyMap m_properties;

  friend class CServiceBaseCallback<S, C>;
};

template<class S, class C> class CServiceBaseCallback
{
public:
  CServiceBaseCallback()
  {
    CServiceProxy<S> service;
    service->AttachCallback((C *)this);
  }


  virtual ~CServiceBaseCallback()
  {
    CServiceProxy<S> service;
    service->DetachCallback((C *)this);
  }

  virtual void OnPropertyChange(const std::string &name, const CVariant &property) = 0;
};
