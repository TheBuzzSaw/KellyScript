#include "SourceFile.hpp"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
    {
        auto sourceFile = LexSource(argv[1]);
    }
    
    return 0;
}

