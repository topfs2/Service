#include "ReactorMailbox.h"
#include <log4cxx/logger.h>

using namespace log4cxx;

static LoggerPtr logger = Logger::getLogger("ReactorMailbox");

void CReactorMailbox::PostMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg)
{
    LOG4CXX_DEBUG(logger, "PostMessage sender=" << sender << " destination=" << destination << " mainloop=" << m_mainloop);
    m_mainloop->schedule(boost::bind(&CReactorMailbox::HandleMessage, this, source, sender, destination, msg));
}
