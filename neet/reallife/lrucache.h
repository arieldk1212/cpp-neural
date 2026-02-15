#ifndef LRU_H
#define LRU_H

#include <list>
#include <unordered_map>

template <typename KeyType, typename ValueType> class LRUCache {
public:
  LRUCache(std::size_t Capacity) : m_Capacity(Capacity) {}
  ~LRUCache() = default;

  void put(const KeyType &Key, const ValueType &Value) {
    if (size() != capacity()) {
      m_CacheData[Key] = Value;
      if (std::find(m_LRU.begin(), m_LRU.end(), Key) != m_LRU.end()) {
        m_LRU.remove(Key);
        m_LRU.push_front(Key);
      } else {
        m_LRU.push_front(Key);
      }
    } else {
      KeyType old = m_LRU.back();
      m_LRU.pop_back();
      m_CacheData.erase(old);
    }
  }

  bool get(const KeyType &Key, ValueType &Value) {
    auto it = m_CacheData.find(Key);
    if (it != m_CacheData.end()) {
      Value = it->second;
      m_LRU.remove(Key);
      m_LRU.push_front(Key);
      return true;
    }
    return false;
  }

  size_t size() const { return m_CacheData.size(); }
  size_t capacity() const { return m_Capacity; }

private:
  std::size_t m_Capacity;
  std::list<KeyType> m_LRU;
  std::unordered_map<KeyType, ValueType> m_CacheData;
};

#endif