#pragma once

#include "Message.h"

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class IMailbox : public boost::enable_shared_from_this<IMailbox>
{
public:
    virtual ~IMailbox() { };

    virtual void PostMessage(boost::shared_ptr<IMailbox> source, std::string sender, std::string destination, MessagePtr msg) = 0;
};

typedef boost::shared_ptr<IMailbox> MailboxPtr;
