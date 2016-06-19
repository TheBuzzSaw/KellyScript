#ifndef SYNTAXTREE_HPP
#define SYNTAXTREE_HPP

#include "Token.hpp"

namespace Kelly
{
    struct Package
    {
        View<const char> name;
        Package* parent;
    };

    struct AbstractSyntaxTree
    {
        Region nodeMemory;
        Region stringMemory;
        std::vector<Package> packages;
    };

    AbstractSyntaxTree Eat(const TreeFood& food);
}

#endif
