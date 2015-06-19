#ifndef VIEW_HPP
#define VIEW_HPP

#include <iostream>
#include <cstring>

namespace Kelly
{
    template<typename T>
    struct View
    {
        T* first;
        size_t length;

        constexpr operator View<const T>() const { return { first, length }; }
    };

    template<typename T>
    bool operator==(const View<T>& a, const View<T>& b)
    {
        return
            a.length == b.length &&
            (a.first == b.first ||
            !memcmp(a.first, b.first, a.length * sizeof(T)));
    }

    template<typename T>
    bool operator<(const View<T>& a, const View<T>& b)
    {
        return
            a.length < b.length ||
            (a.length == b.length &&
            memcmp(a.first, b.first, a.length * sizeof(T)) < 0);
    }

    template<typename T>
    bool operator>(const View<T>& a, const View<T>& b)
    {
        return
            a.length > b.length ||
            (a.length == b.length &&
            memcmp(a.first, b.first, a.length * sizeof(T)) > 0);
    }

    template<typename T>
    constexpr T* begin(const View<T>& view)
    {
        return view.first;
    }

    template<typename T>
    constexpr T* end(const View<T>& view)
    {
        return view.first + view.length;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const View<T>& view)
    {
        for (auto i : view) stream << i;

        return stream;
    }
}

#endif
