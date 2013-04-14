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

#include "Locks.h"
#include <sys/time.h>

using namespace threading;

CMutex::CMutex()
{
    pthread_mutex_init(&m_mutex, NULL);
}

CMutex::~CMutex()
{
    pthread_mutex_destroy(&m_mutex);
}

void CMutex::acquire()
{
    pthread_mutex_lock(&m_mutex);
}

void CMutex::release()
{
    pthread_mutex_unlock(&m_mutex);
}

CCondition::CCondition()
{
    pthread_cond_init(&m_condition, NULL);
}

CCondition::~CCondition()
{
    pthread_cond_destroy(&m_condition);
}

void CCondition::wait()
{
    pthread_cond_wait(&m_condition, &m_mutex);
}

void CCondition::timedWait(long msec)
{
    struct timespec timeToWait;
    struct timeval now;

    gettimeofday(&now,NULL);

    timeToWait.tv_sec = now.tv_sec;
    timeToWait.tv_nsec = (now.tv_usec + msec * 1000UL) * 1000UL;

    pthread_cond_timedwait(&m_condition, &m_mutex, &timeToWait);
}

void CCondition::notifyAll()
{
    pthread_cond_broadcast(&m_condition);
}

CScopedMutex::CScopedMutex(CMutex &mutex) : m_mutex(mutex)
{
    m_mutex.acquire();
}

CScopedMutex::~CScopedMutex()
{
    m_mutex.release();
}
