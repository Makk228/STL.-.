#include "processing.h"
#include "flouw.h"

template<typename Func>
struct TransformAdapter {
    Func func;

    explicit TransformAdapter(Func f) : func(std::move(f)) {}

    template<typename T>
    auto operator()(flouw<T>&& input) const {
        using ResultType = decltype(func(std::declval<T>()));
        flouw<ResultType> result;

        for (auto&& item : input.get_data()) {
            result.push_back(func(std::forward<T>(item)));
        }

        return result;
    }
};

template<typename Func>
TransformAdapter<Func> Transform(Func&& func) {
    return TransformAdapter<Func>{std::forward<Func>(func)};
}
