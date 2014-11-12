#pragma once

#include "IMailbox.h"
#include "IMainloop.h"
#include "Locks.h"
#include "MessagesCore.h"

#include <vector>

class CMonitorMailbox : public IMailbox
{
public:
  CMonitorMailbox() { }
  virtual ~CMonitorMailbox() { };

  void PostMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg);

protected:
  threading::CCondition m_condition;

private:
  typedef std::vector<MessagePtr> MessageQueue;
  MessageQueue m_messages;

public:
  template <class T>
  T GetResult(unsigned int id)
  {
    T result;
    std::string error_message;
    int state = 0; // 0 nothing, 1 error, 2 result

    m_condition.acquire();

    while (state == 0) {
        for (MessageQueue::iterator itr = m_messages.begin(); itr != m_messages.end() && state == 0; itr++)
        {
            if ((*itr)->IsType(MESSAGE_TYPE_RESPONSE))
            {
                messages::CResponsePtr response = boost::dynamic_pointer_cast<messages::CResponse>((*itr));
                m_messages.erase(itr);

                if (response && response->id == id) {
                    boost::shared_ptr<messages::CTemplateResponse<T> > template_response = boost::dynamic_pointer_cast<messages::CTemplateResponse<T> >(response);

                    if (template_response) {
                        result = template_response->result;
                        state = 2;
                    } else {
                        // TODO Handle error
                        state = 1;
                        error_message = "Bad result cast";
                    }
                }

                m_condition.notifyAll();
            } else if ((*itr)->IsType(MESSAGE_TYPE_ERROR)) {
                messages::CErrorPtr error = boost::dynamic_pointer_cast<messages::CError>((*itr));
                m_messages.erase(itr);

                state = 1;
                error_message = error ? error->message : "Bad error cast";

                m_condition.notifyAll();
            }
        }

        if (state == 0) {
            m_condition.wait();
        }
    }

    m_condition.release();

    if (state == 2) {
        return result;
    }

    throw error_message;
  }
};
