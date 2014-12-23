#ifndef DYNAMICSTACK_HPP
#define DYNAMICSTACK_HPP

#include <cstdint>

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

            void* Allocate(size_t byteCount);
            void ReleaseAll();

        protected:
        private:
            DynamicStack(const DynamicStack&) = delete;
            DynamicStack& operator=(const DynamicStack&) = delete;

            size_t _pageSize;
            void* _firstPage;
    };
}

#endif
