#pragma once
#include "processing.h"
#include "flouw.h"
#include <optional>

class DropNulloptAdapter {
public:
	template<typename T>
	auto operator()(flouw<std::optional<T>>&& flow) const {
		flouw<T> result;
		for (auto&& element : flow) {
			if (element.has_value()) {
				result.push_back(std::move(*element));
			}
		}
		return result;
	}
};


inline auto DropNullopt() {
	return DropNulloptAdapter();
}



