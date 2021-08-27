#ifndef H_TEMPLATE
#define H_TEMPLATE 1

#include <vector>
#include <algorithm>
#include <utility>
#include <list>
#include <cstdint>
#include <random>

using key_t = std::uint64_t;

template <class T> using Vec = std::vector<T>;

constexpr std::uint64_t SEED = 828797960; // 乱数を生成するためのシード値
std::mt19937_64 ENGINE(SEED); // [0, 2^64) から値を生成する乱数生成器

#endif