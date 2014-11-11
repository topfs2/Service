#pragma once

#include <map>

#include "IMailbox.h"
#include "Locks.h"

class CMessageRouter : public IMailbox
{
public:
    void RegisterMailbox(const std::string &address, MailboxPtr mailbox);
    std::string RegisterMailbox(MailboxPtr mailbox);
    void UnregisterMailbox(const std::string &address);

    // TODO Should sender be mailbox and let it lookup address instead?
    void PostMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg);
private:
    typedef std::map<std::string, MailboxPtr> MailboxMap;

    threading::CMutex m_lock;
    MailboxMap m_mailboxes;
};

typedef boost::shared_ptr<CMessageRouter> CMessageRouterPtr;
