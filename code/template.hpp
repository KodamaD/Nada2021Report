#ifndef H_TEMPLATE
#define H_TEMPLATE 1

#include <cstdint>
#include <vector>
#include <utility>
#include <memory>

// key とする型
using key_t = std::uint64_t;

// ハッシュ関数（二つ目のステップ）
std::size_t hash(key_t key, const std::size_t d) {
    if (d == 0) {
        return 0;
    } else {
        key ^= key >> (64 - d);
        return (key * 11400714819323198485ull) >> (64 - d);
    }
}

#endif