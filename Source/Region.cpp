#include "Region.hpp"
#include <iostream>
#include <cstring>
#include <cassert>

struct PageHeader
{
    PageHeader* next;
    char* freeBlock;
    int freeByteCount;
    int allocationCount;
};

void Region::Release() noexcept
{
    auto header = (PageHeader*)_block;

    while (header)
    {
        auto nextHeader = header->next;
        free(header);
        header = nextHeader;
    }
}

Region::Region() noexcept : _block(nullptr)
{
}

Region::Region(Region&& other) noexcept : _block(other._block)
{
    other._block = nullptr;
}

Region::~Region() noexcept
{
    Release();
}

Region& Region::operator=(Region&& other) noexcept
{
    if (this != &other)
    {
        Release();
        _block = other._block;
        other._block = nullptr;
    }

    return *this;
}

void* Region::Allocate(int byteCount)
{
    assert(byteCount > 0);

    auto header = (PageHeader*)_block;

    while (header && header->freeByteCount < byteCount)
        header = header->next;

    if (!header)
    {
        constexpr int PageSize = 4 << 10;
        constexpr int FreePageSize = PageSize - sizeof(PageHeader);
        assert(FreePageSize > byteCount);
        header = (PageHeader*)malloc(PageSize);
        header->next = (PageHeader*)_block;
        header->freeBlock = (char*)(header + 1);
        header->freeByteCount = FreePageSize;
        header->allocationCount = 0;

        _block = header;
    }

    void* result = header->freeBlock;
    header->freeBlock += byteCount;
    header->freeByteCount -= byteCount;
    ++header->allocationCount;
    return result;
}

void* Region::AllocateAndWrite(int byteCount, const void* data)
{
    auto block = Allocate(byteCount);
    memcpy(block, data, byteCount);
    return block;
}

View<char> Region::AllocateAndWriteString(int byteCount, const char* data)
{
    auto block = AllocateAndWrite(byteCount, data);
    return {static_cast<char*>(block), byteCount};
}

int Region::PageCount() const noexcept
{
    int result = 0;

    for (auto header = (PageHeader*)_block; header; header = header->next)
        ++result;

    return result;
}

void Region::DebugDump() const
{
    auto header = (PageHeader*)_block;

    std::cout << "--- REGION " << (void*)this << " ---";

    while (header)
    {
        std::cout << '\n' << header->allocationCount << " allocation";
        if (header->allocationCount != 1) std::cout << 's';

        std::cout << ", " << header->freeByteCount << " byte";
        if (header->freeByteCount != 1) std::cout << 's';
        std::cout << " free";

        header = header->next;
    }

    std::cout << std::endl;
}

void* operator new(size_t byteCount, Region& region)
{
    return region.Allocate(byteCount);
}

void* operator new[](size_t byteCount, Region& region)
{
    return region.Allocate(byteCount);
}
