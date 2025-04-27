#pragma once
#include "processing.h"

template<typename Predicate>
class FilterAdapter {
	Predicate pred;
public:
	explicit FilterAdapter(Predicate p) : pred(std::move(p)) {}

	template<typename T>
    auto operator()(flouw<T> input) const {
        flouw<T> result;
        for (const auto& item : input.get_data()) {
            if (pred(item)) {
                result.push_back(item);
            }
        }
        return result;
    }
};

template<typename Predicate>
auto Filter(Predicate p) {
    return FilterAdapter<Predicate>(std::move(p));
}