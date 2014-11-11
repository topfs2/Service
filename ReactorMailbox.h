#pragma once

#include "IMailbox.h"
#include "IMainloop.h"

class CReactorMailbox : public IMailbox
{
public:
  CReactorMailbox(MainloopPtr mainloop) : m_mainloop(mainloop) { }
  virtual ~CReactorMailbox() { };

  void PostMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg);

protected:
  virtual void HandleMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg) = 0;

  MainloopPtr m_mainloop;
};
