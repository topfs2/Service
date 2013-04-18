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

#include "SimpleMainloop.h"
#include "Time.h"

CSimpleMainloop::CSimpleMainloop()
{
    m_threadID = 0;
    m_run = false;
}

CSimpleMainloop::~CSimpleMainloop()
{
    Quit();
}

void CSimpleMainloop::ExecuteOnIdle(RunFunction f)
{
    ExecuteAt(f, timing::GetTimeMS());
}

void CSimpleMainloop::ExecutePeriodical(RunFunction f, uint32_t everyMS)
{
    m_condition.acquire();
    RunRequest r;
    r.method = f;
    r.time = everyMS;
    r.scheduledBy = -1;

    m_periodical.push_back(r);

    r.method = f;
    r.time = timing::GetTimeMS() + everyMS;
    r.scheduledBy = m_periodical.size() - 1;

    m_schedule.push_back(r);

    m_condition.notifyAll();
    m_condition.release();
}

void CSimpleMainloop::ExecuteAt(RunFunction f, uint32_t atMS)
{
    m_condition.acquire();

    RunRequest r;
    r.method = f;
    r.time = atMS;
    r.scheduledBy = -1;

    m_schedule.push_back(r);

    m_condition.notifyAll();
    m_condition.release();
}

void CSimpleMainloop::Quit()
{
    m_run = false;
}

void CSimpleMainloop::run()
{
    m_run = true;
    m_threadID = threading::CThread::self();

    m_condition.acquire();

    while (m_run)
    {
        RunQueue::iterator lowest = FindNextScheduled();

        if (lowest != m_schedule.end()) {
            long now = timing::GetTimeMS();
            if (now >= lowest->time) {
                RunFunction f = lowest->method;
                int scheduledBy = lowest->scheduledBy;
                m_schedule.erase(lowest);
                m_condition.release();

                f();

                m_condition.acquire();

                // If the scheduled run was a periodical, schedule it again after "everyMS"
                if (scheduledBy >= 0)
                {
                    RunRequest r = m_periodical[scheduledBy];
                    r.time += now;
                    r.scheduledBy = scheduledBy;

                    m_schedule.push_back(r);
                }
            } else {
                m_condition.timedWait(lowest->time - now);
            }
        } else {
            // Nothing is scheduled, wait indefinetly
            m_condition.wait(); 
        }
    }

    m_condition.release();
}

// Private
// m_condition must be acquired before executed
CSimpleMainloop::RunQueue::iterator CSimpleMainloop::FindNextScheduled()
{
    RunQueue::iterator lowest = m_schedule.end();

    for (RunQueue::iterator itr = m_schedule.begin(); itr != m_schedule.end(); itr++) {
        if (lowest == m_schedule.end() || itr->time < lowest->time) {
            lowest = itr;
        }
    }

    return lowest;
}
