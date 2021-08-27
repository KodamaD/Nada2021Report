
#include "open_addressing.hpp"
#include "separate_chaining.hpp"

#include <iostream>
#include <random>
#include <cassert>

// 乱数を生成するためのシード値
constexpr std::uint64_t SEED = 828797960; 

// [0, 2^64) から値を生成する乱数生成器
std::mt19937_64 ENGINE(SEED); 

// [low, high] に属する整数値をランダムに生成する
template <class T> random_number(const T low, const T high) {
    return std::uniform_int_distribution<T>(low, high)(gen);
}

int main() {
    
    return 0;
}