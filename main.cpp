#include "SyntaxTree.hpp"
#include "DynamicStack.hpp"
#include <iostream>
#include <cstring>
using namespace std;
using namespace Kelly;

View<char> Allocate(DynamicStack& ds, const char* text)
{
    size_t length = strlen(text);

    auto view = ds.Allocate(length);
    memcpy(view.first, text, length);

    View<char> result;
    result.first = reinterpret_cast<char*>(view.first);
    result.length = view.length;
    return result;
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
        nullptr
    };

    DynamicStack ds(64);

    for (const char** i = strings; *i; ++i)
    {
        cout << Allocate(ds, *i) << endl;
    }

    //ds.ReleaseAll();
    ds.Dump(cout);
}

int main(int argc, char** argv)
{
    TestDynamicStack();

    size_t n = 1 << 19;

    for (int i = 0; i < 8; ++i)
        cout << (n <<= 1) << endl;

    if (argc > 1)
    {
        View<char*> arguments;
        arguments.first = argv + 1;
        arguments.length = argc - 1;

        SyntaxTree tree(arguments);
    }
    else
    {
        cout << "usage: " << argv[0] << " <path>" << endl;
    }

    return 0;
}
