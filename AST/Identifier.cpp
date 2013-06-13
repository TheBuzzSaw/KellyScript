#include "Identifier.hpp"

namespace Kelly
{
    Identifier::Identifier(std::string name, TypeHandle type,
        Identifier* package)
        : _name(std::move(name)), _type(type), _package(package)
    {
    }

    Identifier::Identifier(const Identifier& other)
        : _name(other._name), _type(other._type), _package(other._package)
    {
    }

    Identifier::~Identifier()
    {
    }
}
