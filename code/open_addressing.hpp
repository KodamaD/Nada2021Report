#include "template.hpp"

template <class value_t> class OpenAddressing {

    enum class State : char { 
        Available, Deleted, Null
    };

    std::size_t available, deleted, logn;
    std::vector<State> state;
    std::vector<key_t> keys;
    std::vector<value_t> values;

    std::size_t find_index(const key_t key) {
        std::size_t index = hash(key, logn);
        while (state[index] != State::Null) {
            if (keys[index] == key and state[index] == State::Available) {
                return index;
            }
            index += 1;
            if (index == state.size()) {
                index = 0;
            }
        }
        return index;
    }

    void extend() {
        std::vector<key_t> key;
        std::vector<value_t> value;
        key.reserve(available);
        value.reserve(available);
        for (std::size_t i = 0; i < state.size(); ++i) {
            if (state[i] == State::Available) {
                key.push_back(keys[i]);
                value.push_back(std::move(values[i]));
            }
        }
        const std::size_t len = available;
        available = 0;
        deleted = 0;
        while (logn > 0 and 3 * len <= ((std::size_t)1 << logn)) {
            logn -= 1;
        }
        logn += 1;
        state = std::vector<State>((std::size_t)1 << logn, State::Null);
        keys = std::vector<key_t>((std::size_t)1 << logn);
        values = std::vector<value_t>((std::size_t)1 << logn);
        for (std::size_t i = 0; i < len; ++i) {
            insert(key[i], std::move(value[i]));
        }
    }

  public:

    OpenAddressing(): 
        available(0), deleted(0), logn(0), state(1, State::Null), keys(1), values(1) {}

    // 要素を挿入する。すでに存在した場合は書き換える
    void insert(const key_t key, value_t&& value) {
        const std::size_t index = find_index(key);
        keys[index] = key;
        values[index] = std::move(value);
        if (state[index] == State::Null) {
            available += 1;
            state[index] = State::Available;
            if (3 * (available + deleted) > 2 * state.size()) {
                extend();
            }
        }
    }

    // 要素が存在するならば削除する
    void erase(const key_t key) {
        const std::size_t index = find_index(key);
        if (state[index] == State::Available) {
            available -= 1;
            deleted += 1;
            state[index] = State::Deleted;
        }
    }

    // key に対応する value のポインタを返す。存在しないならば nullptr を返す
    value_t* find(const key_t key) {
        const std::size_t index = find_index(key);
        if (state[index] == State::Available) {
            return &values[index];
        } else {
            return nullptr;
        }
    }

};
