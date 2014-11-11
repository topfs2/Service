#include "ReactorMailbox.h"

void CReactorMailbox::PostMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg)
{
    m_mainloop->schedule(boost::bind(&CReactorMailbox::HandleMessage, this, source, sender, destination, msg));
}
