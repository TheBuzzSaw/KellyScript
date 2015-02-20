#include "VirtualMachine.hpp"

#include <iostream>
#include <vector>
#include <cstring>

namespace Kelly
{
    template<typename T>
    void Push(const void* source, uint8_t*& destination)
    {
        *reinterpret_cast<T*>(destination) = *reinterpret_cast<const T*>(source);
        destination += sizeof(T);
    }

    template<typename T>
    T Pop(uint8_t*& stackPointer)
    {
        stackPointer -= sizeof(T);
        return *reinterpret_cast<const T*>(stackPointer);
    }

    void Run(const uint8_t* bytecodes)
    {
        std::vector<uint8_t> memStack(1024);
        uint8_t* stackPointer = memStack.data();
        const uint8_t* current = bytecodes;

        while (true)
        {
            using namespace Bytecodes;

            switch(*current)
            {
                case Noop: ++current; break;

                case PushLiteral8:
                    Push<uint8_t>(current + 1, stackPointer);
                    current += 2;
                    break;

                case PushLiteral16:
                    Push<uint16_t>(current + 1, stackPointer);
                    current += 3;
                    break;

                case PushLiteral32:
                    Push<uint32_t>(current + 1, stackPointer);
                    current += 5;
                    break;

                case PushLiteral64:
                    Push<uint64_t>(current + 1, stackPointer);
                    current += 9;
                    break;

                case AddS8:
                {
                    int8_t a = Pop<int8_t>(stackPointer);
                    int8_t b = Pop<int8_t>(stackPointer);
                    int8_t c = a + b;
                    Push<int8_t>(&c, stackPointer);
                    ++current;
                    break;
                }

                case AddS16:
                {
                    int16_t a = Pop<int16_t>(stackPointer);
                    int16_t b = Pop<int16_t>(stackPointer);
                    int16_t c = a + b;
                    Push<int16_t>(&c, stackPointer);
                    ++current;
                    break;
                }

                case OutS16:
                    std::cout << Pop<int16_t>(stackPointer) << std::endl;
                    ++current;
                    break;

                case Exit:
                default:
                    return;
            }
        }
    }
}
