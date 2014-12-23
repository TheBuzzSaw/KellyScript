#include "DynamicStack.hpp"
#include <cstdlib>
#include <cassert>

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
        assert(pageSize > sizeof(PageHeader));
    }

    DynamicStack::DynamicStack(DynamicStack&& other)
        : _pageSize(other._pageSize)
        , _firstPage(other._firstPage)
    {
        other._pageSize = 0;
        other._firstPage = nullptr;
    }

    DynamicStack::~DynamicStack()
    {
        while (_firstPage)
        {
            void* dead = _firstPage;
            _firstPage = reinterpret_cast<PageHeader*>(_firstPage)->nextPage;
            free(dead);
        }
    }

    View<uint8_t> DynamicStack::Allocate(size_t byteCount)
    {
        PageHeader* header = reinterpret_cast<PageHeader*>(_firstPage);

        while (header && header->remainingBytes < byteCount)
            header = header->nextPage;

        if (!header)
        {
            header = reinterpret_cast<PageHeader*>(malloc(_pageSize));
            assert(header);
            header->nextPage = reinterpret_cast<PageHeader*>(_firstPage);
            header->freeBlock = reinterpret_cast<uint8_t*>(header + 1);
            header->remainingBytes = _pageSize - sizeof(PageHeader);
            _firstPage = header;
        }

        assert(header->remainingBytes >= byteCount);

        View<uint8_t> result;
        result.first = header->freeBlock;
        result.length = byteCount;
        header->freeBlock += byteCount;
        header->remainingBytes -= byteCount;

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

    size_t DynamicStack::PageCount() const
    {
        size_t result = 0;

        PageHeader* header = reinterpret_cast<PageHeader*>(_firstPage);

        while (header)
        {
            ++result;
            header = header->nextPage;
        }

        return result;
    }

    void DynamicStack::Dump(std::ostream& stream) const
    {
        PageHeader* header = reinterpret_cast<PageHeader*>(_firstPage);

        while (header)
        {
            View<const char> content;
            content.first = reinterpret_cast<const char*>(header + 1);
            content.length =
                _pageSize -
                sizeof(PageHeader) -
                header->remainingBytes;

            stream
                << "page "
                << (void*)header
                << " ("
                << content.length
                << " bytes used)\n"
                << content
                << '\n';

            header = header->nextPage;
        }
    }
}
