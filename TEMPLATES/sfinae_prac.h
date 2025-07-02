#ifndef SFINAE_H
#define SFINAE_H

/**
 * @brief the compiler will not generate an error while you try to substitude
 * the template types with the types i used in my code. meaning the compiler
 * will try next alternative useful when we want to select a different
 * implementation based on characteristic or trait of a type.
 * dont use sfinae unless i really need i.
 */

/**
 * @brief SFINAE ALTERNATIVE - Tag Dispatching
 */

#include <iostream>
#include <type_traits>

template <typename T> auto get_vaule_impl(T t, std::true_type) { return t; }

template <typename T> auto get_vaule_impl(T t, std::false_type) {
  std::cout << "not arithmetic!";
}

template <typename T> auto get_value(T t) {
  return get_vaule_impl(t, std::is_arithmetic<T>());
}

/**
 * @brief SFINAE ALTERNATIVE - Concepts
 */

template <typename T>
  requires std::is_floating_point_v<T>
bool are_equal(T a, T b) {
  return std::abs(a - b) < 0.0001;
}

template <typename T> bool are_equal(T a, T b) { return a == b; }
/**
 * @brief SFINAE ALTERNATIVE IN GENERAL - overloads, template specialization, if
 * constexpr
 */


/**
 * @brief SFINAE EXAMPLES
 */



#endif