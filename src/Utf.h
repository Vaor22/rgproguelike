#pragma once
#include <SFML/System/String.hpp>
#include <string>

// Хелперы для явного декодирования UTF-8 строк в sf::String.
// SFML по умолчанию интерпретирует const char* через системную локаль
// (на Windows это CP-1251/1252), из-за чего русские строки ломаются.
// u8() говорит: "эти байты — UTF-8, декодируй их корректно".

inline sf::String u8(const std::string& s)
{
    return sf::String::fromUtf8(s.begin(), s.end());
}

inline sf::String u8(const char* s)
{
    std::string tmp(s);
    return sf::String::fromUtf8(tmp.begin(), tmp.end());
}
