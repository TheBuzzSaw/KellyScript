#include "Identifier.hpp"

namespace Kelly
{
    Identifier::Identifier(std::string name, Type type, Identifier* parent)
        : _name(std::move(name)), _type((int)type),
        _parent(parent)
    {
        if (_parent) _parent->_childrenByName[_name] = this;
    }

    Identifier::~Identifier()
    {
        for (auto i = _childrenByName.begin();
            i != _childrenByName.end(); ++i)
        {
            delete i->second;
        }
    }

    Identifier* Identifier::Child(const std::string& name) const
    {
        auto i = _childrenByName.find(name);
        return i == _childrenByName.end() ? 0 : i->second;
    }
}
