#ifndef VIEW_HPP
#define VIEW_HPP

#include <iostream>
#include <cstring>
#include <cstddef>

namespace Kelly
{
    template<typename T> struct View
    {
        T* first;
        ptrdiff_t length;

        constexpr operator View<const T>() const { return { first, length }; }
    };

    template<typename T> constexpr T* begin(View<T> view)
    {
        return view.first;
    }

    template<typename T> constexpr T* end(View<T> view)
    {
        return view.first + view.length;
    }

    inline bool operator==(View<const char> a, View<const char> b)
    {
        return a.length == b.length &&
            (a.first == b.first || !memcmp(a.first, b.first, a.length));
    }

    inline bool operator!=(View<const char> a, View<const char> b)
    {
        return !(a == b);
    }

    inline bool operator==(View<const char> a, const char* b)
    {
        return !strncmp(a.first, b, a.length);
    }

    inline bool operator!=(View<const char> a, const char* b)
    {
        return strncmp(a.first, b, a.length);
    }

    inline std::ostream& operator<<(
        std::ostream& stream, View<const char> view)
    {
        return stream.write(view.first, view.length);
    }
}

#endif
