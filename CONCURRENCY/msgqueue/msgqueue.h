#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include "message.h"

#include <memory>
#include <queue>
#include <vector>

class MessageQueue {
public:
  MessageQueue();
  MessageQueue(const std::queue<Message> &Messages) : m_Messages(Messages) {}
  ~MessageQueue() = default;

  MessageQueue(const MessageQueue &) = delete;
  MessageQueue &operator=(const MessageQueue &) = delete;

  void Pop() { m_Messages.pop(); }
  void Push(Message Message) { m_Messages.push(Message); }
  Message Front() const { return m_Messages.front(); }
  Message Back() const { return m_Messages.back(); }

  size_t GetSize() const { return m_Messages.size(); }
  bool IsEmpty() const { return m_Messages.empty(); }

private:
  std::queue<Message> m_Messages;
};

#endif
