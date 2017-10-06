#include "SourceFile.hpp"
#include <iostream>
#include <string>
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
            else if (sourceToken.tokenType == TokenType::StringLiteral)
            {
                cout << "string -- text ";
                cout.write(
                    sourceFile.source.data() + sourceToken.offset,
                    sourceToken.length);
                //cout << ' ' << sourceToken.tokenIndex << endl;
                //continue;
                cout << " parsed "
                    << sourceFile.strings[sourceToken.tokenIndex]
                    << '\n';
            }
        }
    }
    
    cout << "asdf\x5eg\n";
    cout << "\nPRESS ENTER\n";
    string junk;
    getline(cin, junk);
    
    return 0;
}

