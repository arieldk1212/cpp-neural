#ifndef RANGES_PRAC_H
#define RANGES_PRAC_H

/**
 * @brief extension and generalization of the algorithms and iterator libraries
 * that makes them more powerful by making them composable and less error-prone.
 * the library creates and manipulates range "views", lightweight objects that
 * indirectly represent iterable sequences (ranges).
 * @brief the ranges library includes range algorithms, which are applied to
 * ranges eagerly, and range adaptors, which are applied to views lazily.
 * adaptors can be composed into pipelines. so that their actions take place as
 * the view is iterated.
 * @brief ranges algorithms -> deal with the data according to the algorithm.
 *        ranges adaptors -> dont deal with data, deals with how the data is
 *        represented in the view.
 * @param
 */

/* EXAMPLES */

#include <algorithm>
#include <cctype>
#include <ranges>
#include <type_traits>
#include <vector>

auto Even(const std::vector<int> &Data) -> std::vector<int> {
  auto even = Data | std::views::filter([](int n) { return n % 2 == 0; });
};

auto Upper(const std::vector<std::string> &Data) -> std::vector<int> {
  auto upper = Data;
  std::ranges::transform(upper, upper.begin(), [](std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return s;
  });
}

auto foreach (std::vector<int> &Data) {
  std::ranges::for_each(std::views::reverse(Data),
                        [](int &elem) { return elem; });
}

#endif