#pragma once

#include <string>
#include "Position.hpp"

class Error
{
public:
    Error(const std::string& message, const Position& position);
    ~Error();

    std::string GetMessage() const;
    Position GetPosition() const;

    std::string ToString() const;

private:
    std::string m_message;
    Position m_position;
};