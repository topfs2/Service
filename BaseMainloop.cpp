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

#include "BaseMainloop.h"
#include "Time.h"

#include <log4cxx/logger.h>

using namespace log4cxx;

static LoggerPtr logger = Logger::getLogger("basemainloop");

CBaseMainloop::CBaseMainloop()
{
    m_threadID = 0;
    m_run = false;
}

CBaseMainloop::~CBaseMainloop()
{
    LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Destroying mainloop");
    quit();
}

void CBaseMainloop::execute(RunFunction f)
{
    LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Begin Executing");
    m_condition.acquire();

    if (m_run) {
      threading::thread_id_t threadID = threading::CThread::self();

      if (threadID == m_threadID) {
        LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] " << "Executing " << f);
        f();
      } else {
        RunRequest r;
        r.method = f;
        r.time = timing::GetTimeMS();
        r.scheduledBy = -1;
        r.signal = new bool(false);

        m_schedule.push_back(r);

        m_condition.notifyAll();

        while (!*r.signal) {
          m_condition.wait();
        }

        delete r.signal;
      }
    } else {
      // TODO What to do here?
      LOG4CXX_ERROR(logger, "[" << this << " | " << m_threadID << "] called execute when we aren't running");
    }

    m_condition.release();
    LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] End Executing");
}

void CBaseMainloop::schedule(RunFunction f, uint32_t inMS)
{
    LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Scheduleing in " << inMS << " which is " << timing::GetTimeMS());
    scheduleAt(f, timing::GetTimeMS() + inMS);
}

void CBaseMainloop::scheduleAt(RunFunction f, uint64_t atMS)
{
    LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Scheduleing at " << atMS);
    m_condition.acquire();

    RunRequest r;
    r.method = f;
    r.time = atMS;
    r.scheduledBy = -1;
    r.signal = NULL;

    m_schedule.push_back(r);

    m_condition.notifyAll();
    m_condition.release();
}

void CBaseMainloop::schedulePeriodical(RunFunction f, uint32_t everyMS)
{
    LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Scheduleing periodical " << everyMS);
    m_condition.acquire();
    RunRequest r;
    r.method = f;
    r.time = everyMS;
    r.scheduledBy = -1;
    r.signal = NULL;

    m_periodical.push_back(r);

    r.method = f;
    r.time = timing::GetTimeMS() + everyMS;
    r.scheduledBy = m_periodical.size() - 1;

    m_schedule.push_back(r);

    m_condition.notifyAll();
    m_condition.release();
}

void CBaseMainloop::quit()
{
    LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Quiting mainloop");
    m_run = false;
}

void CBaseMainloop::process()
{
    m_run = true;
    m_threadID = threading::CThread::self();

    LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Starting mainloop");

    m_condition.acquire();

    while (m_run)
    {
        RunQueue::iterator lowest = FindNextScheduled();

        if (lowest != m_schedule.end()) {
            uint64_t now = timing::GetTimeMS();
            LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Have something which is lowest " << now << " >= " << lowest->time);

            if (now >= lowest->time) {
                LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Found something to execute");
                RunFunction f = lowest->method;
                int scheduledBy = lowest->scheduledBy;
                m_schedule.erase(lowest);
                m_condition.release();

                LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Executing " << f);
                f();

                m_condition.acquire();

                if (lowest->signal) {
                  *lowest->signal = true;
                  m_condition.notifyAll();
                }

                // If the scheduled run was a periodical, schedule it again after "everyMS"
                if (scheduledBy >= 0)
                {
                    LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Rescheduling a periodical");
                    RunRequest r = m_periodical[scheduledBy];
                    r.time += now;
                    r.scheduledBy = scheduledBy;

                    m_schedule.push_back(r);
                }
            } else {
                uint64_t dt = lowest->time - now;
                LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Timed wait " << dt);
                m_condition.timedWait(dt);
            }
        } else {
            // Nothing is scheduled, wait indefinetly
            LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Just wait");
            m_condition.wait(); 
        }
    }

    LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] Starting mainloop");
    m_condition.release();
}

// Private
// m_condition must be acquired before executed
CBaseMainloop::RunQueue::iterator CBaseMainloop::FindNextScheduled()
{
    LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] FindNextScheduled");
    RunQueue::iterator lowest = m_schedule.end();

    for (RunQueue::iterator itr = m_schedule.begin(); itr != m_schedule.end(); itr++) {
        if (lowest == m_schedule.end() || itr->time < lowest->time) {
            lowest = itr;
        }
    }

    LOG4CXX_DEBUG(logger, "[" << this << " | " << m_threadID << "] FindNextScheduled " << (lowest != m_schedule.end() ? "found something" : "found nothing"));

    return lowest;
}
