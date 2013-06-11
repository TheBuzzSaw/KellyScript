#include "Token.hpp"
using namespace std;

int main(int argc, char** argv)
{
    const char* input = "712 +   6.55.23423.22 = \"asdf\"";

    Kelly::Token token(input);

    while (token.TokenType() != Kelly::Token::None)
    {
        cout << "[" << token.Length() << "] " << token.TokenType()
            << " -- " << token << endl;

        token = Kelly::Token(token.Start() + token.Length());
    }

    return 0;
}
