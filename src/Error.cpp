#include "Error.hpp"

Error::Error(const std::string& message, const Position& position)
    : m_message(message), m_position(position)
{
}

Error::~Error()
{
}

std::string Error::GetMessage() const
{
    return m_message;
}

Position Error::GetPosition() const
{
    return m_position;
}

std::string Error::ToString() const
{
    return "[" + m_position.GetFileName() + "] ERROR AT (Line: " + std::to_string(m_position.GetLine()) + ", Column: " + std::to_string(m_position.GetColumn()) + " [INDEX: " + std::to_string(m_position.GetIndex()) +  "]): " + m_message;
}