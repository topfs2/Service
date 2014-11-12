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

#include "PowerService.h"
#include "MessagesPower.h"

CPowerService::CPowerService(MainloopPtr mainloop) : CServiceBase(mainloop)
{
  m_properties["CanShutdown"]  = true;
  m_properties["CanSuspend"]   = true;
  m_properties["CanHibernate"] = true;
  m_properties["CanReboot"]    = true;
}

void CPowerService::HandleMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg)
{
/*
    if (msg->IsType(MESSAGE_TYPE_SHUTDOWN)) {
        std::cout << "Shutdown" << std::endl;

        source->PostMessage(shared_from_this(), destination, "", MessagePtr(new messages::COnShutdown()));
    } else if (msg->IsType(MESSAGE_TYPE_SLEEP)) {
        std::cout << "Sleep" << std::endl;

        source->PostMessage(shared_from_this(), destination, "", MessagePtr(new messages::COnSleep()));
    } else {
        CServiceBase::HandleMessage(source, sender, destination, msg);
    }
*/

  CServiceBase::HandleMessage(source, sender, destination, msg);
}
/*
CPowerService::CPowerService(MainloopPtr mainloop) : CServiceBase(mainloop)
{
}

CPowerService::~CPowerService()
{
}

void CPowerService::Shutdown()
{
    m_mainloop->schedule(boost::bind(&CPowerService::_Shutdown, this));
}

void CPowerService::Sleep()
{
    m_mainloop->schedule(boost::bind(&CPowerService::_Sleep, this));
}

void CPowerService::attachOnShutdown(voidFunction callback)
{
    onShutdown.connect(callback);
}

void CPowerService::attachOnSleep(voidFunction callback)
{
    onSleep.connect(callback);
}

void CPowerService::attachOnWake(voidFunction callback)
{
    onWake.connect(callback);
}

void CPowerService::_Shutdown()
{
    onShutdown();
}

void CPowerService::_Sleep()
{
    onSleep();
}
*/
