#include "State.hpp"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        char* path = argv[1];
        Kelly::State state;

        if (state.LoadFromFile(path))
        {
            cout << "Load successful." << endl;
        }
        else
        {
            cout << "failed to open file: " << path << endl;
        }
    }
    else
    {
        cout << "usage: " << argv[0] << " <path>" << endl;
    }

    return 0;
}
