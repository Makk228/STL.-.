#pragma once

#include <unordered_map>
#include <optional>
#include <vector>
#include <utility>
#include <type_traits>
#include "flouw.h"

template<typename K, typename V>
struct KV {
    K key;
    V value;

    bool operator==(const KV&) const = default;
};

template<typename L, typename R>
struct JoinResult {
    L left;
    std::optional<R> right;

    bool operator==(const JoinResult&) const = default;
};

template<typename L, typename R>
bool operator==(const JoinResult<L, R>& lhs, const JoinResult<L, R>& rhs) {
    return lhs.left == rhs.left && lhs.right == rhs.right;
}


template<typename RightFlow, typename LeftKeyFn, typename RightKeyFn>
class JoinAdapter {
    RightFlow right_;
    LeftKeyFn left_key_;
    RightKeyFn right_key_;

public:
    JoinAdapter(RightFlow right, LeftKeyFn left_key, RightKeyFn right_key)
        : right_(std::move(right)), left_key_(std::move(left_key)), right_key_(std::move(right_key)) {}

    template<typename LeftRange>
    auto operator()(LeftRange&& left_range) {
        using L = typename std::decay_t<LeftRange>::value_type;
        using R = typename decltype(right_())::value_type;
        using Key = decltype(left_key_(std::declval<L>()));

        std::unordered_multimap<Key, R> right_map;
        for (auto&& r : right_()) {
            right_map.emplace(right_key_(r), std::move(r));
        }

        std::vector<JoinResult<L, R>> result;
        for (auto&& l : left_range) {
            Key key = left_key_(l);
            auto [begin, end] = right_map.equal_range(key);
            if (begin == end) {
                result.emplace_back(JoinResult<L, R>{l, std::nullopt});
            } else {
                for (auto it = begin; it != end; ++it) {
                    result.emplace_back(JoinResult<L, R>{l, it->second});
                }
            }
        }

        return flouw(result);
    }
};


template<typename RightFlow>
auto Join(RightFlow right) {
    auto get_key = [](const auto& kv) { return kv.key; };
    return JoinAdapter<RightFlow, decltype(get_key), decltype(get_key)>(
        std::move(right), get_key, get_key);
}


template<typename RightFlow, typename LeftKeyFn, typename RightKeyFn>
auto Join(RightFlow right, LeftKeyFn left_key, RightKeyFn right_key) {
    return JoinAdapter<RightFlow, LeftKeyFn, RightKeyFn>(
        std::move(right), std::move(left_key), std::move(right_key));
}


template<typename LeftFlow, typename RightFlow, typename LeftKeyFn, typename RightKeyFn>
auto operator|(LeftFlow&& left, JoinAdapter<RightFlow, LeftKeyFn, RightKeyFn>&& adapter) {
    return adapter(std::forward<LeftFlow>(left));
}
