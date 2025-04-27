#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include "flouw.h"
#include <vector>
#include <utility>

#pragma once


template<typename Value, typename Accumulator, typename Func, typename KeySelector>
struct AggregateByKeyAdapter {
    Accumulator initial;
    Func func;
    KeySelector key_selector;

 
    AggregateByKeyAdapter(Accumulator initial, Func func, KeySelector key_selector)
        : initial(std::move(initial)), func(std::move(func)), key_selector(std::move(key_selector)) {}

    template<typename T>
    auto operator()(flouw<T>&& flow) const {
        using Key = decltype(key_selector(std::declval<T>()));  
        using Acc = std::decay_t<Accumulator>;                   

        std::unordered_map<Key, Acc> map;  
        std::vector<Key> key_order;         

      
        for (auto&& value : flow) {
            Key key = key_selector(value);  
            if (map.find(key) == map.end()) { 
                map[key] = initial;         
                key_order.push_back(key);    
            }
            func(value, map[key]);          
        }

   
        flouw<std::pair<Key, Acc>> result;
        for (const auto& key : key_order) {
            result.push_back({key, map[key]});  
        }

        return result;
    }
};


template<typename Accumulator, typename Func, typename KeySelector>
auto AggregateByKey(Accumulator initial, Func func, KeySelector key_selector) {
    return AggregateByKeyAdapter<Accumulator, Accumulator, Func, KeySelector>(
        std::move(initial), std::move(func), std::move(key_selector));
}



template<typename T, typename KeySelector,typename Func, typename Aggregator>
auto operator|(flouw<T>&& input, AggregateByKeyAdapter<T, KeySelector,Func, Aggregator>&& adapter) {
    return adapter(std::move(input));
}
