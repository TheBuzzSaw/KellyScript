#include "SyntaxTree.hpp"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        Kelly::View<char*> arguments;
        arguments.first = argv + 1;
        arguments.length = argc - 1;

        Kelly::SyntaxTree tree(arguments);
    }
    else
    {
        cout << "usage: " << argv[0] << " <path>" << endl;
    }

    return 0;
}
