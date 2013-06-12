#ifndef DATATYPE_HPP
#define DATATYPE_HPP

#include "Identifier.hpp"

namespace Kelly
{
    class DataType : public Identifier
    {
        public:
            DataType(std::string name, size_t byteCount);
            virtual ~DataType();

            inline size_t ByteCount() const { return _byteCount; }

        protected:
        private:
            size_t _byteCount;
    };
}

#endif
