#include "SourceFile.hpp"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    cout << "Version 0\n";
    PrepareLexer();
    for (int i = 1; i < argc; ++i)
    {
        auto sourceFile = LexSource(argv[1]);
        cout << sourceFile << "***\n";
        
        for (auto sourceToken : sourceFile.sourceTokens)
        {
            if (sourceToken.tokenType == TokenType::UnsignedIntegerLiteral)
            {
                cout << "integer -- text ";
                cout.write(
                    sourceFile.source.data() + sourceToken.offset,
                    sourceToken.length);
                cout << " parsed "
                    << sourceFile.literals[sourceToken.tokenIndex].asUInt64
                    << '\n';
            }
        }
    }
    
    return 0;
}

