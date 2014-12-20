#ifndef VIEW_HPP
#define VIEW_HPP

#include <iostream>

namespace Kelly
{
    template<typename T>
    struct View
    {
        T* first;
        std::size_t length;
    };

    template<typename T>
    constexpr const T* begin(const View<T>& view)
    {
        return view.first;
    }

    template<typename T>
    constexpr const T* end(const View<T>& view)
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
