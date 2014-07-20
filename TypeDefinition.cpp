#include "TypeDefinition.hpp"

namespace Kelly
{
    TypeDefinition::TypeDefinition(std::string name, int byteCount)
        : _name(std::move(name))
        , _byteCount(byteCount)
    {
    }

    TypeDefinition::TypeDefinition(TypeDefinition&& other)
        : _name(std::move(other._name))
        , _byteCount(other._byteCount)
    {
        other._byteCount = 0;
    }

    TypeDefinition::~TypeDefinition()
    {
    }

    TypeDefinition& TypeDefinition::operator=(TypeDefinition&& other)
    {
        if (this != &other)
        {
            _name = std::move(other._name);
            _byteCount = other._byteCount;

            other._byteCount = 0;
        }

        return *this;
    }
}
