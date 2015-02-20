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

    template<typename T>
    void PushLiteral(const uint8_t*& source, uint8_t*& destination)
    {
        Push<T>(source + 1, destination);
        source += sizeof(T);
    }

    template<typename T>
    void PushCopy(uint8_t*& stackPointer)
    {
        *reinterpret_cast<T*>(stackPointer) =
            *reinterpret_cast<T*>(stackPointer - sizeof(T));

        stackPointer += sizeof(T);
    }

    template<typename T>
    void Add(uint8_t*& stackPointer)
    {
        T a = Pop<T>(stackPointer);
        T b = Pop<T>(stackPointer);
        T c = a + b;
        Push<T>(&c, stackPointer);
    }

    void Run(const uint8_t* bytecodes)
    {
        std::vector<uint8_t> memStack(1024);
        uint8_t* stackPointer = memStack.data();
        uint8_t* framePointer = stackPointer;
        const uint8_t* current = bytecodes;

        while (true)
        {
            using namespace Bytecodes;

            switch(*current)
            {
                case Noop: break;

                case PushLiteral8:
                    PushLiteral<uint8_t>(current, stackPointer);
                    break;

                case PushLiteral16:
                    PushLiteral<uint16_t>(current, stackPointer);
                    break;

                case PushLiteral32:
                    PushLiteral<uint32_t>(current, stackPointer);
                    break;

                case PushLiteral64:
                    PushLiteral<uint64_t>(current, stackPointer);
                    break;

                case PushCopy8:
                    PushCopy<uint8_t>(stackPointer);
                    break;

                case PushCopy16:
                    PushCopy<uint16_t>(stackPointer);
                    break;

                case PushCopy32:
                    PushCopy<uint32_t>(stackPointer);
                    break;

                case PushCopy64:
                    PushCopy<uint64_t>(stackPointer);
                    break;

                case AddS8:
                    Add<int8_t>(stackPointer);
                    break;

                case AddS16:
                    Add<int16_t>(stackPointer);
                    break;

                case AddS32:
                    Add<int32_t>(stackPointer);
                    break;

                case AddS64:
                    Add<int64_t>(stackPointer);
                    break;

                case OutS8:
                    std::cout << Pop<int8_t>(stackPointer) << std::endl;
                    break;

                case OutS16:
                    std::cout << Pop<int16_t>(stackPointer) << std::endl;
                    break;

                case OutS32:
                    std::cout << Pop<int32_t>(stackPointer) << std::endl;
                    break;

                case OutS64:
                    std::cout << Pop<int64_t>(stackPointer) << std::endl;
                    break;

                case Exit:
                    return;

                default:
                    std::cerr << "unrecognized byte code: " << (int)(*current) << '\n';
                    return;
            }

            ++current;
        }
    }
}
