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

#include <pthread.h>

namespace threading
{
    class CMutex
    {
    public:
        CMutex();
        virtual ~CMutex();

        void acquire();
        void release();

    protected:
        pthread_mutex_t m_mutex;
    };

    class CCondition : public CMutex
    {
    public:
        CCondition();
        virtual ~CCondition();

        void wait();
        void timedWait(long msec);
        void notifyAll();

    protected:
        pthread_cond_t m_condition;
    };

    class CScopedMutex
    {
    public:
        CScopedMutex(CMutex &mutex);
        ~CScopedMutex();

    private:
        CMutex m_mutex;
    };
};
