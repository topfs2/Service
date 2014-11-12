#pragma once

#include "MessagesCore.h"

template <class T>
bool GetArguments(MessagePtr msg, T &arguments)
{
  boost::shared_ptr<messages::CTemplateRequest<T> > request = boost::dynamic_pointer_cast<messages::CTemplateRequest<T> >(msg);
  if (request) {
    arguments = request->arguments;
    return true;
  }

  return false;
}

template <class T>
bool GetRequest(MessagePtr msg, std::string method, T &arguments, unsigned int &id)
{
  if (msg->IsType(MESSAGE_TYPE_REQUEST)) {
    messages::CRequestPtr request = boost::dynamic_pointer_cast<messages::CRequest>(msg);

    if (request->method == method) {
      id = request->id;
      return GetArguments<T>(msg, arguments);
    }
  }

  return false;
}
/*
bool GetRequest(MessagePtr msg, std::string method, unsigned int &id)
{
  if (msg->IsType(MESSAGE_TYPE_REQUEST)) {
    messages::CRequestPtr request = boost::dynamic_pointer_cast<messages::CRequest>(msg);

    if (request->method == method) {
      return true;
    } else {
      std::cout << "Not fitting the method" << std::endl;
    }
  } else {
    std::cout << "Not a request" << std::endl;
  }

  return false;
}
*/

template <class T>
bool GetResult(MessagePtr msg, T &result)
{
  boost::shared_ptr<messages::CTemplateResponse<T> > response = boost::dynamic_pointer_cast<messages::CTemplateResponse<T> >(msg);
  if (response) {
    result = response->result;
    return true;
  }

  return false;
}
