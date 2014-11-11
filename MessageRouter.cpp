#include "MessageRouter.h"
#include <stdexcept>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>

void CMessageRouter::RegisterMailbox(const std::string &address, MailboxPtr mailbox)
{
    if (!mailbox)
        throw std::runtime_error("No mailbox");

    threading::CScopedMutex l(m_lock);

    if (m_mailboxes.find(address) != m_mailboxes.end()) {
        throw std::runtime_error("Already bound mailbox to address");
    }

    m_mailboxes[address] = mailbox;
}

std::string CMessageRouter::RegisterMailbox(MailboxPtr mailbox)
{
    if (!mailbox)
        throw std::runtime_error("No mailbox");

    threading::CScopedMutex l(m_lock);

    boost::uuids::random_generator generator;
    std::string uuid;

    do {
        uuid = boost::lexical_cast<std::string>(generator());
    } while (m_mailboxes.find(uuid) != m_mailboxes.end());

    m_mailboxes[uuid] = mailbox;

    return uuid;
}

void CMessageRouter::UnregisterMailbox(const std::string &address)
{
    threading::CScopedMutex l(m_lock);

    MailboxMap::iterator itr = m_mailboxes.find(address);
    if (itr != m_mailboxes.end()) {
        m_mailboxes.erase(itr);
    }
}

void CMessageRouter::PostMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg)
{
    threading::CScopedMutex l(m_lock);

    if (destination.empty()) {
        for (MailboxMap::iterator itr = m_mailboxes.begin(); itr != m_mailboxes.end(); itr++) {
            if (source != itr->second) {
                itr->second->PostMessage(shared_from_this(), sender, destination, msg);
            }
        }
    } else {
        MailboxMap::iterator itr = m_mailboxes.find(destination);
        if (itr != m_mailboxes.end()) {
            itr->second->PostMessage(shared_from_this(), sender, destination, msg);
        }
    }
}
