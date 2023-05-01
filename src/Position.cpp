#include "Position.hpp"

Position::Position(const std::string& fileName, unsigned int line, unsigned int column, unsigned int index)
    : m_fileName(fileName), m_line(line), m_column(column), m_index(index)
{
}

Position::~Position()
{
}

std::string Position::GetFileName() const
{
    return m_fileName;
}

unsigned int Position::GetLine() const
{
    return m_line;
}

unsigned int Position::GetColumn() const
{
    return m_column;
}

unsigned int Position::GetIndex() const
{
    return m_index;
}

void Position::advance(char currentChar)
{
    m_index++;
    m_column++;

    if (currentChar == '\n')
    {
        m_line++;
        m_column = 0;
    }
}