#include "SyntaxTree.hpp"
#include "Tools.hpp"
#include <iostream>
using namespace std;

namespace Kelly
{
    AbstractSyntaxTree Eat(const TreeFood& food)
    {
        AbstractSyntaxTree result;
        auto source = food.source.data();

        for (size_t i = 0; i < food.tokens.size(); ++i)
        {
            auto token = food.tokens[i];
            View<const char> v = {source + token.start, token.length};

            if (v == "import")
            {
                cout << "found an import!\n";
            }
            else if (v == "export")
            {
                cout << "found an export!\n";

                token = food.tokens[++i];

                if (token.type != Token::Identifier)
                {
                    v = {source + token.start, token.length};

                    cout << food.path << ':' << token.row << ':'
                        << token.column << ':'
                        << " expected package identifier; found "
                        << TokenTypeString(token.type)
                        << ' ' << v << '\n';
                }
            }
            else
            {
                //cout << "error: expected import or export";
            }
        }

        return result;
    }
}
