#include "SyntaxTree.hpp"
#include "DynamicStack.hpp"
#include "Utf8CodePoint.hpp"
#include "Tools.hpp"
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

    for (const char** i = strings; *i; ++i)
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
    const char* i = content.data();

    Utf8CodePoint codePoint = GetUtf8CodePoint(i);
    auto utf32CodePoint = GetUtf32CodePoint(codePoint);

    fout << hex;
    while (utf32CodePoint > 0)
    {
        fout << utf32CodePoint;

        if (utf32CodePoint < 127)
            fout << " (" << char(utf32CodePoint) << ")";

        fout << '\n';

        i += GetLength(codePoint);
        codePoint = GetUtf8CodePoint(i);
        utf32CodePoint = GetUtf32CodePoint(codePoint);
    }

    fout.close();
}

int main(int argc, char** argv)
{
    Utf8CodePoint cpa;
    Utf8CodePoint cpb = {};

    TestUtf8Content();

    cout << "code point A: " << cpa.value << endl;
    cout << "code point B: " << cpb.value << endl;

    cout << "view size: " << sizeof(View<char>) << endl;
    //TestDynamicStack();

    if (argc > 1)
    {
        View<char*> arguments = { argv + 1, size_t(argc - 1) };
        SyntaxTree tree(arguments);
    }
    else
    {
        cout << "usage: " << argv[0] << " <path>" << endl;
    }

    return 0;
}
