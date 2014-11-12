#pragma once

#include "ReactorMailbox.h"
#include "MessagesCore.h"

class CSimpleReactorMailbox : public CReactorMailbox
{
public:
  CSimpleReactorMailbox(MainloopPtr mainloop);
  virtual ~CSimpleReactorMailbox() { };

protected:
  void HandleMessage(MailboxPtr source, std::string sender, std::string destination, MessagePtr msg);

  virtual void HandleNotification(MailboxPtr source, std::string sender, std::string destination, messages::CNotificationPtr msg) { }
  virtual void HandleRequest(MailboxPtr source, std::string sender, std::string destination, messages::CRequestPtr msg) { }
  virtual void HandleResponse(MailboxPtr source, std::string sender, std::string destination, messages::CResponsePtr msg) { }
  virtual void HandleError(MailboxPtr source, std::string sender, std::string destination, messages::CErrorPtr msg) { }
};
