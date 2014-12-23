#include "DynamicStack.hpp"
#include <cstdlib>

namespace Kelly
{
    struct PageHeader
    {
        PageHeader* nextPage;
        uint8_t* freeBlock;
        size_t remainingBytes;
    };

    DynamicStack::DynamicStack(size_t pageSize)
        : _pageSize(pageSize)
        , _firstPage(nullptr)
    {
    }

    DynamicStack::~DynamicStack()
    {
    }

    void* DynamicStack::Allocate(size_t byteCount)
    {
        void* result = nullptr;
        PageHeader* header = reinterpret_cast<PageHeader*>(_firstPage);

        while (header && header->remainingBytes < byteCount)
            header = header->nextPage;

        if (header)
        {
            result = header->freeBlock;
            header->freeBlock += byteCount;
            header->remainingBytes -= byteCount;
        }
        else
        {
            header = reinterpret_cast<PageHeader*>(malloc(_pageSize));
            header->nextPage = reinterpret_cast<PageHeader*>(_firstPage);
            header->freeBlock = reinterpret_cast<uint8_t*>(header + 1);
            header->remainingBytes = _pageSize - sizeof(PageHeader);
            _firstPage = header;
        }

        return result;
    }

    void DynamicStack::ReleaseAll()
    {
        PageHeader* header = reinterpret_cast<PageHeader*>(_firstPage);

        while (header)
        {
            header->freeBlock = reinterpret_cast<uint8_t*>(header + 1);
            header->remainingBytes = _pageSize - sizeof(PageHeader);
            header = header->nextPage;
        }
    }
}
