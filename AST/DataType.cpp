#include "DataType.hpp"

namespace Kelly
{
    DataType::DataType(std::string name, size_t byteCount)
        : Identifier(std::move(name))
    {
        _byteCount = byteCount;
    }

    DataType::~DataType()
    {
    }
}
