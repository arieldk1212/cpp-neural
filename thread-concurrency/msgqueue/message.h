#ifndef MSG_H
#define MSG_H

#include <string>

class Message {
public:
  Message(std::string Message) : m_Message(Message) {}
  ~Message() = default;

  Message(const Message &other) = default;
  Message &operator=(const Message &other) = default;

  std::string GetMessage() const { return m_Message; }

private:
  std::string m_Message;
};

#endif