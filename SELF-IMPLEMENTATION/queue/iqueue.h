// implementing a queue using an existing data structure
#ifndef IQUEUE_H
#define IQUEUE_H

#include <deque>

template <typename T> class IQueue {
public:
  IQueue() = default;
  ~IQueue() = default;

  IQueue(const IQueue &) = delete;
  IQueue &operator=(const IQueue &) = delete;
  IQueue(IQueue &&) = delete;
  IQueue &operator=(IQueue &&) = delete;

  void push(T data) { m_Queue.push_back(data); }
  void pop() {
    if (!empty()) {
      m_Queue.pop_front();
    }
  };

  T &front() const { return m_Queue.front(); }
  T &back() const { return m_Queue.back(); }

  bool empty() { return m_Queue.empty(); }
  std::size_t size() const { return m_Queue.size(); }

private:
  std::deque<T> m_Queue;
};
#endif