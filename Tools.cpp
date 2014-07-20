#include "Tools.hpp"
//#include <cstdlib>
#include <fstream>

namespace Kelly
{
    std::vector<char> FileToString(const char* path)
    {
        std::vector<char> result;

        if (path && *path)
        {
            std::ifstream fin(path, std::ifstream::binary);

            if (fin)
            {
                fin.seekg(0, fin.end);
                auto length = fin.tellg();
                fin.seekg(0, fin.beg);

                result.resize(length + 1);
                fin.read(result.data(), length);
                result[length] = 0;

                fin.close();
            }
        }

        return result;
    }
}
