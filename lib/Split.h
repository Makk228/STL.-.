#include "flouw.h"
#include "processing.h"
#include <sstream>
#include <string>

struct SplitAdapter {
    std::string delimiter;

    explicit SplitAdapter(std::string delim) : delimiter(std::move(delim)) {}

    flouw<std::string> operator()(flouw<std::stringstream>&& input) const {
        flouw<std::string> result;

        for (auto& ss : input.get_data()) {
            std::string line = ss.str();  // извлекаем строку
            size_t pos = 0;
            std::string temp = line;

            while ((pos = temp.find(delimiter)) != std::string::npos) {
                result.push_back(temp.substr(0, pos));
                temp.erase(0, pos + delimiter.length());
            }

            result.push_back(temp);
        }

        return result;
    }
};


inline SplitAdapter Split(const std::string& delim) {
    return SplitAdapter(delim);
}
