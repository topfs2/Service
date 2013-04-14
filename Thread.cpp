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

#include "Thread.h"

using namespace threading;

void CThread::start()
{
    pthread_create(&m_thread, NULL, staticRun, this);
}

void CThread::join()
{
    pthread_join(m_thread, NULL);
}

void CThread::interrupt()
{
    m_running = false;
}

void *CThread::staticRun(void *arg)
{
    if (arg == NULL)
        return NULL;

    CThread *thread = (CThread *)arg;

    thread->m_running = true;
    thread->run();
}
