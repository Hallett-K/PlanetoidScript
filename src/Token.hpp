#pragma once

#include <string>

class Token
{
public:
    enum class Type
    {
        None,
        Number,
        String,
        Identifier,
        Keyword,
        Plus,
        PlusEqual,
        Increment,
        Minus,
        MinusEqual,
        Decrement,
        Multiply,
        Divide,
        Power,
        LeftParenthesis,
        RightParenthesis,
        LeftBrace,
        RightBrace,
        LeftBracket,
        RightBracket,
        Semicolon,
        Equals,
        IsEqual,
        IsNotEqual,
        GreaterThan,
        GreaterThanOrEqual,
        LessThan,
        LessThanOrEqual,
        ConditionalAnd,
        ConditionalOr,
        LogicalNot,
        Comma,
        Dot,
        EndOfFile
    };
private:
public:
    Token(Type type, const std::string& value);
    ~Token();

    Type GetType() const;
    const std::string& GetValue() const;

    std::string ToString() const;

private:
    Type m_type;
    std::string m_value;
};