#include "SyntaxTree.hpp"
#include "DynamicStack.hpp"
#include "Utf8CodePoint.hpp"
#include "Tools.hpp"
#include "VirtualMachine.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;
using namespace Kelly;

const View<char> Allocate(DynamicStack& ds, const char* text)
{
    size_t length = strlen(text);

    auto view = ds.Allocate(length);
    memcpy(view.first, text, length);

    return { reinterpret_cast<char*>(view.first), view.length };
}

void TestDynamicStack()
{
    const char* strings[] = {
        "Hello",
        "Goodbye",
        "What",
        "Where",
        "Sky",
        "Earth",
        "Science",
        "Religion",
        "Power",
        "Necromancy",
        "ASDF",
        "White",
        "Triangle",
        nullptr
    };

    DynamicStack ds(64);
    vector<View<char>> _views;

    for (auto i = strings; *i; ++i)
    {
        _views.push_back(Allocate(ds, *i));
    }

    for (auto view : _views) cout << view << endl;

    ds.Dump(cout);
    ds.ReleaseAll();
    cout << "After ReleaseAll()...\n";
    ds.Dump(cout);
}

void TestUtf8Content()
{
    ofstream fout("dump.txt", ofstream::binary);

    if (!fout) return;

    auto content = FileToString("UTF-8-demo.txt");
    auto i = content.c_str();

    Utf8CodePoint codePoint = GetUtf8CodePoint(i);
    auto utf32CodePoint = GetUtf32CodePoint(codePoint);

    while (utf32CodePoint > 0)
    {
        fout
            << hex << "0x" << utf32CodePoint
            << " (" << dec << utf32CodePoint << ") '";

        fout.write(codePoint.chars, GetLength(codePoint)) << '\'';

        //if (31 < utf32CodePoint && utf32CodePoint < 127)
            //fout << " '" << char(utf32CodePoint) << "'";

        fout << '\n';

        i += GetLength(codePoint);
        codePoint = GetUtf8CodePoint(i);
        utf32CodePoint = GetUtf32CodePoint(codePoint);
    }

    fout.close();
}

void TestBytecodes()
{
    using namespace Bytecodes;

    uint32_t n = 0x80000000;
    cout << n << ", " << -n << endl;

    int total = 0;
    for (int i = 0; i < 16; ++i) total += i;
    cout << "Answer: " << total << endl;

    uint8_t bytecodes[] = {
        PushLiteral32, 0x00, 0x00, 0x00, 0x00, // local 'total'
        PushLiteral32, 0x00, 0x00, 0x00, 0x00, // local 'i'
        PushLocal32, 0x04, 0x00, // load 'i'
        PushLiteral32, 0x10, 0x00, 0x00, 0x00, // load 16
        JumpIfGE32, 46, 0x00,
        PushLocal32, 0x00, 0x00, // load 'total'
        PushLocal32, 0x04, 0x00, // load 'i'
        AddS32,
        PopToLocal32, 0x00, 0x00, // store 'total'
        PushLocal32, 0x04, 0x00, // load 'i'
        PushLiteral32, 0x01, 0x00, 0x00, 0x00,
        AddS32,
        PopToLocal32, 0x04, 0x00, // store 'i'
        Jump, 10, 0x00,
        PushLocal32, 0x00, 0x00, // load 'total'
        OutS32,
        Noop,
        Exit,
        Noop
        };

    Run(bytecodes);
}

int main(int argc, char** argv)
{
    vector<int> vt(128);
    vt.clear();
    //vt.shrink_to_fit();
    cout << "vector test: " << vt.capacity() << endl;

    for (int i = 1; i < argc; ++i)
    {
        auto treeFood = ParseFile(argv[i]);

        cout << "parsed " << treeFood.tokens.size() << " tokens\n";

        for (auto value : treeFood.integers)
        {
            cout << "integer value " << value.value << " @ "
                << value.tokenIndex << endl;
        }

        for (auto token : treeFood.tokens)
        {
            auto tokenTypeName = "unknown";

            switch(token.type)
            {
                case Token::Identifier: tokenTypeName = "identifier"; break;
                case Token::Symbol: tokenTypeName = "symbol"; break;
                case Token::NumericLiteral: tokenTypeName = "number"; break;
                case Token::StringLiteral: tokenTypeName = "string"; break;
                default: break;
            }

            View<const char> v
                {treeFood.source.data() + token.start, token.length};

            cout << tokenTypeName << " @ row " << token.row << " col "
                << token.column << " " << v << '\n';
        }
    }

    if (argc < 2)
    {
        cout << "usage: " << argv[0] << " <path>" << endl;
    }

    return 0;
}
