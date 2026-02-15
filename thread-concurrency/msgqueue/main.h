#ifndef MAIN_H
#define MAIN_H

#include "message.h"
#include "msgqueue.h"

#include <iostream>
#include <string>
#include <unordered_map>

class Subscriber;

class Broker {
public:
  void Subscribe(const std::string &Topic,
                 std::unique_ptr<Subscriber> Subscriber) {
    m_Subscribers[Topic].emplace_back(std::move(Subscriber));
  }
  void Publish(const std::string &Topic, Message Message) {
    m_Topics[Topic].Push(Message);
    for (auto &sub : m_Subscribers[Topic]) {
    }
  }

private:
  std::unordered_map<std::string, MessageQueue> m_Topics;
  std::unordered_map<std::string, std::vector<std::unique_ptr<Subscriber>>>
      m_Subscribers;
};

class Subscriber {
public:
  void subscribe(const std::string &Topic) {
    m_Broker->Subscribe(Topic, std::unique_ptr<Subscriber>());
  }
  void receive(Message Message, const std::string &Topic) {
    std::cout << "Received message: " << Message.GetMessage()
              << ", from topic: " << Topic;
  }

private:
  std::shared_ptr<Broker> m_Broker;
};

class Publisher {
public:
  Publisher(std::shared_ptr<Broker> Broker) : m_Broker(Broker) {}

  void Publish(const std::string &Topic, Message Message) {
    m_Broker->Publish(Topic, Message);
  }

private:
  std::shared_ptr<Broker> m_Broker;
};

#endif