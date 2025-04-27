#pragma once

#include <vector>
#include <iterator>
#include "processing.h"
#include "flouw.h"  


template<typename Container>
class AsDataFlowAdapter {
	public:
	
    using value_type  = typename std::decay_t<Container>::value_type;
	
    explicit AsDataFlowAdapter(Container&& container) {
		data_.insert(data_.end(),
		std::make_move_iterator(std::begin(container)),
		std::make_move_iterator(std::end(container)));
    }
	
	auto begin() const { return data_.begin(); }
    auto end() const { return data_.end(); }
	
    flouw<value_type> operator()() {
		return flouw<value_type>(std::move(data_));
    }
	private:
	
	std::vector<value_type> data_;
};


template<typename Container>
auto AsDataFlow(Container&& container) {
    return AsDataFlowAdapter<Container>(std::forward<Container>(container));
}


template<typename Container, typename Func>
auto operator|(AsDataFlowAdapter<Container>&& adapter, Func&& next) {
    return std::forward<Func>(next)(std::move(adapter)());
}


