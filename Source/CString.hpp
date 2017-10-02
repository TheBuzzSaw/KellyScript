#ifndef CStringHpp
#define CStringHpp

#include <cstring>
#include <iostream>

struct CString
{
    const char* text;
};

inline bool operator==(CString a, CString b)
{
    return !strcmp(a.text, b.text);
}

inline bool operator!=(CString a, CString b)
{
    return strcmp(a.text, b.text);
}

inline bool operator<(CString a, CString b)
{
    return strcmp(a.text, b.text) < 0;
}

inline bool operator<=(CString a, CString b)
{
    return strcmp(a.text, b.text) <= 0;
}

inline bool operator>(CString a, CString b)
{
    return strcmp(a.text, b.text) > 0;
}

inline bool operator>=(CString a, CString b)
{
    return strcmp(a.text, b.text) >= 0;
}

inline std::ostream& operator<<(std::ostream& stream, CString cs)
{
    return stream << cs.text;
}

#endif

