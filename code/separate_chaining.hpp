#include "template.hpp"

template <class value_t> class SeparateChaining {

    struct Node;
    using Ptr = std::unique_ptr<Node>;

    struct Node {
        key_t key;
        value_t value;
        Ptr next;
        Node() = default;
        explicit Node(const key_t key_, value_t&& value_):
            key(key_), value(std::move(value_)), next() {}
    };

    std::size_t available, logn;
    std::vector<Ptr> chains;

    Ptr* find_ptr(const key_t key) {
        Ptr* ptr = &chains[hash(key, logn)];
        while (*ptr and (*ptr)->key != key) {
            ptr = &((*ptr)->next);
        }
        return ptr;
    }

    void extend() {
        std::vector<key_t> keys;
        std::vector<value_t> values;
        keys.reserve(available);
        values.reserve(available);
        for (Ptr& front : chains) {
            for (Ptr* ptr = &front; *ptr; ptr = &((*ptr)->next)) {
                keys.push_back((*ptr)->key);
                values.push_back(std::move((*ptr)->value));
            }
        }
        const std::size_t len = available;
        available = 0;
        logn += 1;
        chains = std::vector<Ptr>((std::size_t)1 << logn);
        for (std::size_t i = 0; i < len; ++i) {
            insert(keys[i], std::move(values[i]));
        }
    }

  public:

    SeparateChaining(): available(0), logn(0), chains(1) {}
    
    // 要素を挿入する。すでに存在した場合は書き換える
    void insert(const key_t key, value_t&& value) {
        if (Ptr* ptr = find_ptr(key); *ptr) {
            (*ptr)->value = std::move(value);
        } else {
            available += 1;
            *ptr = std::make_unique<Node>(key, std::move(value));
            if (available > chains.size()) {
                extend();
            }
        }
    }

    // 要素が存在するならば削除する
    void erase(const key_t key) {
        if (Ptr* ptr = find_ptr(key); *ptr) {
            *ptr = std::move((*ptr)->next);
        }
    }

    // key に対応する value のポインタを返す。存在しないならば nullptr を返す
    value_t* find(const key_t key) {
        if (Ptr* ptr = find_ptr(key); *ptr) {
            return &((*ptr)->value);
        } else {
            return nullptr;
        }
    }

};
