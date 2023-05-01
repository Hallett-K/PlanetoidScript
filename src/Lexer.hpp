#pragma once

#include <string>
#include <vector>

#include "Position.hpp"

class Token;
class Lexer
{
public:
    Lexer(const std::string& fileName, const std::string& source);
    ~Lexer();

    std::vector<Token> generateTokens();
private:
    std::string m_source;
    Position m_position;
    char m_currentChar;
    
    void advance();
};