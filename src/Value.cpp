#include "Value.hpp"

#include <cmath>

Value::Value()
    : m_number(0.0f), m_type(Type::Null)
{
}

Value::Value(const Value& other)
    : m_type(other.m_type)
{
    if (m_type == Type::Number)
    {
        m_number = other.m_number;
    }
    else if (m_type == Type::String || m_type == Type::ObjectPointer)
    {
        new (&m_string) std::string(other.m_string);
    }
    else if (m_type == Type::Array)
    {
        new (&m_array) std::vector<Value>(other.m_array);
    }
}

Value::Value(float value)
    : m_number(value), m_type(Type::Number)
{
}

Value::Value(const std::string& value, bool isString)
    : m_string(value)
{
    if (isString)
    {
        m_type = Type::String;
    }
    else
    {
        m_type = Type::ObjectPointer;
    }
}

Value::Value(const std::vector<Value>& value)
    : m_array(value), m_type(Type::Array)
{
}

Value::~Value()
{
}

void Value::operator=(const Value& other)
{
    m_type = other.m_type;

    if (m_type == Type::Number)
    {
        m_number = other.m_number;
    }
    else if (m_type == Type::String || m_type == Type::ObjectPointer)
    {
        new (&m_string) std::string(other.m_string);
    }
    else if (m_type == Type::Array)
    {
        new (&m_array) std::vector<Value>(other.m_array);
    }
}

void Value::operator=(float value)
{
    m_type = Type::Number;
    m_number = value;
}

void Value::operator=(const std::string& value)
{
    m_type = Type::String;
    new (&m_string) std::string(value);
}

void Value::operator=(const std::vector<Value>& value)
{
    m_type = Type::Array;
    new (&m_array) std::vector<Value>(value);
}

Value Value::operator+(const Value& other) const
{
    if (m_type == Type::Number && other.m_type == Type::Number)
    {
        return m_number + other.m_number;
    }
    else if (m_type == Type::String && other.m_type == Type::String)
    {
        return m_string + other.m_string;
    }
    else if (m_type == Type::String && other.m_type == Type::Number)
    {
        return m_string + std::to_string(other.m_number);
    }
    else if (m_type == Type::Number && other.m_type == Type::String)
    {
        return std::to_string(m_number) + other.m_string;
    }
    else if (m_type == Type::Array && other.m_type == Type::Array)
    {
        std::vector<Value> result(m_array);
        result.insert(result.end(), other.m_array.begin(), other.m_array.end());
        return result;
    }
    else if (m_type == Type::Array)
    {
        std::vector<Value> result(m_array);
        result.push_back(other);
        return result;
    }
    else
    {
        return Value();
    }
}

Value Value::operator+(float value) const
{
    if (m_type == Type::Number)
    {
        return m_number + value;
    }
    else if (m_type == Type::String)
    {
        return m_string + std::to_string(value);
    }
    else
    {
        return Value();
    }
}

Value Value::operator+(const std::string& value) const
{
    if (m_type == Type::String)
    {
        return m_string + value;
    }
    else if (m_type == Type::Number)
    {
        return std::to_string(m_number) + value;
    }
    else
    {
        return Value();
    }
}

Value Value::operator-(const Value& other) const
{
    if (m_type == Type::Number && other.m_type == Type::Number)
    {
        return m_number - other.m_number;
    }
    else
    {
        return Value();
    }
}

Value Value::operator-(float value) const
{
    if (m_type == Type::Number)
    {
        return m_number - value;
    }
    else
    {
        return Value();
    }
}

Value Value::operator*(const Value& other) const
{
    if (m_type == Type::Number && other.m_type == Type::Number)
    {
        return m_number * other.m_number;
    }
    else
    {
        return Value();
    }
}

Value Value::operator*(float value) const
{
    if (m_type == Type::Number)
    {
        return m_number * value;
    }
    else
    {
        return Value();
    }
}

Value Value::operator/(const Value& other) const
{
    if (m_type == Type::Number && other.m_type == Type::Number)
    {
        return m_number / other.m_number;
    }
    else
    {
        return Value();
    }
}

Value Value::operator/(float value) const
{
    if (m_type == Type::Number)
    {
        return m_number / value;
    }
    else
    {
        return Value();
    }
}

Value Value::operator^(const Value& other) const
{
    if (m_type == Type::Number && other.m_type == Type::Number)
    {
        return std::pow(m_number, other.m_number);
    }
    else
    {
        return Value();
    }
}

Value Value::operator^(float value) const
{
    if (m_type == Type::Number)
    {
        return std::pow(m_number, value);
    }
    else
    {
        return Value();
    }
}

Value Value::operator-() const
{
    if (m_type == Type::Number)
    {
        return -m_number;
    }
    else
    {
        return Value();
    }
}

bool Value::operator==(const Value& other) const
{
    if (m_type == Type::Number && other.m_type == Type::Number)
    {
        return m_number == other.m_number;
    }
    else if (m_type == Type::String && other.m_type == Type::String
        || m_type == Type::ObjectPointer && other.m_type == Type::ObjectPointer)
    {
        return m_string == other.m_string;
    }
    else
    {
        return false;
    }
}

bool Value::operator==(float value) const
{
    if (m_type == Type::Number)
    {
        return m_number == value;
    }
    else
    {
        return false;
    }
}

bool Value::operator==(const std::string& value) const
{
    if (m_type == Type::String || m_type == Type::ObjectPointer)
    {
        return m_string == value;
    }
    else
    {
        return false;
    }
}

bool Value::operator==(const std::vector<Value>& value) const
{
    if (m_type == Type::Array)
    {
        return m_array == value;
    }
    else
    {
        return false;
    }
}

bool Value::operator!=(const Value& other) const
{
    if (m_type == Type::Number && other.m_type == Type::Number)
    {
        return m_number != other.m_number;
    }
    else if (m_type == Type::String && other.m_type == Type::String
        || m_type == Type::ObjectPointer && other.m_type == Type::ObjectPointer)
    {
        return m_string != other.m_string;
    }
    else
    {
        return true;
    }
}

bool Value::operator!=(float value) const
{
    if (m_type == Type::Number)
    {
        return m_number != value;
    }
    else
    {
        return true;
    }
}

bool Value::operator!=(const std::string& value) const
{
    if (m_type == Type::String || m_type == Type::ObjectPointer)
    {
        return m_string != value;
    }
    else
    {
        return true;
    }
}

bool Value::operator!=(const std::vector<Value>& value) const
{
    if (m_type == Type::Array)
    {
        return m_array != value;
    }
    else
    {
        return true;
    }
}

bool Value::operator>(const Value& other) const
{
    if (m_type == Type::Number && other.m_type == Type::Number)
    {
        return m_number > other.m_number;
    }
    else
    {
        return false;
    }
}

bool Value::operator>(float value) const
{
    if (m_type == Type::Number)
    {
        return m_number > value;
    }
    else
    {
        return false;
    }
}

bool Value::operator<(const Value& other) const
{
    if (m_type == Type::Number && other.m_type == Type::Number)
    {
        return m_number < other.m_number;
    }
    else
    {
        return false;
    }
}

bool Value::operator<(float value) const
{
    if (m_type == Type::Number)
    {
        return m_number < value;
    }
    else
    {
        return false;
    }
}

bool Value::operator>=(const Value& other) const
{
    if (m_type == Type::Number && other.m_type == Type::Number)
    {
        return m_number >= other.m_number;
    }
    else
    {
        return false;
    }
}

bool Value::operator>=(float value) const
{
    if (m_type == Type::Number)
    {
        return m_number >= value;
    }
    else
    {
        return false;
    }
}

bool Value::operator<=(const Value& other) const
{
    if (m_type == Type::Number && other.m_type == Type::Number)
    {
        return m_number <= other.m_number;
    }
    else
    {
        return false;
    }
}

bool Value::operator<=(float value) const
{
    if (m_type == Type::Number)
    {
        return m_number <= value;
    }
    else
    {
        return false;
    }
}

std::string Value::toString() const
{
    if (m_type == Type::Number)
    {
        if (m_number == std::floor(m_number))
        {
            return std::to_string((int)m_number);
        }
        else
        {
            return std::to_string(m_number);
        }
    }
    else if (m_type == Type::String || m_type == Type::ObjectPointer)
    {
        return m_string;
    }
    else if (m_type == Type::Array)
    {
        std::string result = "[";
        for (int i = 0; i < m_array.size(); i++)
        {
            result += m_array[i].toString();
            if (i < m_array.size() - 1)
            {
                result += ", ";
            }
        }
        result += "]";
        return result;
    }
    else
    {
        return "NULL";
    }
}

size_t Value::size() const
{
    if (m_type == Type::Array)
    {
        return m_array.size();
    }
    else if (m_type == Type::String)
    {
        return m_string.size();
    }
    else if (m_type == Type::ObjectPointer)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}