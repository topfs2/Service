#include "MessageRouter.h"
#include <stdexcept>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>

#include <log4cxx/logger.h>

using namespace log4cxx;

static LoggerPtr logger = Logger::getLogger("MessageRouter");

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
    LOG4CXX_DEBUG(logger, "PostMessage sender=" << sender << " destination=" << destination);
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
            LOG4CXX_DEBUG(logger, "PostMessage " << itr->second);
            itr->second->PostMessage(shared_from_this(), sender, destination, msg);
        } else {
          std::cerr << "Tried to send to a mailbox which isn't registered " << destination << std::endl;
          LOG4CXX_ERROR(logger, "Tried to send to a mailbox which isn't registered " << destination);
        }
    }
}
