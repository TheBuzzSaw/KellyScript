#include "SyntaxTree.hpp"
#include "Tools.hpp"
#include <iostream>
using namespace std;

namespace Kelly
{
    SyntaxTree::SyntaxTree(View<char*> arguments)
    {
        // Process all flags first.

        for (auto argument : arguments)
        {
            if (argument[0] == '-') cout << "flag " << argument << '\n';
        }

        // Now compile files.

        for (auto argument : arguments)
        {
            if (argument[0] != '-')
            {
                cout << "file " << argument << '\n';
                ReadFile(argument);
            }
        }

        Add(TypeDefinition("Int8", 1));
        Add(TypeDefinition("Int16", 2));
        Add(TypeDefinition("Int32", 4));
        Add(TypeDefinition("Int64", 8));
    }

    SyntaxTree::SyntaxTree(SyntaxTree&& other)
        : _typeDefinitions(move(other._typeDefinitions))
        , _typeDefinitionIndexByName(
            move(other._typeDefinitionIndexByName))
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
            _typeDefinitions.push_back(move(td));
        }
    }

    void SyntaxTree::ReadFile(const char* file)
    {
        auto content = FileToString(file);
        cout << "content of " << file << ":\n" << content.data() << endl;
    }
}
