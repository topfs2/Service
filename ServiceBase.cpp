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

CServiceBase::CServiceBase(MainloopPtr mainloop) : CReactorMailbox(mainloop)
{
}

CServiceBase::~CServiceBase()
{
}

void CServiceBase::HandleMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg)
{
    if (msg->IsType(MESSAGE_TYPE_GET_PROPERTY)) {
        messages::CGetPropertyPtr request = boost::dynamic_pointer_cast<messages::CGetProperty>(msg);

        CVariant value;
        PropertyMap::const_iterator itr = m_properties.find(request->key);
        if (itr != m_properties.end())
            value = itr->second;

        MessagePtr response(new messages::CChangeProperty(request->key, value, request->id));

        source->PostMessage(shared_from_this(), destination, sender, response);
    } else if (msg->IsType(MESSAGE_TYPE_SET_PROPERTY)) {
        messages::CSetPropertyPtr request = boost::dynamic_pointer_cast<messages::CSetProperty>(msg);

        CVariant value = request->value;

        PropertyMap::iterator itr = m_properties.find(request->key);
        if (itr == m_properties.end() || !value.Equals(itr->second)) {
            m_properties[request->key] = value;
            source->PostMessage(shared_from_this(), destination, "", MessagePtr(new messages::CChangeProperty(request->key, value, 0)));
        }

//        source->PostMessage(shared_from_this(), destination, sender, MessagePtr(new messages::CChangeProperty(request->key, value, request->id)));
    }
}
