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

CSimpleMainloop::CSimpleMainloop()
{
  m_run = false;
}

CSimpleMainloop::~CSimpleMainloop()
{
    Quit();
}

void CSimpleMainloop::RunOnce(RunFunction f)
{
    m_condition.acquire();

    m_schedule.push_back(f);

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

    m_condition.acquire();

    while (m_run)
    {
        RunFunction f = NULL;

        f = m_schedule.front();
        if (f)
        {
            m_schedule.pop_front();
            m_condition.release();

            f();

            m_condition.acquire();
        }
    }

    m_condition.release();
}
