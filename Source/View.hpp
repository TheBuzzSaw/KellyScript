#ifndef ViewHpp
#define ViewHpp

template<class T> struct View
{
    T* data;
    int count;
    
    inline T* begin() const { return data; }
    inline T* end() const { return data + count; }
    
    operator View<const T>() { return {data, count}; }
};

#endif
