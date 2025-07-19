#ifndef FIX_H
#define FIX_H

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

template <std::size_t BlockSize, std::size_t BlockCount> class FSAllocator {
public:
  FSAllocator() noexcept { initialize_free_list(); }
  ~FSAllocator() noexcept = default;

  FSAllocator(const FSAllocator &) = delete;
  FSAllocator &operator=(const FSAllocator &) = delete;
  FSAllocator(FSAllocator &&) noexcept = default;
  FSAllocator &operator=(FSAllocator &&) noexcept = default;

public:
  struct Node {
    Node *next;
  };

public:
  /**
   * @brief forward to a new block, return the previous empty block
   */
  Node *allocate() noexcept {
    if (!m_Head) {
      return nullptr;
    }
    Node *ret = m_Head;
    m_Head = m_Head->next;
    return ret;
  }

  void deallocate(Node *ptr) noexcept {
    ptr->next = m_Head;
    m_Head = ptr;
  }

private:
  void initialize_free_list() noexcept {
    auto *base = static_cast<unsigned char *>(m_Buffer.data());
    m_Head = nullptr;
    for (std::size_t i = 0; i < BlockCount; i++) {
      auto *block = base + i * BlockSize;
      auto node = static_cast<Node *>(block);
      node->next = m_Head;
      m_Head = node;
    }
  }

  // bool owns(void *ptr) const noexcept {}

private:
  // std::vector<std::byte> m_Buffer{BlockSize * BlockCount};
  std::array<std::aligned_storage_t<BlockSize, alignof(std::max_align_t)>,
             BlockCount>
      m_Buffer;
  Node *m_Head{nullptr};
};

#endif