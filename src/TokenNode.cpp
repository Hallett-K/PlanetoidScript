#include "TokenNode.hpp"

TokenNode::TokenNode(Token token, NodeType type)
    : m_token(token), m_type(type)
{
}

ArrayNode::ArrayNode(Token token, std::vector<TokenNode*> array)
    : TokenNode(token, NodeType::Array), m_array(array)
{
}

std::vector<TokenNode*> ArrayNode::GetArray() const
{
    return m_array;
}

BinaryOperationNode::BinaryOperationNode(Token token, TokenNode* left, TokenNode* right)
    : TokenNode(token, NodeType::BinaryOperation), m_left(left), m_right(right)
{
}

TokenNode* BinaryOperationNode::GetLeft() const
{
    return m_left;
}

TokenNode* BinaryOperationNode::GetRight() const
{
    return m_right;
}

UnaryOperationNode::UnaryOperationNode(Token token, TokenNode* right)
    : TokenNode(token, NodeType::UnaryOperation), m_right(right)
{
}

TokenNode* UnaryOperationNode::GetRight() const
{
    return m_right;
}

VariableAssignmentNode::VariableAssignmentNode(Token token, Token object, TokenNode* right)
    : TokenNode(token, NodeType::VarAssign), m_object(object), m_right(right)
{
}

Token VariableAssignmentNode::GetObject() const
{
    return m_object;
}

TokenNode* VariableAssignmentNode::GetRight() const
{
    return m_right;
}

VarRetrievalNode::VarRetrievalNode(Token token, Token object)
    : TokenNode(token, NodeType::Identifier), m_object(object)
{
}

Token VarRetrievalNode::GetObject() const
{
    return m_object;
}

IfNode::IfNode(Token token, TokenNode* condition, TokenNode* ifBlock, TokenNode* elseBlock)
    : TokenNode(token, NodeType::If), m_condition(condition), m_ifBlock(ifBlock), m_elseBlock(elseBlock)
{
}

TokenNode* IfNode::GetCondition() const
{
    return m_condition;
}

TokenNode* IfNode::GetIfBlock() const
{
    return m_ifBlock;
}

TokenNode* IfNode::GetElseBlock() const
{
    return m_elseBlock;
}

WhileNode::WhileNode(Token token, TokenNode* condition, TokenNode* block, bool isDoWhile)
    : TokenNode(token, NodeType::While), m_condition(condition), m_block(block), m_isDoWhile(isDoWhile)
{
}

TokenNode* WhileNode::GetCondition() const
{
    return m_condition;
}

TokenNode* WhileNode::GetBlock() const
{
    return m_block;
}

bool WhileNode::IsDoWhile() const
{
    return m_isDoWhile;
}

SequenceNode::SequenceNode(Token token, std::vector<TokenNode*> nodes)
    : TokenNode(token, NodeType::Sequence), m_nodes(nodes)
{
}

std::vector<TokenNode*> SequenceNode::GetNodes() const
{
    return m_nodes;
}

void SequenceNode::AddNode(TokenNode* node)
{
    m_nodes.push_back(node);
}

ForNode::ForNode(Token token, TokenNode* init, TokenNode* condition, TokenNode* increment, TokenNode* block)
    : TokenNode(token, NodeType::For), m_init(init), m_condition(condition), m_increment(increment), m_block(block)
{
}

TokenNode* ForNode::GetInit() const
{
    return m_init;
}

TokenNode* ForNode::GetCondition() const
{
    return m_condition;
}

TokenNode* ForNode::GetIncrement() const
{
    return m_increment;
}

TokenNode* ForNode::GetBlock() const
{
    return m_block;
}

ForEachNode::ForEachNode(Token token, TokenNode* init, TokenNode* block)
    : TokenNode(token, NodeType::ForEach), m_array(init), m_block(block)
{
}

TokenNode* ForEachNode::GetArray() const
{
    return m_array;
}

TokenNode* ForEachNode::GetBlock() const
{
    return m_block;
}

ArrayAccessNode::ArrayAccessNode(Token token, Token object, TokenNode* index)
    : TokenNode(token, NodeType::ArrayAccess), m_object(object), m_index(index)
{
};

Token ArrayAccessNode::GetObject() const
{
    return m_object;
}

TokenNode* ArrayAccessNode::GetIndex() const
{
    return m_index;
}

ArrayAssignmentNode::ArrayAssignmentNode(Token token, Token object, TokenNode* index, TokenNode* right)
    : TokenNode(token, NodeType::ArrayAssign), m_object(object), m_index(index), m_value(right)
{
}

Token ArrayAssignmentNode::GetObject() const
{
    return m_object;
}

TokenNode* ArrayAssignmentNode::GetIndex() const
{
    return m_index;
}

TokenNode* ArrayAssignmentNode::GetValue() const
{
    return m_value;
}

ArgumentListNode::ArgumentListNode(Token token, std::vector<TokenNode*> args)
    : TokenNode(token, NodeType::ArgumentList), m_arguments(args)
{
}

std::vector<TokenNode*> ArgumentListNode::GetArguments() const
{
    return m_arguments;
}

FunctionCallNode::FunctionCallNode(Token token, Token object, TokenNode* args)
    : TokenNode(token, NodeType::FunctionCall), m_object(object), m_arguments(args)
{
}

Token FunctionCallNode::GetObject() const
{
    return m_object;
}

TokenNode* FunctionCallNode::GetArguments() const
{
    return m_arguments;
}

ArrayInitNode::ArrayInitNode(Token token, Token object, std::vector<TokenNode*> args)
    : TokenNode(token, NodeType::ArrayInit), m_object(object), m_elements(args)
{
}

Token ArrayInitNode::GetObject() const
{
    return m_object;
}

std::vector<TokenNode*> ArrayInitNode::GetElements() const
{
    return m_elements;
}

ReturnNode::ReturnNode(Token token, TokenNode* value)
    : TokenNode(token, NodeType::Return), m_value(value)
{
}

TokenNode* ReturnNode::GetValue() const
{
    return m_value;
}

FunctionDefinitionNode::FunctionDefinitionNode(Token token, Token object, TokenNode* block)
    : TokenNode(token, NodeType::FunctionDefinition), m_object(object), m_block(block)
{
}

Token FunctionDefinitionNode::GetObject() const
{
    return m_object;
}

TokenNode* FunctionDefinitionNode::GetBlock() const
{
    return m_block;
}

ObjectDefinitionNode::ObjectDefinitionNode(Token token, TokenNode* block, TokenNode* parent)
    : TokenNode(token, NodeType::ObjectDefinition), m_block(block), m_parent(parent)
{
}

TokenNode* ObjectDefinitionNode::GetBlock() const
{
    return m_block;
}

TokenNode* ObjectDefinitionNode::GetParent() const
{
    return m_parent;
}

ImportNode::ImportNode(Token token)
    : TokenNode(token, NodeType::Import)
{
}