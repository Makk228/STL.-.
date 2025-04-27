	#pragma once

	#include <vector>
	#include "flouw.h"


	class AsVectorAdapter {
	
	public:
		template<typename T>
		std::vector<T> operator()(flouw<T>&& data) {
			
			std::vector<T> result;
			for (auto&& element : data) {
				result.push_back(std::move(element));
			}
			return result;
		}
	};


	inline auto AsVector() {
		return AsVectorAdapter();
	}

	template<typename T>
	auto operator|(flouw<T>&& input, AsVectorAdapter&& adapter) {
		return adapter(std::move(input));
	}


