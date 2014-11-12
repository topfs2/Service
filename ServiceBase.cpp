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
#include "MessagesProperties.h"
#include "MessagesUtils.h"
#include <log4cxx/logger.h>

using namespace log4cxx;
using namespace messages;

static LoggerPtr logger = Logger::getLogger("ServiceBase");

CServiceBase::CServiceBase(MainloopPtr mainloop) : CReactorMailbox(mainloop)
{
}

CServiceBase::~CServiceBase()
{
}

void CServiceBase::HandleMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg)
{
  LOG4CXX_DEBUG(logger, "HandleMessage sender=" << sender << " destination=" << destination);

  unsigned int id;
  std::string key;
  KeyValue set;
  if (GetRequest<std::string>(msg, "GetProperty", key, id)) {
    LOG4CXX_DEBUG(logger, "GetProperty " << key << " " << id);

    CVariant value;
    PropertyMap::const_iterator itr = m_properties.find(key);
    if (itr != m_properties.end())
        value = itr->second;

    MessagePtr response(new CTemplateResponse<CVariant>(value, id));
    source->PostMessage(shared_from_this(), destination, sender, response);
  } else if (GetRequest<KeyValue>(msg, "SetProperty", set, id)) {
    LOG4CXX_INFO(logger, "Setting property " << set.first);
    m_properties[set.first] = set.second;
  } else {
    LOG4CXX_WARN(logger, "Not ok message");
  }
}
