#ifndef TinyStringHpp
#define TinyStringHpp

#include <cstring>
#include <iostream>
#include <cstdint>

template<typename T> union TinyString
{
    T value;
    char bytes[sizeof(T)];
    
    inline void Assign(const char* text)
    {
        if (text)
            strncpy(bytes, text, sizeof(bytes));
        else
            value = 0;
    }
    
    inline bool Append(char c)
    {
        for (size_t i = 0; i < sizeof(bytes); ++i)
        {
            if (!bytes[i])
            {
                bytes[i] = c;
                return true;
            }
        }
        
        return false;
    }
};

template<typename T>
constexpr bool operator==(TinyString<T> a, TinyString<T> b)
{
    return a.value == b.value;
}

template<typename T>
constexpr bool operator!=(TinyString<T> a, TinyString<T> b)
{
    return a.value != b.value;
}

template<typename T>
constexpr bool operator<(TinyString<T> a, TinyString<T> b)
{
    return a.value < b.value;
}

template<typename T>
constexpr bool operator<=(TinyString<T> a, TinyString<T> b)
{
    return a.value <= b.value;
}

template<typename T>
constexpr bool operator>(TinyString<T> a, TinyString<T> b)
{
    return a.value > b.value;
}

template<typename T>
constexpr bool operator>=(TinyString<T> a, TinyString<T> b)
{
    return a.value >= b.value;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, TinyString<T> ts)
{
    if (ts.bytes[sizeof(T) - 1])
        stream.write(ts.bytes, sizeof(T));
    else
        stream << ts.bytes;

    return stream;
}

using String4 = TinyString<int32_t>;
using String8 = TinyString<int64_t>;

#endif

