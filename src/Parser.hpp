#pragma once

#include <vector>

#include "Token.hpp"
#include "TokenNode.hpp"

class Parser
{
public:
    Parser(const std::vector<Token>& tokens);
    ~Parser();

    TokenNode* Parse();
private:
    std::vector<Token> m_tokens;
    unsigned int m_index;

    void advance();
    void recede();

    TokenNode* parseLines(bool isBlock);
    TokenNode* parseLine();
    TokenNode* parseComparisonSequence();
    TokenNode* parseComparison();
    TokenNode* parseExpression();
    TokenNode* parseTerm();
    TokenNode* parsePrimary();
    TokenNode* parseFactor();
    TokenNode* parseIfElse();
    TokenNode* parseWhile();
    TokenNode* parseDoWhile();
    TokenNode* parseFor();
    TokenNode* parseForEach();

    TokenNode* parseIdentifier(Token object = Token(Token::Type::None, ""));

    TokenNode* parseArrayIndex(Token object = Token(Token::Type::None, ""));
    TokenNode* parseArrayInit(Token object = Token(Token::Type::None, ""));

    TokenNode* parseFunctionCall(Token object = Token(Token::Type::None, ""));
    TokenNode* parseArgumentList();

    TokenNode* parseFunctionDefinition(Token object = Token(Token::Type::None, ""));
    TokenNode* parseObjectDefinition(Token object = Token(Token::Type::None, ""));
    TokenNode* parseObjectIdentifiers();
};