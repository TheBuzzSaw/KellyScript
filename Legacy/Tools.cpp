#include "Tools.hpp"
#include <fstream>
#include <sstream>

namespace Kelly
{
    std::string FileToString(const char* path)
    {
        std::string result;

        if (HasContent(path))
        {
            std::ifstream fin(path, std::ifstream::binary);

            if (fin)
            {
                std::ostringstream oss;
                oss << fin.rdbuf();
                result = oss.str();
                fin.close();
            }
        }

        return result;
    }
}
