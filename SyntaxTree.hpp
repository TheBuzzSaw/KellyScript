#ifndef SYNTAXTREE_HPP
#define SYNTAXTREE_HPP

#include "View.hpp"
#include "TypeDefinition.hpp"
#include "Token.hpp"
#include <vector>
#include <unordered_map>

namespace Kelly
{
    class SyntaxTree
    {
        public:
            SyntaxTree(View<char*> arguments);
            SyntaxTree(SyntaxTree&& other);
            SyntaxTree(const SyntaxTree&) = delete;
            ~SyntaxTree();

            SyntaxTree& operator=(SyntaxTree&& other);
            SyntaxTree& operator=(const SyntaxTree&) = delete;

        protected:
        private:
            void Add(TypeDefinition td);
            void ReadFile(const char* file);

            std::vector<Token> _tokens;
            std::vector<TypeDefinition> _typeDefinitions;
            std::unordered_map<std::string,
                decltype(_typeDefinitions)::size_type>
                    _typeDefinitionIndexByName;
    };
}

#endif
