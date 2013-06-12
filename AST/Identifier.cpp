#include "Identifier.hpp"

namespace Kelly
{
    Identifier::Identifier(std::string name) : _name(std::move(name))
    {
    }

    Identifier::~Identifier()
    {
    }
}
