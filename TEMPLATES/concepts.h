#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <__config>
#include <concepts>
#include <type_traits>

template <typename T>
void is_odd(T x) {
}

template <typename T>
requires std::equality_comparable<T> && std::is_default_constructible<T>
bool isdefaultvalue(const T &x) {
  return x == T();
}

concept Street = requires (const T &street) {
  street.somefunction("location")
} -> std::same_as<bool>

#endif