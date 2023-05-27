#pragma once

#include "ApplicationState.hpp"
#include "TokenNode.hpp"

class SymbolTable;
class Value;

class Lexer;
class Parser;

class Interpreter
{
public:
    Interpreter();
    ~Interpreter();

    ApplicationState& GetState();

    Value Interpret(TokenNode* node);
    Value InterpretSequence(TokenNode* node);
    Value InterpretNumber(TokenNode* node);
    Value InterpretString(TokenNode* node);
    Value InterpretArray(TokenNode* node);
    Value InterpretBinaryOperation(TokenNode* node);
    Value InterpretUnaryOperation(TokenNode* node);
    Value InterpretVarAssign(TokenNode* node);
    Value InterpretIdentifier(TokenNode* node);
    Value InterpretIf(TokenNode* node);
    Value InterpretWhile(TokenNode* node);
    Value InterpretFor(TokenNode* node);
    Value InterpretForEach(TokenNode* node);
    Value InterpretArrayAccess(TokenNode* node);
    Value InterpretArrayInit(TokenNode* node);
    Value InterpretArrayAssign(TokenNode* node);
    Value InterpretFunctionCall(TokenNode* node, SymbolTable* module = nullptr);
    Value InterpretBreak(TokenNode* node);
    Value InterpretContinue(TokenNode* node);
    Value InterpretReturn(TokenNode* node);
    Value InterpretFunctionDefinition(TokenNode* node);
    Value InterpretObjectDefinition(TokenNode* node);
    Value InterpretImport(TokenNode* node);

    void SetCurrentDirectory(const std::string& directory);

    void Reset();
private:
    SymbolTable* m_currentSymbolTable;
    ApplicationState m_state;
};