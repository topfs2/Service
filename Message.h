#pragma once

#include <string>
#include <boost/shared_ptr.hpp>

class CMessage
{
public:
    CMessage(int type) : type(type) { };
    virtual ~CMessage() { };

    const int Type() const { return type; }
    const bool IsType(int type) const
    {
        return (((this->type & type) > 0) || (this->type == type));
    }

    const int type;
};

typedef boost::shared_ptr<CMessage> MessagePtr;
