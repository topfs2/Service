#pragma once

#include <string>
#include <boost/shared_ptr.hpp>

class CMessage
{
public:
    CMessage(int type, unsigned int id = 0) : type(type), id(id) { };
    virtual ~CMessage() { };

    const int Type() const { return type; }
    const bool IsType(int type) const
    {
        return (((this->type & type) > 0) || (this->type == type));
    }

    const int type;
    const unsigned int id;
};

typedef boost::shared_ptr<CMessage> MessagePtr;
