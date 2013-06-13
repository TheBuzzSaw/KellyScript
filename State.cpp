#include "State.hpp"
#include "Tools.hpp"
#include "Token.hpp"
#include <iostream>

namespace Kelly
{
    State::State()
    {
        _currentInstruction = 0;
        _stack = malloc(1024 * 1024 * 1);
        _stackPointer = reinterpret_cast<UInt8*>(_stack);
    }

    State::~State()
    {
        free(_stack);

        for (auto i = _identifiersByName.begin();
            i != _identifiersByName.end(); ++i)
        {
            delete i->second;
        }
    }

    void State::LoadFromString(const char* script)
    {
        if (HasContent(script))
        {
            std::vector<Token> tokens;

            Token token(script);

            while (token.Type() != Token::None)
            {
                tokens.push_back(token);
                token = Token(token.Start() + token.Length());
            }
        }
    }

    bool State::LoadFromFile(const char* path)
    {
        bool result = false;
        std::vector<char> script = Kelly::FileToString(path);

        if (script.size() > 0)
        {
            result = true;
            LoadFromString(&script[0]);
        }

        return result;
    }

    void State::Step()
    {
        if (_currentInstruction)
        {
            _currentInstruction->Execute();
            _currentInstruction = _currentInstruction->NextInstruction();
        }
    }

    void State::PrepareIdentifiers()
    {
        Identifier::TypeHandle nativeType = Identifier::Keyword
            | Identifier::DataType;

        Save(new Identifier("int16", nativeType, 0));
        Save(new Identifier("int32", nativeType, 0));
        Save(new Identifier("int64", nativeType, 0));
    }

    void State::Save(Identifier* identifier)
    {
        _identifiersByName[identifier->Name()] = identifier;
    }
}
