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

#include "IMainloop.h"
#include "Locks.h"
#include "Thread.h"
#include <deque>
#include <vector>

class CBaseMainloop : public IExclusiveThreadMainloop
{
public:
    CBaseMainloop();
    virtual ~CBaseMainloop();

    virtual void execute(RunFunction f);

    virtual void schedule(RunFunction f, uint32_t inMS = 0);
    virtual void scheduleAt(RunFunction f, uint64_t atMS = 0);
    virtual void schedulePeriodical(RunFunction f, uint32_t everyMS);

    virtual void quit();

protected:
    void process();

private:
    threading::CCondition m_condition;
    struct RunRequest {
        RunFunction method;
        uint64_t time;
        int scheduledBy;
        bool *signal;
    };

    typedef std::vector<RunRequest> RunQueue;
    RunQueue m_schedule;

    typedef std::vector<RunRequest> RunPeriodicalList;
    RunPeriodicalList m_periodical;

    RunQueue::iterator FindNextScheduled();

    bool m_run;
    threading::thread_id_t m_threadID;
};
