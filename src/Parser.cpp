#include "Parser.hpp"

#include <iostream>
#include "Error.hpp"

Parser::Parser(const std::vector<Token>& tokens)
    : m_tokens(tokens), m_index(0)
{
}

Parser::~Parser()
{
}

TokenNode* Parser::Parse()
{
    return parseLines(false);
}

void Parser::advance()
{
    m_index++;
}

void Parser::recede()
{
    m_index--;
}

TokenNode* Parser::parseLines(bool isBlock)
{
    unsigned int numOpenBraces = isBlock ? 1 : 0;
    SequenceNode* node = new SequenceNode(Token(Token::Type::None, ""), {});

    while (m_tokens[m_index].GetType() != Token::Type::EndOfFile)
    {
        if (m_tokens[m_index].GetType() == Token::Type::LeftBrace)
        {
            numOpenBraces++;
        }
        else if (m_tokens[m_index].GetType() == Token::Type::RightBrace)
        {
            numOpenBraces--;
            if (numOpenBraces < 0)
            {
                Error e("Unexpected '}'", Position("Parser", 0, 0, m_index));
                std::cout << e.ToString() << '\n';
                return NULL;
            }
            else if (numOpenBraces == 0 && isBlock)
            {
                return node;
            }
        }

        TokenNode* line = parseLine();
        if (line)
        {
            node->AddNode(line);
        }
        else
        {
            return NULL;
        }
    }

    return node;
}

TokenNode* Parser::parseLine()
{
    TokenNode* node = parseComparisonSequence();
    if (m_tokens[m_index].GetType() == Token::Type::Semicolon)
    {
        advance();
        return node;
    }
    else
    {
        Error e("Expected ';'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
}

TokenNode* Parser::parseComparisonSequence()
{
    TokenNode* left = parseComparison();
    while (m_tokens[m_index].GetType() == Token::Type::ConditionalAnd || m_tokens[m_index].GetType() == Token::Type::ConditionalOr)
    {
        Token op = m_tokens[m_index];
        advance();
        TokenNode* right = parseComparison();
        left = new BinaryOperationNode(op, left, right);
    }

    return left;
}

TokenNode* Parser::parseComparison()
{
    // (!) Expression ([== | != | < | > | <= | >=] Expression)
    // Not
    bool isNot = false;
    if (m_tokens[m_index].GetType() == Token::Type::LogicalNot)
    {
        if (m_tokens[m_index + 1].GetType() != Token::Type::LeftParenthesis)
        {
            Error e("Expected '(' after '!'", Position("Parser", 0, 0, m_index));
            std::cout << e.ToString() << '\n';
            return NULL;
        }
        isNot = true;
        advance();
    }
    TokenNode* left = parseExpression();

    // Comparison
    while (m_tokens[m_index].GetType() == Token::Type::IsEqual || m_tokens[m_index].GetType() == Token::Type::IsNotEqual ||
        m_tokens[m_index].GetType() == Token::Type::LessThan || m_tokens[m_index].GetType() == Token::Type::GreaterThan ||
        m_tokens[m_index].GetType() == Token::Type::LessThanOrEqual || m_tokens[m_index].GetType() == Token::Type::GreaterThanOrEqual)
    {
        Token op = m_tokens[m_index];
        advance();
        TokenNode* right = parseExpression();
        left = new BinaryOperationNode(op, left, right);
    }

    if (isNot)
    {
        left = new UnaryOperationNode(Token(Token::Type::LogicalNot, "!"), left);
    }
    
    return left;
}

TokenNode* Parser::parseExpression()
{
    TokenNode* left = parseTerm();

    while (m_tokens[m_index].GetType() == Token::Type::Plus || m_tokens[m_index].GetType() == Token::Type::Minus)
    {
        Token op = m_tokens[m_index];
        advance();
        TokenNode* right = parseTerm();
        left = new BinaryOperationNode(op, left, right);
    }

    return left;
}

TokenNode* Parser::parseTerm()
{
    TokenNode* left = parsePrimary();
    while (m_tokens[m_index].GetType() == Token::Type::Multiply || m_tokens[m_index].GetType() == Token::Type::Divide)
    {
        Token op = m_tokens[m_index];
        advance();
        TokenNode* right = parsePrimary();
        left = new BinaryOperationNode(op, left, right);
    }

    return left;
}

TokenNode* Parser::parsePrimary()
{
    TokenNode* left = parseFactor();
    while (m_tokens[m_index].GetType() == Token::Type::Power)
    {
        Token op = m_tokens[m_index];
        advance();
        TokenNode* right = parseFactor();
        left = new BinaryOperationNode(op, left, right);
    }

    return left;
}

TokenNode* Parser::parseFactor()
{
    if (m_tokens[m_index].GetType() == Token::Type::Number)
    {
        advance();
        return new TokenNode(m_tokens[m_index-1], NodeType::Number);   
    }
    else if (m_tokens[m_index].GetType() == Token::Type::String)
    {
        advance();
        return new TokenNode(m_tokens[m_index-1], NodeType::String);
    }
    else if (m_tokens[m_index].GetType() == Token::Type::LeftParenthesis)
    {
        advance();
        TokenNode* node = parseComparisonSequence();
        if (m_tokens[m_index].GetType() == Token::Type::RightParenthesis)
        {
            advance();
            return node;
        }
        else
        {
            Error e("Expected ')'", Position("Parser", 0, 0, m_index));
            std::cout << e.ToString() << '\n';
            return NULL;
        }
    }
    else if (m_tokens[m_index].GetType() == Token::Type::Plus || m_tokens[m_index].GetType() == Token::Type::Minus)
    {
        Token op = m_tokens[m_index];
        advance();
        TokenNode* right = parseFactor();
        return new UnaryOperationNode(op, right);
    }
    else if (m_tokens[m_index].GetType() == Token::Type::LeftBracket)
    {
        advance();
        ArgumentListNode* vals = dynamic_cast<ArgumentListNode*>(parseArrayInit());
        return new ArrayNode(Token(Token::Type::None, ""), vals->GetArguments());
    }
    else if (m_tokens[m_index].GetType() == Token::Type::Identifier)
    {
        return parseIdentifier();
    }
    else if (m_tokens[m_index].GetType() == Token::Type::Keyword)
    {
        if (m_tokens[m_index].GetValue() == "if")
        {
            return parseIfElse();
        }
        else if (m_tokens[m_index].GetValue() == "while")
        {
            return parseWhile();
        }
        else if (m_tokens[m_index].GetValue() == "do")
        {
            return parseDoWhile();
        }
        else if (m_tokens[m_index].GetValue() == "for")
        {
            return parseFor();
        }
        else if (m_tokens[m_index].GetValue() == "continue")
        {
            advance();
            return new TokenNode(Token(Token::Type::Keyword, "continue"), NodeType::Continue);
        }
        else if (m_tokens[m_index].GetValue() == "break")
        {
            advance();
            return new TokenNode(Token(Token::Type::Keyword, "break"), NodeType::Break);
        }
        else if (m_tokens[m_index].GetValue() == "return")
        {
            Token returnToken = m_tokens[m_index];
            advance();
            TokenNode* node;
            if (m_tokens[m_index].GetType() != Token::Type::Semicolon)
            {
                node = parseComparisonSequence();
            }
            
            return new ReturnNode(returnToken, node);
        }

        Error e("Unknown Keyword: ", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << m_tokens[m_index].ToString() << '\n';
        return NULL;
    }
    
    Error e("Unexpected token: ", Position("Parser", 0, 0, m_index));
    std::cout << e.ToString() << m_tokens[m_index].ToString() << '\n';
    return NULL;
}

TokenNode* Parser::parseIfElse()
{
    Token ifToken = m_tokens[m_index];
    advance();
    if (m_tokens[m_index].GetType() != Token::Type::LeftParenthesis)
    {
        Error e("Expected '('", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* condition = parseComparisonSequence();
    if (m_tokens[m_index].GetType() != Token::Type::RightParenthesis)
    {
        Error e("Expected ')'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    if (m_tokens[m_index].GetType() != Token::Type::LeftBrace)
    {
        Error e("Expected '{'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* ifBody = parseLines(true);
    if (m_tokens[m_index].GetType() != Token::Type::RightBrace)
    {
        Error e("Expected '}'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* elseBody = NULL;
    while (m_tokens[m_index].GetType() == Token::Type::Keyword && m_tokens[m_index].GetValue() == "else")
    {
        advance();
        // If
        if (m_tokens[m_index].GetType() == Token::Type::Keyword && m_tokens[m_index].GetValue() == "if")
        {
            elseBody = parseIfElse();
        }
        else
        {
            if (m_tokens[m_index].GetType() != Token::Type::LeftBrace)
            {
                Error e("Expected '{'", Position("Parser", 0, 0, m_index));
                std::cout << e.ToString() << '\n';
                return NULL;
            }
            advance();
            elseBody = parseLines(true);
            if (m_tokens[m_index].GetType() != Token::Type::RightBrace)
            {
                Error e("Expected '}'", Position("Parser", 0, 0, m_index));
                std::cout << e.ToString() << '\n';
                return NULL;
            }
            advance();
        }
    }
    return new IfNode(ifToken, condition, ifBody, elseBody);
}

TokenNode* Parser::parseWhile()
{
    Token whileToken = m_tokens[m_index];
    advance();
    if (m_tokens[m_index].GetType() != Token::Type::LeftParenthesis)
    {
        Error e("Expected '('", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* condition = parseComparisonSequence();
    if (m_tokens[m_index].GetType() != Token::Type::RightParenthesis)
    {
        Error e("Expected ')'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    if (m_tokens[m_index].GetType() == Token::Type::Keyword && m_tokens[m_index].GetValue() == "do")
    {
        advance();
    }

    if (m_tokens[m_index].GetType() != Token::Type::LeftBrace)
    {
        Error e("Expected '{'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* body = parseLines(true);
    if (m_tokens[m_index].GetType() != Token::Type::RightBrace)
    {
        Error e("Expected '}'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    return new WhileNode(whileToken, condition, body, false);
}

TokenNode* Parser::parseDoWhile()
{
    Token doToken = m_tokens[m_index];
    advance();
    if (m_tokens[m_index].GetType() != Token::Type::LeftBrace)
    {
        Error e("Expected '{'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* body = parseLines(true);
    if (m_tokens[m_index].GetType() != Token::Type::RightBrace)
    {
        Error e("Expected '}'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    if (m_tokens[m_index].GetType() != Token::Type::Keyword || m_tokens[m_index].GetValue() != "while")
    {
        Error e("Expected 'while'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    if (m_tokens[m_index].GetType() != Token::Type::LeftParenthesis)
    {
        Error e("Expected '('", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* condition = parseComparisonSequence();
    if (m_tokens[m_index].GetType() != Token::Type::RightParenthesis)
    {
        Error e("Expected ')'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    return new WhileNode(doToken, condition, body, true);
}

TokenNode* Parser::parseFor()
{
    Token forToken = m_tokens[m_index];
    advance();
    if (m_tokens[m_index].GetType() != Token::Type::LeftParenthesis)
    {
        Error e("Expected '('", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* init = parseFactor();
    if (m_tokens[m_index].GetType() != Token::Type::Semicolon)
    {
        Error e("Expected ';'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* condition = parseComparisonSequence();
    if (m_tokens[m_index].GetType() != Token::Type::Semicolon)
    {
        Error e("Expected ';'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* increment = parseExpression();
    if (m_tokens[m_index].GetType() != Token::Type::RightParenthesis)
    {
        Error e("Expected ')'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    if (m_tokens[m_index].GetType() != Token::Type::LeftBrace)
    {
        Error e("Expected '{'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* body = parseLines(true);
    if (m_tokens[m_index].GetType() != Token::Type::RightBrace)
    {
        Error e("Expected '}'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    return new ForNode(forToken, init, condition, increment, body);
}

TokenNode* Parser::parseIdentifier(Token object)
{
    Token var = m_tokens[m_index];
    advance();
    if (m_tokens[m_index].GetType() == Token::Type::Equals)
    {
        advance();
        if (m_tokens[m_index].GetType() == Token::Type::LeftBracket)
        {
            advance();
            ArgumentListNode* vals = dynamic_cast<ArgumentListNode*>(parseArrayInit());
            return new ArrayInitNode(var, object, vals->GetArguments());
        }
        else if (m_tokens[m_index].GetType() == Token::Type::Keyword && m_tokens[m_index].GetValue() == "func")
        {
            return parseFunctionDefinition();
        }
        else if (m_tokens[m_index].GetType() == Token::Type::Keyword && m_tokens[m_index].GetValue() == "object")
        {
            return parseObjectDefinition();
        }
        TokenNode* right = parseComparisonSequence();
        return new VariableAssignmentNode(var, object, right);
    }
    else if (m_tokens[m_index].GetType() == Token::Type::LeftBracket)
    {
        return parseArrayIndex();
    }
    else if (m_tokens[m_index].GetType() == Token::Type::LeftParenthesis)
    {
        return parseFunctionCall(object);
    }
    else if (m_tokens[m_index].GetType() == Token::Type::Dot)
    {
        advance();
        return parseIdentifier(var);
    }
    return new VarRetrievalNode(var, object);
}

TokenNode* Parser::parseArrayIndex(Token object)
{
    Token var = m_tokens[m_index - 1];
    advance();
    TokenNode* index = parseComparisonSequence();
    if (m_tokens[m_index].GetType() != Token::Type::RightBracket)
    {
        Error e("Expected ']'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    if (m_tokens[m_index].GetType() == Token::Type::Equals)
    {
        advance();
        TokenNode* right = parseComparisonSequence();
        return new ArrayAssignmentNode(var, object, index, right);
    }
    return new ArrayAccessNode(var, object, index);
}

TokenNode* Parser::parseArrayInit(Token object)
{
    std::vector<TokenNode*> args;
    while (m_tokens[m_index].GetType() != Token::Type::RightBracket)
    {
        TokenNode* arg = parseComparisonSequence();
        args.push_back(arg);
        if (m_tokens[m_index].GetType() != Token::Type::Comma && m_tokens[m_index].GetType() != Token::Type::RightBracket)
        {
            Error e("Expected ',' or ']'", Position("Parser", 0, 0, m_index));
            std::cout << e.ToString() << '\n';
            return NULL;
        }
        if (m_tokens[m_index].GetType() == Token::Type::Comma)
        {
            advance();
        }
    }
    advance();
    return new ArgumentListNode(Token(Token::Type::None, ""), args);
}

TokenNode* Parser::parseFunctionCall(Token object)
{
    Token func = m_tokens[m_index - 1];
    advance();
    TokenNode* args = parseArgumentList();
    return new FunctionCallNode(func, object, args);
}

TokenNode* Parser::parseArgumentList()
{
    std::vector<TokenNode*> args;
    while (m_tokens[m_index].GetType() != Token::Type::RightParenthesis)
    {
        TokenNode* arg = parseComparisonSequence();
        args.push_back(arg);
        if (m_tokens[m_index].GetType() != Token::Type::Comma && m_tokens[m_index].GetType() != Token::Type::RightParenthesis)
        {
            Error e("Expected ',' or ')'", Position("Parser", 0, 0, m_index));
            std::cout << e.ToString() << '\n';
            return NULL;
        }
        if (m_tokens[m_index].GetType() == Token::Type::Comma)
        {
            advance();
        }
    }
    advance();
    return new ArgumentListNode(Token(Token::Type::None, ""), args);
}

TokenNode* Parser::parseFunctionDefinition(Token object)
{
    Token func = m_tokens[m_index - 2];
    advance();
    if (m_tokens[m_index].GetType() != Token::Type::LeftBrace)
    {
        Error e("Expected '{'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* body = parseLines(true);
    if (m_tokens[m_index].GetType() != Token::Type::RightBrace)
    {
        Error e("Expected '}'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    return new FunctionDefinitionNode(func, object, body);
}

TokenNode* Parser::parseObjectDefinition(Token object)
{
    Token obj = m_tokens[m_index - 2];
    advance();
    if (m_tokens[m_index].GetType() != Token::Type::LeftBrace)
    {
        Error e("Expected '{'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    TokenNode* body = parseObjectIdentifiers();
    if (m_tokens[m_index].GetType() != Token::Type::RightBrace)
    {
        Error e("Expected '}'", Position("Parser", 0, 0, m_index));
        std::cout << e.ToString() << '\n';
        return NULL;
    }
    advance();
    return new ObjectDefinitionNode(obj, body);
}

TokenNode* Parser::parseObjectIdentifiers()
{
    unsigned int numOpenBraces = 1;
    SequenceNode* node = new SequenceNode(Token(Token::Type::None, ""), {});

    while (m_tokens[m_index].GetType() != Token::Type::EndOfFile)
    {
        if (m_tokens[m_index].GetType() == Token::Type::LeftBrace)
        {
            numOpenBraces++;
        }
        else if (m_tokens[m_index].GetType() == Token::Type::RightBrace)
        {
            numOpenBraces--;
            if (numOpenBraces == 0)
            {
                return node;
            }
        }

        TokenNode* line = parseIdentifier();
        if (line)
        {
            node->AddNode(line);
            if (m_tokens[m_index].GetType() != Token::Type::Semicolon)
            {
                Error e("Expected ';'", Position("Parser", 0, 0, m_index));
                std::cout << e.ToString() << '\n';
                return NULL;
            }
            advance();
        }
        else
        {
            return NULL;
        }
    }

    return node;
}