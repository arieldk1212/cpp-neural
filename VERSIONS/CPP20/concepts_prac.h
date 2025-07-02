#ifndef CONCEPTS_PRAC_H
#define CONCEPTS_PRAC_H

/**
 * @brief concepts that can be used to perform compile-time validation of
 * template arguments and perform function dispatch based on properties of
 * types. provide equational reasoning in programs.
 * has syntactic and semantic requirements, in general only the syntactic
 * requirements can be checked by the compiler.
 * if a concept is satisfied but not modeled or if a semantic requirement is not
 * met at the point of use, the program is ill-formed (behavior is undefined).
 */

/* EXAMPLES */

#include <concepts>

// std::same_as

namespace same_as {

namespace detail {
template <class T, class U>
concept SameHelper = std::is_same_v<T, U>;
}

template <class T, class U>
concept same_as = detail::SameHelper<T, U> && detail::SameHelper<U, T>;

} // namespace same_as

#endif