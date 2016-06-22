#include "SyntaxTree.hpp"
#include "Tools.hpp"
#include <iostream>
using namespace std;

namespace Kelly
{
    Value<Package> AbstractSyntaxTree::FindPackage(Package package)
    {
        for (size_t i = 0; i < packages.size(); ++i)
        {
            auto p = packages[i];
            if (package.name == p.name && package.parentIndex == p.parentIndex)
                return {static_cast<int32_t>(i), p};
        }

        return {-1, {}};
    }

    AbstractSyntaxTree Eat(const TreeFood& food)
    {
        AbstractSyntaxTree result;
        auto source = food.source.data();
        int32_t exportPackageIndex = -1;
        std::vector<int32_t> importedPackageIndices;

        for (size_t i = 0; i < food.tokens.size(); ++i)
        {
            auto token = food.tokens[i];
            View<const char> v = {source + token.start, token.length};

            if (v == "import")
            {
                cout << "found an import!\n";

                token = food.tokens[++i];
                v = {source + token.start, token.length};

                if (token.type == Token::Identifier)
                {
                    auto package = result.FindPackage({v, -1});

                    if (package.index < 0)
                    {
                        cout << food.path << ':' << token.row << ':'
                            << token.column << ':'
                            << " no such package " << v
                            << '\n';

                        break;
                    }

                    token = food.tokens[++i];
                    v = {source + token.start, token.length};

                    while (token.type == Token::Symbol)
                    {
                        if (v == ";") break;

                        if (v != ".")
                        {
                            cout << food.path << ':' << token.row << ':'
                                << token.column << ':'
                                << " expected '.' or ';'; found "
                                << TokenTypeString(token.type) << ' ' << v
                                << '\n';

                            break;
                        }

                        token = food.tokens[++i];
                        v = {source + token.start, token.length};

                        if (token.type == Token::Identifier)
                        {
                            package = result.FindPackage({v, package.index});

                            if (package.index < 0)
                            {
                                cout << food.path << ':' << token.row << ':'
                                    << token.column << ':'
                                    << " no such package " << v
                                    << '\n';

                                break;
                            }

                            importedPackageIndices.push_back(package.index);
                        }
                        else
                        {
                            cout << food.path << ':' << token.row << ':'
                                << token.column << ':'
                                << " expected package identifier; found "
                                << TokenTypeString(token.type) << ' ' << v
                                << '\n';

                            break;
                        }

                        token = food.tokens[++i];
                        v = {source + token.start, token.length};
                    }
                }
                else
                {
                    cout << food.path << ':' << token.row << ':'
                        << token.column << ':'
                        << " expected package identifier; found "
                        << TokenTypeString(token.type) << ' ' << v
                        << '\n';

                    break;
                }
            }
            else if (v == "export")
            {
                if (exportPackageIndex >= 0)
                {
                    cout << "Exporting another package! We already exported "
                        << result.packages[exportPackageIndex].name << "!\n";
                }

                token = food.tokens[++i];
                v = {source + token.start, token.length};

                if (token.type == Token::Identifier)
                {
                    auto package = result.FindPackage({v, -1});

                    if (package.index < 0)
                    {
                        package.index = static_cast<int32_t>(
                            result.packages.size());
                        package.value = {v, -1};
                        result.packages.push_back(package.value);
                    }

                    token = food.tokens[++i];
                    v = {source + token.start, token.length};

                    while (token.type == Token::Symbol)
                    {
                        if (v == ";") break;

                        if (v != ".")
                        {
                            cout << food.path << ':' << token.row << ':'
                                << token.column << ':'
                                << " expected '.' or ';'; found "
                                << TokenTypeString(token.type) << ' ' << v
                                << '\n';

                            break;
                        }

                        token = food.tokens[++i];
                        v = {source + token.start, token.length};

                        if (token.type == Token::Identifier)
                        {
                            auto nextPackage = result.FindPackage(
                                {v, package.index});

                            if (nextPackage.index < 0)
                            {
                                nextPackage.index = static_cast<int32_t>(
                                    result.packages.size());
                                nextPackage.value.name = v;
                                nextPackage.value.parentIndex = package.index;
                                result.packages.push_back(nextPackage.value);
                            }

                            package = nextPackage;

                            if (exportPackageIndex < 0)
                                exportPackageIndex = package.index;
                        }
                        else
                        {
                            cout << food.path << ':' << token.row << ':'
                                << token.column << ':'
                                << " expected package identifier; found "
                                << TokenTypeString(token.type) << ' ' << v
                                << '\n';

                            break;
                        }

                        token = food.tokens[++i];
                        v = {source + token.start, token.length};
                    }
                }
                else
                {
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

        cout << "imported packages:";
        for (auto index : importedPackageIndices)
        {
            auto package = result.packages[index];
            cout << "\n[" << index << "]" << package.name;

            while (package.parentIndex >= 0)
            {
                cout << " <- [" << package.parentIndex << "]";
                package = result.packages[package.parentIndex];
                cout << package.name;
            }
        }

        cout << '\n';

        return result;
    }
}
