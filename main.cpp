#include "State.hpp"
#include "View.hpp"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    Kelly::View<const char> view;
    view.first = "HELLO" + 1;
    view.length = 3;

    for (auto i : view) cout << i;
    cout << view << endl;

    if (argc > 1)
    {
        char* path = argv[1];
        Kelly::State state = Kelly::State::FromFile(path);

        if (state.IsReady())
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
