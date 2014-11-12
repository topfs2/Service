#include "MonitorMailbox.h"

void CMonitorMailbox::PostMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg)
{
    std::cout << "CMonitorMailbox::PostMessage sender=" << sender << " destination=" << destination << std::endl;

    m_condition.acquire();

    m_messages.push_back(msg);
    std::cout << "Got message " << m_messages.size() << std::endl;

    m_condition.notifyAll();

    m_condition.release();
}
