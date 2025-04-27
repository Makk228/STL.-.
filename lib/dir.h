#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <stdexcept>
#include "processing.h" 
#include "flouw.h"
class DirAdapter {
    std::filesystem::path path_;
    bool recursive_;

public:
    DirAdapter(const std::filesystem::path& path, bool recursive = false)
        : path_(path), recursive_(recursive) {}

    flouw<std::filesystem::path> operator()() const {
        std::vector<std::filesystem::path> files;

        if (!std::filesystem::exists(path_)) {
            throw std::runtime_error("Directory does not exist");
        }

        if (recursive_) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(path_)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path());
                }
            }
        } else {
            for (const auto& entry : std::filesystem::directory_iterator(path_)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path());
                }
            }
        }

        return flouw<std::filesystem::path>(std::move(files));
    }
};


inline auto Dir(const std::filesystem::path& path, bool recursive = false) {
    return DirAdapter(path, recursive);
}


template<typename Func>
auto operator|(DirAdapter adapter, Func&& next) {
    return std::forward<Func>(next)(adapter());
}

