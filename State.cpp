#include "State.hpp"

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
    }

    void State::Step()
    {
        if (_currentInstruction)
        {
            _currentInstruction->Execute();
            _currentInstruction = _currentInstruction->NextInstruction();
        }
    }
}
