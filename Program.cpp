#include "Program.hpp"
#include "Token.hpp"
#include <vector>

namespace Kelly
{
    Program::Program()
    {
        _firstInstruction = 0;
    }

    Program::~Program()
    {
        delete _firstInstruction;
    }

    void Program::Load(const char* input)
    {
        std::vector<Token> tokens;

        Token token(input);

        while (token.TokenType() != Token::None)
        {
            tokens.push_back(token);
            token = Token(token.Start() + token.Length());
        }
    }

    void Program::Run()
    {
        for (Instruction* i = _firstInstruction; i; i = i->NextInstruction())
            i->Execute();
    }
}
