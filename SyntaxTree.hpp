#ifndef SYNTAXTREE_HPP
#define SYNTAXTREE_HPP

#include "Token.hpp"

namespace Kelly
{
    struct Package
    {
        View<const char> name;
        int32_t parentIndex;
    };

    struct AbstractSyntaxTree
    {
        Region nodeMemory;
        Region stringMemory;
        std::vector<Package> packages;

        Value<Package> FindPackage(Package package);
    };

    AbstractSyntaxTree Eat(const TreeFood& food);
}

#endif
