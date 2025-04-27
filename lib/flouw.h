#pragma once
#include<vector>
#include <type_traits>

template<typename T>
class flouw {
    std::vector<T> data_;

public:
    using value_type = T;

    flouw() = default;
	
    explicit flouw(std::vector<T> data) : data_(std::move(data)) {}

    auto begin() const { return data_.begin(); }
    auto end() const { return data_.end(); }

    const std::vector<T>& get_data() const { return data_; }
    std::vector<T>& get_data() { return data_; }

    void push_back(const T& value) { data_.push_back(value); }
    void push_back(T&& value) { data_.push_back(std::move(value)); }
};


template<typename T, typename Func>
auto operator|(flouw<T> input, Func&& adapter) {
    return std::forward<Func>(adapter)(std::move(input));
}


template<typename Lhs, typename Rhs,
         typename = std::enable_if_t<std::is_invocable_v<Rhs, Lhs>>>
auto operator|(Lhs&& lhs, Rhs&& rhs) {
    return rhs(std::forward<Lhs>(lhs));
}
