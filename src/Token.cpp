#include "Token.hpp"

Token::Token(Type type, const std::string& value)
    : m_type(type), m_value(value)
{
}

Token::~Token()
{
}

Token::Type Token::GetType() const
{
    return m_type;
}

const std::string& Token::GetValue() const
{
    return m_value;
}

std::string Token::ToString() const
{
    return "{ TYPE: " + std::to_string((int)m_type) + " VALUE: " + std::string(m_value) + "}\n";
}