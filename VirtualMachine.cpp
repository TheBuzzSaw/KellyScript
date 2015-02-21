#include "VirtualMachine.hpp"

#include <windows.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>

namespace Kelly
{
    struct State
    {
        uint8_t* stackPointer;
        uint8_t* framePointer;
        const uint8_t* current;
    };

    template<typename T>
    void Push(State& state, const void* source)
    {
        *reinterpret_cast<T*>(state.stackPointer) =
            *reinterpret_cast<const T*>(source);
        state.stackPointer += sizeof(T);
    }

    template<typename T>
    T Pop(State& state)
    {
        state.stackPointer -= sizeof(T);
        return *reinterpret_cast<const T*>(state.stackPointer);
    }

    template<typename T>
    void PushLiteral(State& state)
    {
        Push<T>(state, state.current + 1);
        state.current += sizeof(T);
    }

    template<typename T>
    void PushLocal(State& state)
    {
        uint16_t offset =
            *reinterpret_cast<const uint16_t*>(state.current + 1);

        state.current += 2;

        *reinterpret_cast<T*>(state.stackPointer) =
            *reinterpret_cast<const T*>(state.framePointer + offset);

        state.stackPointer += sizeof(T);
    }

    template<typename T>
    void PushCopy(State& state)
    {
        *reinterpret_cast<T*>(state.stackPointer) =
            *reinterpret_cast<T*>(state.stackPointer - sizeof(T));

        state.stackPointer += sizeof(T);
    }

    template<typename T>
    void StoreLocal(State& state)
    {
        uint16_t offset =
            *reinterpret_cast<const uint16_t*>(state.current + 1);

        state.current += 2;

        *reinterpret_cast<T*>(state.framePointer + offset) = Pop<T>(state);
    }

    template<typename T>
    void Add(State& state)
    {
        T a = Pop<T>(state);
        T b = Pop<T>(state);
        T c = a + b;
        Push<T>(state, &c);
    }

    void Run(const uint8_t* bytecodes)
    {
        std::vector<uint8_t> memStack(1024);
        State state;
        state.stackPointer = memStack.data();
        state.framePointer = state.stackPointer;
        state.current = bytecodes;

        std::cerr << std::hex;

        while (true)
        {
            using namespace Bytecodes;
            //std::cerr
            //  << "offset " << std::dec << (int)(current - bytecodes)
            //  << " bytecode " << std::hex << (int)(*current) << '\n';
            //::Sleep(500);

            switch(*state.current)
            {
                case Noop: break;

                case Jump:
                {
                    uint16_t offset =
                        *reinterpret_cast<const uint16_t*>(state.current + 1);

                    state.current = bytecodes + offset - 1;

                    break;
                }

                case Pop8: state.stackPointer -= 1; break;
                case Pop16: state.stackPointer -= 2; break;
                case Pop32: state.stackPointer -= 4; break;
                case Pop64: state.stackPointer -= 8; break;

                case PushLiteral8: PushLiteral<uint8_t>(state); break;
                case PushLiteral16: PushLiteral<uint16_t>(state); break;
                case PushLiteral32: PushLiteral<uint32_t>(state); break;
                case PushLiteral64: PushLiteral<uint64_t>(state); break;

                case PushLocal8: PushLocal<uint8_t>(state); break;
                case PushLocal16: PushLocal<uint16_t>(state); break;
                case PushLocal32: PushLocal<uint32_t>(state); break;
                case PushLocal64: PushLocal<uint64_t>(state); break;

                case PushCopy8: PushCopy<uint8_t>(state); break;
                case PushCopy16: PushCopy<uint16_t>(state); break;
                case PushCopy32: PushCopy<uint32_t>(state); break;
                case PushCopy64: PushCopy<uint64_t>(state); break;

                case StoreLocal8: StoreLocal<uint8_t>(state); break;
                case StoreLocal16: StoreLocal<uint16_t>(state); break;
                case StoreLocal32: StoreLocal<uint32_t>(state); break;
                case StoreLocal64: StoreLocal<uint64_t>(state); break;

                case AddS8: Add<int8_t>(state); break;
                case AddS16: Add<int16_t>(state); break;
                case AddS32: Add<int32_t>(state); break;
                case AddS64: Add<int64_t>(state); break;

                case JumpIfGE32:
                {
                    uint16_t offset =
                        *reinterpret_cast<const uint16_t*>(state.current + 1);

                    auto b = Pop<uint32_t>(state);
                    auto a = Pop<uint32_t>(state);

                    //std::cerr << "a " << a << " b " << b << '\n';

                    if (a >= b)
                        state.current = bytecodes + offset - 1;
                    else
                        state.current += 2;

                    break;
                }

                case OutS8:
                    std::cout << Pop<int8_t>(state) << std::endl;
                    break;

                case OutS16:
                    std::cout << Pop<int16_t>(state) << std::endl;
                    break;

                case OutS32:
                    std::cout << Pop<int32_t>(state) << std::endl;
                    break;

                case OutS64:
                    std::cout << Pop<int64_t>(state) << std::endl;
                    break;

                case Exit:
                    return;

                default:
                    std::cerr
                        << "unrecognized byte code: "
                        << (int)(*state.current)
                        << '\n';
                    return;
            }

            ++state.current;
        }
    }
}
