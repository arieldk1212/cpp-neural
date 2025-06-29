#ifndef SPAN_PRAC_H
#define SPAN_PRAC_H

/**
 * @brief a non-owning view over a contiguous sequence of objects.
 * @brief an object that can refer to a contigiuous sequence of objects with the
 * first element of the sequence at position zero. it can have either static
 * extent in which case the number of elements in the sequence is known at
 * compile-time, or a dynamic extent.
 * @param T - element type.
 * @param Extent - the number of elements in the sequence, or
 * std::dynamic_extent if dynamic.
 * its data member holds a pointer to the underlying sequence.
 * non owning, non copying, non allocating container.
 */

/* EXAMPLES */

#include <algorithm>
#include <cstddef>
#include <span>

/**
 * then we can use it like this:
 * std::array<int, 5> arr = {10, 20, 30, 40, 50};
 * std::span<int> sp = arr;
 * auto view1 = slide(sp, 1, 3); starts at 1 and jumps 3 times, returning {20,
 * 30, 40}; auto view2 = slide(sp, 3, 3); starts at 3 and jumps 3 times,
 * returning {};
 */
template <class T, size_t N>
[[nodiscard]]
constexpr auto slide(std::span<T, N> s, size_t offset, size_t width) {
  return s.subspan(offset, offset + width <= s.size() ? width : 0U);
}

template <class T, size_t N, size_t M>
constexpr bool starts_with(std::span<T, N> data, std::span<T, M> prefix) {
  return data.size() >= prefix.size() &&
         std::equal(prefix.begin(), prefix.end(), data.begin());
}

template <class T, size_t N, size_t M>
constexpr bool ends_with(std::span<T, N> data, std::span<T, M> suffix) {
  return data.size() >= suffix.size() &&
         std::equal(data.end() - suffix.size(), data.end(),
                    suffix.end() - suffix.size());
}

template <class T, size_t N, size_t M>
constexpr bool contains(std::span<T, N> span, std::span<T, M> sub) {
  return std::ranges::search(span, sub).begin() != span.end();
}

#endif