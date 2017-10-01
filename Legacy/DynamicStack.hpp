#ifndef DYNAMICSTACK_HPP
#define DYNAMICSTACK_HPP

#include "View.hpp"

namespace Kelly
{
    class DynamicStack
    {
        public:
            DynamicStack(size_t pageSize);
            DynamicStack(DynamicStack&& other);
            ~DynamicStack();

            DynamicStack& operator=(DynamicStack&& other);

            size_t PageSize() const { return _pageSize; }
            size_t PageCount() const;
            void Dump(std::ostream& stream) const;

            View<uint8_t> Allocate(size_t byteCount);
            void ReleaseAll();
            void FreePages();

        protected:
        private:
            DynamicStack(const DynamicStack&) = delete;
            DynamicStack& operator=(const DynamicStack&) = delete;

            size_t _pageSize;
            void* _firstPage;
    };
}

#endif
