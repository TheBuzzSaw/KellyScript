#include "SyntaxTree.hpp"
#include <iostream>
using namespace std;

namespace Kelly
{
    SyntaxTree::SyntaxTree(View<char*> arguments)
    {
        // Process all flags first.

        for (const auto i : arguments)
        {
            if (i[0] == '-') cout << "flag " << i << '\n';
        }

        // Now compile files.

        for (const auto i : arguments)
        {
            if (i[0] != '-')
            {
                cout << "file " << i << '\n';

            }
        }

        Add(TypeDefinition("Int8", 1));
        Add(TypeDefinition("Int16", 2));
        Add(TypeDefinition("Int32", 4));
        Add(TypeDefinition("Int64", 8));
    }

    SyntaxTree::SyntaxTree(SyntaxTree&& other)
        : _typeDefinitions(std::move(other._typeDefinitions))
        , _typeDefinitionIndexByName(
            std::move(other._typeDefinitionIndexByName))
    {

    }

    SyntaxTree::~SyntaxTree()
    {
    }

    void SyntaxTree::Add(TypeDefinition td)
    {
        auto i = _typeDefinitionIndexByName.find(td.Name());

        if (i == _typeDefinitionIndexByName.end())
        {
            _typeDefinitionIndexByName[td.Name()] = _typeDefinitions.size();
            _typeDefinitions.push_back(std::move(td));
        }
    }
}
