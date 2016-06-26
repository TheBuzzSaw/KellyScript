#include "SyntaxTree.hpp"
#include "Tools.hpp"
#include <iostream>
#include <string>
using namespace std;

namespace Kelly
{
    void AbstractSyntaxTree::Eat(const TreeFood& food)
    {
        auto source = food.source.data();
        string buffer;

        for (size_t i = 0; i < food.tokens.size(); ++i)
        {
            auto token = food.tokens[i];
            View<const char> v = {source + token.start, token.length};

            if (token.type != Token::Identifier)
            {
                cout << food.path << ':' << token.row << ':' << token.column
                    << ": expected export, import, or function declaration;"
                    " found " << TokenTypeString(token.type) << ' ' << v
                    << " instead\n";

                break;
            }

            if (v == "export")
            {
                token = food.tokens[++i];
                v = {source + token.start, token.length};

                if (token.type != Token::Identifier)
                {
                    cout << food.path << ':' << token.row << ':'
                        << token.column
                        << ": expected package identifier; found "
                        << TokenTypeString(token.type) << ' ' << v
                        << " instead\n";

                    break;
                }

                buffer.clear();
                buffer.append(v.first, v.length);

                token = food.tokens[++i];
                v = {source + token.start, token.length};

                if (token.type != Token::Symbol)
                {
                    cout << food.path << ':' << token.row << ':'
                        << token.column
                        << ": expected '.' or ';'; found "
                        << TokenTypeString(token.type) << ' ' << v
                        << " instead\n";

                    break;
                }

                while (token.type == Token::Symbol)
                {
                    if (v == ";") break;

                    if (v != ".")
                    {
                        cout << food.path << ':' << token.row << ':'
                            << token.column
                            << ": expected '.' or ';'; found "
                            << TokenTypeString(token.type) << ' ' << v
                            << " instead\n";

                        buffer.clear();
                        break;
                    }

                    buffer += '.';
                    token = food.tokens[++i];
                    v = {source + token.start, token.length};

                    if (token.type != Token::Identifier)
                    {
                        cout << food.path << ':' << token.row << ':'
                            << token.column
                            << ": expected package identifier; found "
                            << TokenTypeString(token.type) << ' ' << v
                            << " instead\n";

                        buffer.clear();
                        break;
                    }

                    buffer.append(v.first, v.length);
                    token = food.tokens[++i];
                    v = {source + token.start, token.length};
                }

                if (buffer.size() > 0)
                {
                    auto v2 = stringMemory.AllocateAndWriteString(
                        buffer.size(),
                        buffer.data());

                    cout << "exported " << v2 << endl;
                }
                else
                {
                    break;
                }
            }
            else if (v == "import")
            {
            }
            else
            {
                // Name of function declaration?
            }
        }
    }
}
