#ifndef IHASHMAP_H
#define IHASHMAP_H

#include <cstddef>

template <typename Key, typename Value> class HMNode {
public:
  HMNode(Key key, Value value) : m_Key(key), m_Value(value) {}

  Key GetKey() const { return m_Key; }
  Value GetValue() const { return m_Value; }
  HMNode *GetNext() { return m_Next; }

  void SetKey(Key key) { m_Key = key; }
  void SetValue(Value value) { m_Value = value; }
  void SetNext(HMNode *next) { m_Next = next; }

private:
  Key m_Key;
  Value m_Value;
  HMNode *m_Next;
};

template <typename Key> struct Hashed {
  unsigned long operator()(const Key &key, size_t TableSize) const {
    return static_cast<unsigned long>(key % TableSize);
  }
};

template <typename Key, typename Value, typename Functor = Hashed<Key>>
class IHashMap {
public:
  IHashMap(size_t TableSize)
      : m_TableSize(TableSize), m_Table(new HMNode<Key, Value> *[m_TableSize]) {
  }
  ~IHashMap() {
    for (int i = 0; i < m_TableSize; i++) {
      auto curr = m_Table[i];
      while (curr != nullptr) {
        auto prev = curr;
        curr = curr->GetNext();
        delete prev;
      }
      m_Table[i] = nullptr;
    }
    if (m_Table != nullptr) {
      delete[] m_Table;
    }
  }

  bool Get(const Key &key, Value &value) const {
    auto HashValue = m_HashFunc(key);
    HMNode<Key, Value> *curr = m_Table[HashValue];
    while (curr != nullptr) {
      if (curr->GeyKey() == key) {
        value = curr->GetValue();
        return true;
      }
      curr = curr->next;
    }
    return false;
  }

  void Put(const Key &key, const Value &value) {
    auto HashValue = m_HashFunc(key);
    HMNode<Key, Value> *curr = m_HashFunc[HashValue];
    HMNode<Key, Value> *prev = nullptr;

    while (curr != nullptr && curr->GetKey() != key) {
      prev = curr;
      curr = curr->GetNext();
    }

    if (curr == nullptr) {
      auto node = new HMNode(key, value);
      if (prev == nullptr) {
        m_Table[HashValue] = node;
      } else {
        prev->SetNext(node);
      }
    } else {
      curr->SetValue(value);
    }
  }

  void Remove(const Key &key) {
    auto HashValue = m_HashFunc(key);
    HMNode<Key, Value> *curr = m_HashFunc[HashValue];
    HMNode<Key, Value> *prev = nullptr;

    while (curr != nullptr && curr->GetKey() != key) {
      prev = curr;
      curr = curr->GetNext();
    }
    if (curr == nullptr) {
      return; // no key found
    }
    if (prev == nullptr) {
      m_Table[HashValue] = curr->GetNext();
    } else {
      prev->SetNext(curr->GetNext());
    }
    delete curr;
  }

private:
  size_t m_TableSize;
  Functor m_HashFunc;
  HMNode<Key, Value> **m_Table;
};

#endif