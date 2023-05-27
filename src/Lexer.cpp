#include "Lexer.hpp"

#include <algorithm>
#include <iostream>

#include "Error.hpp"
#include "SymbolTable.hpp"
#include "Token.hpp"

Lexer::Lexer(const std::string& fileName, const std::string& source)
    : m_source(source), m_position(fileName, 1, 0, 0), m_currentChar(m_source[m_position.GetIndex()])
{
}

Lexer::~Lexer()
{
}

void Lexer::advance()
{
    m_position.advance(m_currentChar);
    if (m_position.GetIndex() > m_source.length() - 1)
    {
        m_currentChar = '\0';
    }
    else
    {
        m_currentChar = m_source[m_position.GetIndex()];
    }
}

std::vector<Token> Lexer::generateTokens()
{
    std::vector<Token> tokens;
    while (m_currentChar != '\0')
    {
        if (m_currentChar == ' ' || m_currentChar == '\t' || m_currentChar == '\n' || m_currentChar == '\r')
        {
            advance();
        }
        else if (isdigit(m_currentChar))
        {
            unsigned int dotCount = 0;
            std::string numberString = "";
            while (isdigit(m_currentChar) || m_currentChar == '.')
            {
                if (m_currentChar == '.')
                {
                    dotCount++;
                }
                numberString += m_currentChar;
                advance();
            }
            if (dotCount > 1)
            {
                Error error("Invalid number format: " + numberString, m_position);
                std::cout << error.ToString() << '\n';
                return std::vector<Token>();
            }
            tokens.push_back(Token(Token::Type::Number, numberString));
        }
        else if (m_currentChar == '"')
        {
            std::string string = "";
            advance();
            while (m_currentChar != '"')
            {
                if (m_currentChar == '\0')
                {
                    Error error("Unterminated string", m_position);
                    std::cout << error.ToString() << '\n';
                    return std::vector<Token>();
                }
                string += m_currentChar;
                advance();
            }
            advance();
            tokens.push_back(Token(Token::Type::String, string));
        }
        else if (isalpha(m_currentChar))
        {
            // Identifier
            std::string identifier = "";
            while (isalnum(m_currentChar) || m_currentChar == '_')
            {
                identifier += m_currentChar;
                advance();
            }

            if (g_symbolTable.IsKeyword(identifier))
            {
                if (identifier == "true")
                {
                    tokens.push_back(Token(Token::Type::Number, std::to_string(1)));
                }
                else if (identifier == "false")
                {
                    tokens.push_back(Token(Token::Type::Number, std::to_string(0)));
                }
                else if (identifier == "null")
                {
                    tokens.push_back(Token(Token::Type::Number, ""));
                }
                else
                {
                    tokens.push_back(Token(Token::Type::Keyword, identifier));
                }
            }
            else
            {
                tokens.push_back(Token(Token::Type::Identifier, identifier));
            }
        }
        else if (m_currentChar == '+')
        {
            advance();
            if (m_currentChar == '+')
            {
                advance();
                tokens.push_back(Token(Token::Type::Increment, "++"));
            }
            else if (m_currentChar == '=')
            {
                advance();
                tokens.push_back(Token(Token::Type::PlusEqual, "+="));
            }
            else
            {
                tokens.push_back(Token(Token::Type::Plus, "+"));
            }
        }
        else if (m_currentChar == '-')
        {
            advance();
            if (m_currentChar == '-')
            {
                advance();
                tokens.push_back(Token(Token::Type::Decrement, "--"));
            }
            else if (m_currentChar == '=')
            {
                advance();
                tokens.push_back(Token(Token::Type::MinusEqual, "-="));
            }
            else
            {
                tokens.push_back(Token(Token::Type::Minus, "-"));
            }
        }
        else if (m_currentChar == '*')
        {
            tokens.push_back(Token(Token::Type::Multiply, "*"));
            advance();
        }
        else if (m_currentChar == '/')
        {
            advance();
            if (m_currentChar == '/')
            {
                // Comment
                while (m_currentChar != '\n' && m_currentChar != '\0')
                {
                    advance();
                }
            }
            else if (m_currentChar == '*')
            {
                // Comment
                advance();
                while (m_currentChar != '\0')
                {
                    if (m_currentChar == '*' && m_source[m_position.GetIndex() + 1] == '/')
                    {
                        advance();
                        advance();
                        break;
                    }
                    advance();
                }
            }
            else
            {
                tokens.push_back(Token(Token::Type::Divide, "/"));
            }
        }
        else if (m_currentChar == '^')
        {
            tokens.push_back(Token(Token::Type::Power, "^"));
            advance();
        }
        else if (m_currentChar == '(')
        {
            tokens.push_back(Token(Token::Type::LeftParenthesis, "("));
            advance();
        }
        else if (m_currentChar == ')')
        {
            tokens.push_back(Token(Token::Type::RightParenthesis, ")"));
            advance();
        }
        else if (m_currentChar == '{')
        {
            tokens.push_back(Token(Token::Type::LeftBrace, "{"));
            advance();
        }
        else if (m_currentChar == '}')
        {
            tokens.push_back(Token(Token::Type::RightBrace, "}"));
            advance();
        }
        else if (m_currentChar == '[')
        {
            tokens.push_back(Token(Token::Type::LeftBracket, "["));
            advance();
        }
        else if (m_currentChar == ']')
        {
            tokens.push_back(Token(Token::Type::RightBracket, "]"));
            advance();
        }
        else if (m_currentChar == ';')
        {
            tokens.push_back(Token(Token::Type::Semicolon, ";"));
            advance();
        }
        else if (m_currentChar == '=')
        {
            advance();
            if (m_currentChar == '=')
            {
                advance();
                tokens.push_back(Token(Token::Type::IsEqual, "=="));
            }
            else
            {
                tokens.push_back(Token(Token::Type::Equals, "="));
            }
        }
        else if (m_currentChar == '!')
        {
            advance();
            if (m_currentChar == '=')
            {
                advance();
                tokens.push_back(Token(Token::Type::IsNotEqual, "!="));
            }
            else
            {
                tokens.push_back(Token(Token::Type::LogicalNot, "!"));
            }
        }
        else if (m_currentChar == '&')
        {
            advance();
            if (m_currentChar == '&')
            {
                advance();
                tokens.push_back(Token(Token::Type::ConditionalAnd, "&&"));
            }
            else
            {
                Error error("Unexpected character '" + std::string(1, m_currentChar) + "'", m_position);
                std::cout << error.ToString() << "'\n";
                return std::vector<Token>();
            }
        }
        else if (m_currentChar == '|')
        {
            advance();
            if (m_currentChar == '|')
            {
                advance();
                tokens.push_back(Token(Token::Type::ConditionalOr, "||"));
            }
            else
            {
                Error error("Unexpected character '" + std::string(1, m_currentChar) + "'", m_position);
                std::cout << error.ToString() << "'\n";
                return std::vector<Token>();
            }
        }
        else if (m_currentChar == '>')
        {
            advance();
            if (m_currentChar == '=')
            {
                advance();
                tokens.push_back(Token(Token::Type::GreaterThanOrEqual, ">="));
            }
            else
            {
                tokens.push_back(Token(Token::Type::GreaterThan, ">"));
            }
        }
        else if (m_currentChar == '<')
        {
            advance();
            if (m_currentChar == '=')
            {
                advance();
                tokens.push_back(Token(Token::Type::LessThanOrEqual, "<="));
            }
            else
            {
                tokens.push_back(Token(Token::Type::LessThan, "<"));
            }
        }
        else if (m_currentChar == ',')
        {
            tokens.push_back(Token(Token::Type::Comma, ","));
            advance();
        }
        else if (m_currentChar == '.')
        {
            tokens.push_back(Token(Token::Type::Dot, "."));
            advance();
        }
        else if (m_currentChar == '\0')
        {
            break;
        }
        else
        {
            Error error("Unexpected character '" + std::string(1, m_currentChar) + "'", m_position);
            std::cout << error.ToString() << "'\n";
            return std::vector<Token>();
        }
    }
    tokens.push_back(Token(Token::Type::EndOfFile, ""));
    return tokens;
}