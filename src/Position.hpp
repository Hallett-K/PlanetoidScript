#pragma once

#include <string>

class Position
{
public:
    Position(const std::string& fileName, unsigned int line, unsigned int column, unsigned int index);
    ~Position();

    std::string GetFileName() const;
    unsigned int GetLine() const;
    unsigned int GetColumn() const;
    unsigned int GetIndex() const;

    void advance(char currentChar);
private:
    std::string m_fileName;
    unsigned int m_line;
    unsigned int m_column;
    unsigned int m_index;
};