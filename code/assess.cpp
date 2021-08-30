
#include "open_addressing.hpp"
#include "separate_chaining.hpp"

#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include <cassert>
#include <unordered_set>
#include <unordered_map>
#include <map>

// 乱数生成
std::uint64_t random_number() {
    static std::uint64_t state = 828797960;
    state ^= state << 7;
    state ^= state >> 9;
    return state;
}

// 実行時間計測
template <class F> std::int64_t measure(const F& f) {
    using namespace std::chrono;
    const auto begin = system_clock::now();
    f();
    const auto end = system_clock::now();
    return duration_cast<milliseconds>(end - begin).count();
}

int main() {
    for (const std::size_t n : {100000, 1000000, 10000000}) {
        std::map<std::string, std::int64_t> elapsed;
        
        for (int step = 0; step < 5; ++step) {
            std::unordered_set<key_t> for_insert, for_find;
            while (for_insert.size() < n) {
                for_insert.insert(random_number());
            }
            while (for_find.size() < n) {
                const key_t key = random_number();
                if (for_insert.find(key) == for_insert.end()) {
                    for_find.insert(key);
                }
            }

            std::unordered_map<key_t, std::uint64_t> umap;
            elapsed["std::unordered_map insert"] += measure([&] { for (const auto x : for_insert) umap.emplace(x, random_number()); });
            elapsed["std::unordered_map find1"] += measure([&] { for (const auto x : for_insert) assert(umap.find(x) != umap.end()); });
            elapsed["std::unordered_map find2"] += measure([&] { for (const auto x : for_find) assert(umap.find(x) == umap.end()); });
            elapsed["std::unordered_map erase"] += measure([&] { for (const auto x : for_insert) umap.erase(x); });
            SeparateChaining<std::uint64_t> smap;
            elapsed["Separate Chaining insert"] += measure([&] { for (const auto x : for_insert) smap.insert(x, random_number()); });
            elapsed["Separate Chaining find1"] += measure([&] { for (const auto x : for_insert) assert(smap.find(x)); });
            elapsed["Separate Chaining find2"] += measure([&] { for (const auto x : for_find) assert(!smap.find(x)); });
            elapsed["Separate Chaining erase"] += measure([&] { for (const auto x : for_insert) smap.erase(x); });
            OpenAddressing<std::uint64_t> omap;
            elapsed["Open Addressing insert"] += measure([&] { for (const auto x : for_insert) omap.insert(x, random_number()); });
            elapsed["Open Addressing find1"] += measure([&] { for (const auto x : for_insert) assert(omap.find(x)); });
            elapsed["Open Addressing find2"] += measure([&] { for (const auto x : for_find) assert(!omap.find(x)); });
            elapsed["Open Addressing erase"] += measure([&] { for (const auto x : for_insert) omap.erase(x); });
        }

        std::cout << "n = " << n << std::endl;
        for (const auto& [s, c] : elapsed) {
            std::cout << s << ": " << c / 5.0 << "ms" << std::endl;
        }
    }
    return 0;
}