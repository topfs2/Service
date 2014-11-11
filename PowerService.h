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

#include "ServiceBase.h"

class CPowerService : public CServiceBase
{
public:
    CPowerService(MainloopPtr mainloop);

protected:
    void HandleMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg);
};

/*
class CPowerService : public CServiceBase
{
public:
    CPowerService(MainloopPtr mainloop);
    virtual ~CPowerService();

    void Shutdown();
    void Sleep();

    void attachOnShutdown(voidFunction callback);
    void attachOnSleep(voidFunction callback);
    void attachOnWake(voidFunction callback);

private:
    voidSignal onShutdown;
    voidSignal onSleep;
    voidSignal onWake;

    void _Shutdown();
    void _Sleep();
};
*/
