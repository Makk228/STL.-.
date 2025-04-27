#pragma once
#include <ostream>
#include <sstream>
#include <iostream>
#include <string>
#include "flouw.h"

template<typename Stream, typename T>
struct WriteAdapter {
    Stream& out; 
    char delimiter;


    WriteAdapter(Stream& out_, char delim) : out(out_), delimiter(delim) {}


    flouw<T> operator()(flouw<T>&& input) const {
        for (const auto& item : input.get_data()) {
            std::stringstream ss;
            ss << item;  
            out << ss.str() << delimiter;  
        }
        return std::move(input);  
    }
};



template<typename Stream>
auto Write(Stream& out, char delimiter) {
    return [&,delimiter = delimiter](auto&& input) {
        using T = typename std::remove_reference<decltype(input)>::type::value_type;

        auto it = input.begin();
        auto end = input.end();

        if (it != end) {
            out << *it; 
            ++it;
        }

        for (; it != end; ++it) {
            out << delimiter << *it; 
        }

        out << delimiter; 

        return std::forward<decltype(input)>(input);
    };
}
