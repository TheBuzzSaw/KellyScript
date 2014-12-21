#include "State.hpp"
#include "Tools.hpp"
#include "Token.hpp"
#include <iostream>
#include <string>

namespace Kelly
{
    State State::FromString(const char* source)
    {
        if (HasContent(source))
        {
            std::vector<Token> tokens;

            Token token = FromSource(source);

            while (token.type != Token::Types::None)
            {
                tokens.push_back(token);
                token = FromSource(token.view.first + token.view.length);
            }
        }

        return State();
    }

    State State::FromFile(const char* path)
    {
        std::vector<char> script = Kelly::FileToString(path);

        return FromString(script.data());
    }

    State::State()
        : _currentInstruction(nullptr)
        , _stackPointer(nullptr)
    {
        _stack.resize(1024);
    }

    State::State(State&& other)
        : _currentInstruction(other._currentInstruction)
        , _stack(std::move(other._stack))
        , _stackPointer(other._stackPointer)
    {
        other._currentInstruction = nullptr;
        other._stackPointer = nullptr;
    }

    State::~State()
    {
    }

    State& State::operator=(State&& other)
    {
        if (this != &other)
        {
            _currentInstruction = other._currentInstruction;
            _stack = std::move(other._stack);
            _stackPointer = other._stackPointer;

            other._currentInstruction = nullptr;
            other._stackPointer = nullptr;
        }

        return *this;
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
        //TypeHandle nativeType = KeywordType | DataTypeType;
    }
}
