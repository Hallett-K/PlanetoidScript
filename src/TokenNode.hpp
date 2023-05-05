#pragma once

#include <vector>
#include "Token.hpp"

enum class NodeType
{
    Sequence,
    BinaryOperation,
    UnaryOperation,
    Number,
    String,
    Array, // [ ... ]
    VarAssign,
    Identifier,
    If,
    While,
    For,
    ForEach,
    ArrayAccess, // array identifier
    ArrayInit, // array = [ ... ]
    ArrayAssign, // array[index] = value
    ArgumentList,
    FunctionCall,
    Break,
    Continue,
    Return,
    FunctionDefinition,
    ObjectDefinition,
    ObjectAssign,
};

class TokenNode
{
public:
    TokenNode(Token token, NodeType type = NodeType::Number);
    virtual ~TokenNode() = default;

    Token GetToken() const { return m_token; }
    NodeType GetType() const { return m_type; }
protected:
    Token m_token;
    NodeType m_type;
};

class ArrayNode : public TokenNode
{
public:
    ArrayNode(Token token, std::vector<TokenNode*> array);
    virtual ~ArrayNode() = default;

    std::vector<TokenNode*> GetArray() const;
private:
    std::vector<TokenNode*> m_array;
};

class BinaryOperationNode : public TokenNode
{
public:
    BinaryOperationNode(Token token, TokenNode* left, TokenNode* right);
    virtual ~BinaryOperationNode() = default;

    TokenNode* GetLeft() const;
    TokenNode* GetRight() const;
private:
    TokenNode* m_left;
    TokenNode* m_right;
};

class UnaryOperationNode : public TokenNode
{
public:
    UnaryOperationNode(Token token, TokenNode* right);
    virtual ~UnaryOperationNode() = default;

    TokenNode* GetRight() const;
private:
    TokenNode* m_right;
};

class VariableAssignmentNode : public TokenNode
{
public:
    VariableAssignmentNode(Token token, Token object, TokenNode* right);
    virtual ~VariableAssignmentNode() = default;

    Token GetObject() const;
    TokenNode* GetRight() const;
private:
    Token m_object;
    TokenNode* m_right;
};

class VarRetrievalNode : public TokenNode
{
public:
    VarRetrievalNode(Token token, Token object);
    virtual ~VarRetrievalNode() = default;

    Token GetObject() const;
private:
    Token m_object;
};

class IfNode : public TokenNode
{
public:
    IfNode(Token token, TokenNode* condition, TokenNode* ifBlock, TokenNode* elseBlock);
    virtual ~IfNode() = default;

    TokenNode* GetCondition() const;
    TokenNode* GetIfBlock() const;
    TokenNode* GetElseBlock() const;
private:
    TokenNode* m_condition;
    TokenNode* m_ifBlock;
    TokenNode* m_elseBlock;
};

class WhileNode : public TokenNode
{
public:
    WhileNode(Token token, TokenNode* condition, TokenNode* block, bool isDoWhile);
    virtual ~WhileNode() = default;

    TokenNode* GetCondition() const;
    TokenNode* GetBlock() const;
    bool IsDoWhile() const;
private:
    TokenNode* m_condition;
    TokenNode* m_block;
    bool m_isDoWhile;
};

class SequenceNode : public TokenNode
{
public:
    SequenceNode(Token token, std::vector<TokenNode*> nodes);
    virtual ~SequenceNode() = default;

    std::vector<TokenNode*> GetNodes() const;
    void AddNode(TokenNode* node);
private:
    std::vector<TokenNode*> m_nodes;
};

class ForNode : public TokenNode
{
public:
    ForNode(Token token, TokenNode* init, TokenNode* condition, TokenNode* increment, TokenNode* block);
    virtual ~ForNode() = default;

    TokenNode* GetInit() const;
    TokenNode* GetCondition() const;
    TokenNode* GetIncrement() const;
    TokenNode* GetBlock() const;
private:
    TokenNode* m_init;
    TokenNode* m_condition;
    TokenNode* m_increment;
    TokenNode* m_block;
};

class ForEachNode : public TokenNode
{
public:
    ForEachNode(Token token, TokenNode* array, TokenNode* block);
    virtual ~ForEachNode() = default;

    TokenNode* GetArray() const;
    TokenNode* GetBlock() const;
private:
    TokenNode* m_array;
    TokenNode* m_block;
};

class ArrayAccessNode : public TokenNode
{
public:
    ArrayAccessNode(Token token, Token object, TokenNode* index);
    virtual ~ArrayAccessNode() = default;

    Token GetObject() const;
    TokenNode* GetIndex() const;
private:
    Token m_object;
    TokenNode* m_index;
};

class ArrayInitNode : public TokenNode
{
public:
    ArrayInitNode(Token token, Token object, std::vector<TokenNode*> elements);
    virtual ~ArrayInitNode() = default;

    Token GetObject() const;
    std::vector<TokenNode*> GetElements() const;
private:
    Token m_object;
    std::vector<TokenNode*> m_elements;
};

class ArrayAssignmentNode : public TokenNode
{
public:
    ArrayAssignmentNode(Token token, Token object, TokenNode* index, TokenNode* value);
    virtual ~ArrayAssignmentNode() = default;

    Token GetObject() const;
    TokenNode* GetIndex() const;
    TokenNode* GetValue() const;
private:
    Token m_object;
    TokenNode* m_index;
    TokenNode* m_value;
};

class ArgumentListNode : public TokenNode
{
public:
    ArgumentListNode(Token token, std::vector<TokenNode*> arguments);
    virtual ~ArgumentListNode() = default;

    std::vector<TokenNode*> GetArguments() const;
private:
    std::vector<TokenNode*> m_arguments;
};

class FunctionCallNode : public TokenNode
{
public:
    FunctionCallNode(Token token, Token object, TokenNode* arguments);
    virtual ~FunctionCallNode() = default;

    Token GetObject() const;
    TokenNode* GetArguments() const;
private:
    Token m_object;
    TokenNode* m_arguments;
};

class ReturnNode : public TokenNode
{
public:
    ReturnNode(Token token, TokenNode* value);
    virtual ~ReturnNode() = default;

    TokenNode* GetValue() const;
private:
    TokenNode* m_value;
};

class FunctionDefinitionNode : public TokenNode
{
public:
    FunctionDefinitionNode(Token token, Token object, TokenNode* block);
    virtual ~FunctionDefinitionNode() = default;

    Token GetObject() const;
    TokenNode* GetBlock() const;
private:
    Token m_object;
    TokenNode* m_block;
};

class ObjectDefinitionNode : public TokenNode
{
public:
    ObjectDefinitionNode(Token token, TokenNode* block, TokenNode* parent = nullptr);
    virtual ~ObjectDefinitionNode() = default;

    TokenNode* GetBlock() const;
    TokenNode* GetParent() const;

private:
    TokenNode* m_block;
    TokenNode* m_parent;
};