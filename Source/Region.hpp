#ifndef REGION_HPP_KELLY
#define REGION_HPP_KELLY

#include "View.hpp"
#include <cstdlib>

class Region
{
    void* _block;

    void Release() noexcept;

public:
    Region() noexcept;
    Region(Region&& other) noexcept;
    Region(const Region&) = delete;
    ~Region() noexcept;

    Region& operator=(Region&& other) noexcept;
    Region& operator=(const Region&) = delete;

    void* Allocate(int byteCount);
    void* AllocateAndWrite(int byteCount, const void* data);
    View<char> AllocateAndWriteString(int byteCount, const char* data);
    int PageCount() const noexcept;
    void DebugDump() const;
};

void* operator new(size_t byteCount, Region& region);
void* operator new[](size_t byteCount, Region& region);

#endif
