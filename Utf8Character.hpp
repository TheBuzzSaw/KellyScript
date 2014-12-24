#ifndef UTF8CHARACTER_HPP
#define UTF8CHARACTER_HPP

#include <cstdint>

class Utf8Character
{
    public:
        constexpr Utf8Character() : _bytes{} {}
        constexpr Utf8Character(const Utf8Character&) = default;
        ~Utf8Character() = default;

        Utf8Character& operator=(const Utf8Character&) = default;

    protected:
    private:
        uint8_t _bytes[8];
};

#endif
