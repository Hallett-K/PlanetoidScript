#include "Interpreter.hpp"

#include <iostream>

#include "Error.hpp"
#include "SymbolTable.hpp"
#include "Value.hpp"

Interpreter::Interpreter()
{
    m_currentSymbolTable = &g_symbolTable;
}

Interpreter::~Interpreter()
{
}

Value Interpreter::Interpret(TokenNode* node)
{
    if (node->GetType() == NodeType::Sequence)
    {
        return InterpretSequence(node);
    }
    if (node->GetType() == NodeType::Number)
    {
        return InterpretNumber(node);
    }
    else if (node->GetType() == NodeType::String)
    {
        return InterpretString(node);
    }
    else if (node->GetType() == NodeType::Array)
    {
        return InterpretArray(node);
    }
    else if (node->GetType() == NodeType::BinaryOperation)
    {
        return InterpretBinaryOperation(node);
    }
    else if (node->GetType() == NodeType::UnaryOperation)
    {
        return InterpretUnaryOperation(node);
    }
    else if (node->GetType() == NodeType::VarAssign)
    {
        return InterpretVarAssign(node);
    }
    else if (node->GetType() == NodeType::Identifier)
    {
        return InterpretIdentifier(node);
    }
    else if (node->GetType() == NodeType::If)
    {
        return InterpretIf(node);
    }
    else if (node->GetType() == NodeType::While)
    {
        return InterpretWhile(node);
    }
    else if (node->GetType() == NodeType::For)
    {
        return InterpretFor(node);
    }
    else if (node->GetType() == NodeType::ArrayAccess)
    {
        return InterpretArrayAccess(node);
    }
    else if (node->GetType() == NodeType::ArrayInit)
    {
        return InterpretArrayInit(node);
    }
    else if (node->GetType() == NodeType::ArrayAssign)
    {
        return InterpretArrayAssign(node);
    }
    else if (node->GetType() == NodeType::FunctionCall)
    {
        return InterpretFunctionCall(node);
    }
    else if (node->GetType() == NodeType::Break)
    {
        return InterpretBreak(node);
    }
    else if (node->GetType() == NodeType::Continue)
    {
        return InterpretContinue(node);
    }
    else if (node->GetType() == NodeType::Return)
    {
        return InterpretReturn(node);
    }
    else if (node->GetType() == NodeType::FunctionDefinition)
    {
        return InterpretFunctionDefinition(node);
    }
    else if (node->GetType() == NodeType::ObjectDefinition)
    {
        return InterpretObjectDefinition(node);
    }
    else
    {
        Error e("Unknown Node Type: ", Position("Interpreter", 0, 0, 0));
        std::cout << e.ToString() << (int)node->GetType() << '\n';
        return Value();
    }
}

Value Interpreter::InterpretSequence(TokenNode* node)
{
    SequenceNode* seqNode = dynamic_cast<SequenceNode*>(node);
    Value result = 0;
    for (int i = 0; i < seqNode->GetNodes().size(); i++)
    {
        result = Interpret(seqNode->GetNodes()[i]);

        if (m_state.hasError)
        {
            break;
        }
        if ((m_state.canBreak && m_state.breakCalled) || (m_state.canContinue && m_state.continueCalled) || (m_state.canReturn && m_state.returnCalled))
        {
            break;
        }
    }
    return result;
}

Value Interpreter::InterpretNumber(TokenNode* node)
{
    if (node->GetToken().GetValue() == "")
        return Value();
    return Value(std::stof(node->GetToken().GetValue()));
}

Value Interpreter::InterpretString(TokenNode* node)
{
    return Value(node->GetToken().GetValue());
}

Value Interpreter::InterpretArray(TokenNode* node)
{
    ArrayNode* arrayNode = dynamic_cast<ArrayNode*>(node);
    std::vector<Value> values;
    for (int i = 0; i < arrayNode->GetArray().size(); i++)
    {
        values.push_back(Interpret(arrayNode->GetArray()[i]));
    }
    
    Value value(values);
    return value;
}

Value Interpreter::InterpretBinaryOperation(TokenNode* node)
{
    BinaryOperationNode* binOpNode = dynamic_cast<BinaryOperationNode*>(node);
    Value left = Interpret(binOpNode->GetLeft());
    Value right = Interpret(binOpNode->GetRight());
    Token token = binOpNode->GetToken();
    if (token.GetType() == Token::Type::Plus)
    {
        return left + right;
    }
    else if (token.GetType() == Token::Type::Minus)
    {
        return left - right;
    }
    else if (token.GetType() == Token::Type::Multiply)
    {
        return left * right;
    }
    else if (token.GetType() == Token::Type::Divide)
    {
        if (right.getNumber() == 0 || !right.isNumber())
        {
            Error e("Division by zero: ", Position("", 0, 0, 0));
            std::cout << e.ToString() << '\n';
            return Value();
        }
        return left / right;
    }
    else if (token.GetType() == Token::Type::Power)
    {
        return left ^ right;
    }
    else if (token.GetType() == Token::Type::IsEqual)
    {
        return left == right;
    }
    else if (token.GetType() == Token::Type::IsNotEqual)
    {
        return left != right;
    }
    else if (token.GetType() == Token::Type::ConditionalAnd)
    {
        return left != 0 && right != 0;
    }
    else if (token.GetType() == Token::Type::ConditionalOr)
    {
        return left != 0 || right != 0;
    }
    else if (token.GetType() == Token::Type::GreaterThan)
    {
        return left > right;
    }
    else if (token.GetType() == Token::Type::GreaterThanOrEqual)
    {
        return left >= right;
    }
    else if (token.GetType() == Token::Type::LessThan)
    {
        return left < right;
    }
    else if (token.GetType() == Token::Type::LessThanOrEqual)
    {
        return left <= right;
    }
    else
    {
        Error e("Unknown Binary Operation: ", Position("", 0, 0, 0));
        std::cout << e.ToString() << (int)token.GetType() << '\n';
        return Value();
    }
}

Value Interpreter::InterpretUnaryOperation(TokenNode* node)
{
    UnaryOperationNode* unOpNode = dynamic_cast<UnaryOperationNode*>(node);
    Value right = Interpret(unOpNode->GetRight());
    Token token = unOpNode->GetToken();
    if (token.GetType() == Token::Type::Plus)
    {
        return right;
    }
    else if (token.GetType() == Token::Type::Minus)
    {
        return -right;
    }
    else if (token.GetType() == Token::Type::LogicalNot)
    {
        return right == 0;
    }
    else
    {
        Error e("Unknown Unary Operation: ", Position("", 0, 0, 0));
        std::cout << e.ToString() << (int)token.GetType() << '\n';
        return Value();
    }
}

Value Interpreter::InterpretVarAssign(TokenNode* node)
{
    VariableAssignmentNode* varAssignNode = dynamic_cast<VariableAssignmentNode*>(node);
    Token token = varAssignNode->GetToken();

    if (varAssignNode->GetRight()->GetType() == NodeType::FunctionCall)
    {
        FunctionCallNode* funcCallNode = dynamic_cast<FunctionCallNode*>(varAssignNode->GetRight());
        Token obj = funcCallNode->GetToken();
        if (m_currentSymbolTable->ObjectExists(obj.GetValue(), true))
        {
            m_currentSymbolTable->AddObjectInstance(token.GetValue(), obj.GetValue());

            // Call init function
            std::string instScopeName = m_currentSymbolTable->GetObjectInstanceScopeName(obj.GetValue());
            SymbolTable* instScope = m_currentSymbolTable->GetScope(instScopeName);
            SymbolTable* currentScope = m_currentSymbolTable;

            if (instScope->IsUserFunction("init"))
            {
                FunctionCallNode initFuncCallNode(Token(Token::Type::Identifier, "init"), token, funcCallNode->GetArguments());
                InterpretFunctionCall(&initFuncCallNode);
            }

            return Value();
        }
    }
    Value right = Interpret(varAssignNode->GetRight());
    

    if (varAssignNode->GetObject().GetType() == Token::Type::Identifier)
    {
        Token object = varAssignNode->GetObject();
        if (m_currentSymbolTable->ObjectInstanceExists(object.GetValue()))
        {
            std::string instanceName = m_currentSymbolTable->GetObjectInstanceScopeName(object.GetValue());
            if (!m_currentSymbolTable->GetScope(instanceName)->VarExists(token.GetValue()))
            {
                Error e("Unknown Variable: ", Position("Interpreter", 0, 0, 0));
                std::cout << e.ToString() << token.GetValue() << '\n';
                return Value();
            }
            m_currentSymbolTable->GetScope(instanceName)->RegisterVar(token.GetValue(), right);
        }
        else
        {
            Error e("Unknown Object Instance: ", Position("", 0, 0, 0));
            std::cout << e.ToString() << object.GetValue() << '\n';
            return Value();
        }
    }
    else
    {
        m_currentSymbolTable->RegisterVar(token.GetValue(), right);
    }

    return right;
}

Value Interpreter::InterpretIdentifier(TokenNode* node)
{
    VarRetrievalNode* varRetNode = dynamic_cast<VarRetrievalNode*>(node);
    Token token = varRetNode->GetToken();
    SymbolTable* symbolTable = m_currentSymbolTable;

    if (varRetNode->GetObject().GetType() == Token::Type::Identifier)
    {
        Token object = varRetNode->GetObject();
        if (m_currentSymbolTable->ObjectInstanceExists(object.GetValue()))
        {
            std::string instanceName = m_currentSymbolTable->GetObjectInstanceScopeName(object.GetValue());
            symbolTable = m_currentSymbolTable->GetScope(instanceName);
        }
        else
        {
            Error e("Unknown Object Instance: ", Position("", 0, 0, 0));
            std::cout << e.ToString() << object.GetValue() << '\n';
            return Value();
        }
    }

    if (symbolTable->VarExists(token.GetValue(), true))
    {
        return symbolTable->GetVar(token.GetValue(), true);
    }

    Error e("Unknown Identifier: ", Position("Interpreter", 0, 0, 0));
    std::cout << e.ToString() << token.GetValue() << '\n';
    return Value();
}

Value Interpreter::InterpretIf(TokenNode* node)
{
    IfNode* ifNode = dynamic_cast<IfNode*>(node);
    Value condition = Interpret(ifNode->GetCondition());
    if (condition != 0)
    {
        unsigned int scopeCount = m_currentSymbolTable->GetScopeCount();
        m_currentSymbolTable->AddScope("IfBlock" + std::to_string(scopeCount));
        m_currentSymbolTable = m_currentSymbolTable->GetScope("IfBlock" + std::to_string(scopeCount));

        Value n = Interpret(ifNode->GetIfBlock());
        m_currentSymbolTable = m_currentSymbolTable->GetParentScope();
        m_currentSymbolTable->RemoveScope("IfBlock" + std::to_string(scopeCount));

        return n;
    }
    else
    {
        if (ifNode->GetElseBlock() != nullptr)
        {
            unsigned int scopeCount = m_currentSymbolTable->GetScopeCount();
            m_currentSymbolTable->AddScope("ElseBlock" + std::to_string(scopeCount));
            m_currentSymbolTable = m_currentSymbolTable->GetScope("ElseBlock" + std::to_string(scopeCount));

            Value n = Interpret(ifNode->GetElseBlock());
            m_currentSymbolTable = m_currentSymbolTable->GetParentScope();
            m_currentSymbolTable->RemoveScope("ElseBlock" + std::to_string(scopeCount));

            return n;
        }
        return Value();
    }
}

Value Interpreter::InterpretWhile(TokenNode* node)
{
    WhileNode* whileNode = dynamic_cast<WhileNode*>(node);
    Value condition = Interpret(whileNode->GetCondition());
    Value result(0);

    unsigned int scopeCount = m_currentSymbolTable->GetScopeCount();
    m_currentSymbolTable->AddScope("WhileLoop" + std::to_string(scopeCount));
    m_currentSymbolTable = m_currentSymbolTable->GetScope("WhileLoop" + std::to_string(scopeCount));

    bool retainContinue = m_state.canContinue;
    bool retainBreak = m_state.canBreak;

    m_state.canContinue = true;
    m_state.canBreak = true;

    if (whileNode->IsDoWhile())
    {
        do
        {
            result = Interpret(whileNode->GetBlock());
            condition = Interpret(whileNode->GetCondition());
            if (m_state.breakCalled)
            {
                m_state.breakCalled = false;
                break;
            }
            if (m_state.continueCalled)
            {
                m_state.continueCalled = false;
                continue;
            }
        } while (condition != 0);
    }
    else
    {
        while (condition != 0)
        {
            result = Interpret(whileNode->GetBlock());
            condition = Interpret(whileNode->GetCondition());
            if (m_state.breakCalled)
            {
                m_state.breakCalled = false;
                break;
            }
            if (m_state.continueCalled)
            {
                m_state.continueCalled = false;
                continue;
            }
        }
    }

    m_currentSymbolTable = m_currentSymbolTable->GetParentScope();
    m_currentSymbolTable->RemoveScope("WhileLoop" + std::to_string(scopeCount));

    m_state.canContinue = retainContinue;
    m_state.canBreak = retainBreak;

    return result;
}

Value Interpreter::InterpretFor(TokenNode* node)
{
    ForNode* forNode = dynamic_cast<ForNode*>(node);
    unsigned int scopeCount = m_currentSymbolTable->GetScopeCount();
    m_currentSymbolTable->AddScope("ForLoop" + std::to_string(scopeCount));
    m_currentSymbolTable = m_currentSymbolTable->GetScope("ForLoop" + std::to_string(scopeCount));

    Value result(0);

    Value init = Interpret(forNode->GetInit());
    Value condition = Interpret(forNode->GetCondition());

    bool retainContinue = m_state.canContinue;
    bool retainBreak = m_state.canBreak;

    m_state.canContinue = true;
    m_state.canBreak = true;

    while (condition != 0)
    {
        result = Interpret(forNode->GetBlock());
        Value increment = Interpret(forNode->GetIncrement());
        condition = Interpret(forNode->GetCondition());
        if (m_state.breakCalled)
        {
            m_state.breakCalled = false;
            break;
        }
        if (m_state.continueCalled)
        {
            m_state.continueCalled = false;
            continue;
        }
    }

    m_currentSymbolTable = m_currentSymbolTable->GetParentScope();
    m_currentSymbolTable->RemoveScope("ForLoop" + std::to_string(scopeCount));

    m_state.canContinue = retainContinue;
    m_state.canBreak = retainBreak;

    return result;
}

Value Interpreter::InterpretArrayAccess(TokenNode* node)
{
    ArrayAccessNode* arrayAccessNode = dynamic_cast<ArrayAccessNode*>(node);

    Token token = arrayAccessNode->GetToken();
    std::string varName = token.GetValue();

    Value index = Interpret(arrayAccessNode->GetIndex());
    Value array = Value();
    if (arrayAccessNode->GetObject().GetType() == Token::Type::Identifier)
    {
        Token object = arrayAccessNode->GetObject();
        if (m_currentSymbolTable->ObjectInstanceExists(object.GetValue(), true))
        {
            std::string instanceName = m_currentSymbolTable->GetObjectInstanceScopeName(object.GetValue());
            array = m_currentSymbolTable->GetScope(instanceName)->GetVar(varName, false);
        }
        else
        {
            Error e("Array Access on non-object variable: ", Position("Interpreter", 0, 0, 0));
            std::cout << e.ToString() << varName << '\n';
            return Value();
        }
    }
    else
    {
        array = m_currentSymbolTable->GetVar(varName);
    }
    if (!array.isString() && !array.isArray())
    {
        Error e("Array Access on non-array variable: ", Position("Interpreter", 0, 0, 0));
        std::cout << e.ToString() << varName << '\n';
        return Value();
    }
    if (!index.isNumber())
    {
        Error e("Array Access with non-integer index: ", Position("Interpreter", 0, 0, 0));
        std::cout << e.ToString() << varName << '\n';
        return Value();
    }
    size_t number = (size_t)index.getNumber();

    if (array.isArray())
    {
        if (number >= array.getArray().size())
        {
            Error e("Array Access with out of bounds index: ", Position("Interpreter", 0, 0, 0));
            std::cout << e.ToString() << varName << '\n';
            return Value();
        }
        return array.getArray()[number];
    }

    if (number >= array.getString().size())
    {
        Error e("Array Access with out of bounds index: ", Position("Interpreter", 0, 0, 0));
        std::cout << e.ToString() << varName << '\n';
        return Value();
    }
    char c = array.getString()[number];
    std::string s(1, c);
    return Value(s);
}

Value Interpreter::InterpretArrayInit(TokenNode* node)
{
    ArrayInitNode* arrayInitNode = dynamic_cast<ArrayInitNode*>(node);
    Token token = arrayInitNode->GetToken();
    std::vector<Value> values;
    for (TokenNode* valueNode : arrayInitNode->GetElements())
    {
        values.push_back(Interpret(valueNode));
    }
    Value array(values);
    if (arrayInitNode->GetObject().GetType() == Token::Type::Identifier)
    {
        Token object = arrayInitNode->GetObject();
        if (m_currentSymbolTable->ObjectInstanceExists(object.GetValue()))
        {
            std::string instanceName = m_currentSymbolTable->GetObjectInstanceScopeName(object.GetValue());
            m_currentSymbolTable->GetScope(instanceName)->RegisterVar(token.GetValue(), array);
        }
        else
        {
            Error e("Unknown Object Instance: ", Position("", 0, 0, 0));
            std::cout << e.ToString() << object.GetValue() << '\n';
            return Value();
        }
    }
    else
    {
        m_currentSymbolTable->RegisterVar(token.GetValue(), array);
    }
    return array;
}

Value Interpreter::InterpretArrayAssign(TokenNode* node)
{
    ArrayAssignmentNode* arrayAssignNode = dynamic_cast<ArrayAssignmentNode*>(node);
    Token token = arrayAssignNode->GetToken();
    std::string varName = token.GetValue();
    Value array = Value();
    SymbolTable* scope = m_currentSymbolTable;

    if (arrayAssignNode->GetObject().GetType() == Token::Type::Identifier)
    {
        Token object = arrayAssignNode->GetObject();
        if (m_currentSymbolTable->ObjectInstanceExists(object.GetValue(), true))
        {
            std::string instanceName = m_currentSymbolTable->GetObjectInstanceScopeName(object.GetValue());
            scope = m_currentSymbolTable->GetScope(instanceName);
            array = scope->GetVar(varName, false);
        }
        else
        {
            Error e("Array Assignment on non-object variable: ", Position("Interpreter", 0, 0, 0));
            std::cout << e.ToString() << varName << '\n';
            return Value();
        }
    }
    else
    {
        array = m_currentSymbolTable->GetVar(varName, true);
    }

    if (!array.isArray())
    {
        Error e("Array Assignment on non-array variable: ", Position("Interpreter", 0, 0, 0));
        std::cout << e.ToString() << varName << '\n';
        return Value();
    }
    Value index = Interpret(arrayAssignNode->GetIndex());
    if (!index.isNumber())
    {
        Error e("Array Assignment with non-integer index: ", Position("Interpreter", 0, 0, 0));
        std::cout << e.ToString() << varName << '\n';
        return Value();
    }
    std::vector<Value> values = array.getArray();
    size_t number = (size_t)index.getNumber();
    if (number >= values.size())
    {
        while (number >= values.size())
        {
            values.push_back(Value());
        }
    }
    Value value = Interpret(arrayAssignNode->GetValue());
    
    values[number] = value;
    scope->RegisterVar(varName, values);
    return value;
}

Value Interpreter::InterpretFunctionCall(TokenNode* node)
{
    FunctionCallNode* funcCallNode = dynamic_cast<FunctionCallNode*>(node);
    Token token = funcCallNode->GetToken();
    std::string funcName = token.GetValue();

    SymbolTable* scope = m_currentSymbolTable;
    if (funcCallNode->GetObject().GetType() == Token::Type::Identifier)
    {
        Token object = funcCallNode->GetObject();
        if (m_currentSymbolTable->ObjectInstanceExists(object.GetValue(), true))
        {
            std::string instanceName = m_currentSymbolTable->GetObjectInstanceScopeName(object.GetValue());
            scope = m_currentSymbolTable->GetScope(instanceName);
        }
        else
        {
            Error e("Function Call on non-object variable: ", Position("Interpreter", 0, 0, 0));
            std::cout << e.ToString() << funcName << '\n';
            return Value();
        }
    }

    if (m_currentSymbolTable->IsBuiltInFunction(funcName))
    {
        std::vector<Value> args;
        ArgumentListNode* argListNode = dynamic_cast<ArgumentListNode*>(funcCallNode->GetArguments());
        if (argListNode)
        {
            for (TokenNode* argNode : argListNode->GetArguments())
            {
                args.push_back(Interpret(argNode));
            }
        }
        return m_currentSymbolTable->CallBuiltInFunction(funcName, args);
    }
    else if (scope->IsUserFunction(funcName))
    {
        std::vector<Value> args;
        ArgumentListNode* argListNode = dynamic_cast<ArgumentListNode*>(funcCallNode->GetArguments());
        if (argListNode)
        {
            for (TokenNode* argNode : argListNode->GetArguments())
            {
                args.push_back(Interpret(argNode));
            }
        }

        SymbolTable* parent = scope;
        SymbolTable* current = m_currentSymbolTable;
        unsigned int scopeCount = scope->GetScopeCount();
        scope->AddScope("UserFunc" + funcName + std::to_string(scopeCount));
        scope = scope->GetScope("UserFunc" + funcName + std::to_string(scopeCount));
        scope->RegisterVar("args", args);

        bool retainReturn = m_state.returnCalled;
        m_state.canReturn = true;

        m_currentSymbolTable = scope;
        Value result = Interpret(parent->GetUserFunction(funcName));
        m_currentSymbolTable = current;

        if (m_state.returnCalled)
        {
            result = m_state.returnValue;
        }

        scope = scope->GetParentScope();
        scope->RemoveScope("UserFunc" + funcName + std::to_string(scopeCount));

        m_state.canReturn = retainReturn;
        
        return result;
    }

    Error e("Unknown Function: ", Position("Interpreter", 0, 0, 0));
    std::cout << e.ToString() << funcName << '\n';
    return Value();
    
}

Value Interpreter::InterpretBreak(TokenNode* node)
{
    if (!m_state.canBreak)
    {
        Error e("Break called outside of loop: ", Position("Interpreter", 0, 0, 0));
        std::cout << e.ToString() << '\n';
    }
    else
    {
        m_state.breakCalled = true;
    }
    return Value();
}

Value Interpreter::InterpretContinue(TokenNode* node)
{
    if (!m_state.canContinue)
    {
        Error e("Continue called outside of loop: ", Position("Interpreter", 0, 0, 0));
        std::cout << e.ToString() << '\n';
    }
    else
    {
        m_state.continueCalled = true;
    }
    return Value();
}

Value Interpreter::InterpretReturn(TokenNode* node)
{
    if (!m_state.canReturn)
    {
        Error e("Return called outside of function: ", Position("Interpreter", 0, 0, 0));
        std::cout << e.ToString() << '\n';
        return Value();
    }

    ReturnNode* returnNode = dynamic_cast<ReturnNode*>(node);
    Value value;
    if (returnNode->GetValue())
    {
        value = Interpret(returnNode->GetValue());
    }
    m_state.returnCalled = true;
    m_state.returnValue = value;
    return value;
}

Value Interpreter::InterpretFunctionDefinition(TokenNode* node)
{
    FunctionDefinitionNode* funcDefNode = dynamic_cast<FunctionDefinitionNode*>(node);
    Token token = funcDefNode->GetToken();
    std::string funcName = token.GetValue();

    SymbolTable* scope = m_currentSymbolTable;
    if (funcDefNode->GetObject().GetType() == Token::Type::Identifier)
    {
        Token object = funcDefNode->GetObject();
        if (m_currentSymbolTable->ObjectInstanceExists(object.GetValue(), true))
        {
            std::string instanceName = m_currentSymbolTable->GetObjectInstanceScopeName(object.GetValue());
            scope = m_currentSymbolTable->GetScope(instanceName);
        }
        else
        {
            Error e("Function Definition on non-object variable: ", Position("Interpreter", 0, 0, 0));
            std::cout << e.ToString() << funcName << '\n';
            return Value();
        }
    }

    scope->RegisterUserFunction(funcName, funcDefNode->GetBlock());

    return Value();
}

Value Interpreter::InterpretObjectDefinition(TokenNode* node)
{
    if (!m_state.canDefineObject)
    {
        Error e("Object Definition inside of object: ", Position("Interpreter", 0, 0, 0));
        std::cout << e.ToString() << '\n';
        return Value();
    }

    bool retainDefineObject = m_state.canDefineObject;
    m_state.canDefineObject = false;

    ObjectDefinitionNode* objDefNode = dynamic_cast<ObjectDefinitionNode*>(node);
    Token token = objDefNode->GetToken();
    std::string objName = token.GetValue();

    if (!m_currentSymbolTable->RegisterObject(objName))
    {
        Error e("Object Definition with duplicate name: ", Position("Interpreter", 0, 0, 0));
        std::cout << e.ToString() << objName << '\n';
        m_state.hasError = true;
        return Value();
    }

    std::string objScopeName = m_currentSymbolTable->GetObjectScopeName(objName);
    m_currentSymbolTable = m_currentSymbolTable->GetScope(objScopeName);

    TokenNode* block = objDefNode->GetBlock();

    if (block)
    {
        SequenceNode* seqNode = dynamic_cast<SequenceNode*>(block);
        if (!seqNode)
        {
            Error e("Object Definition with non-sequence block: ", Position("Interpreter", 0, 0, 0));
            std::cout << e.ToString() << objName << '\n';
            m_state.hasError = true;
            return Value();
        }

        for (TokenNode* snode : seqNode->GetNodes())
        {
            if (snode->GetType() == NodeType::ArrayInit)
            {
                InterpretArrayInit(snode);
            }
            else if (snode->GetType() == NodeType::FunctionDefinition)
            {
                InterpretFunctionDefinition(snode);
            }
            else if (snode->GetType() == NodeType::VarAssign)
            {
                InterpretVarAssign(snode);
            }
            else
            {
                Error e("Object Definition with invalid block: ", Position("Interpreter", 0, 0, 0));
                std::cout << e.ToString() << objName << '\n';
                m_state.hasError = true;
                return Value();
            }
        }
    }

    m_state.canDefineObject = retainDefineObject;

    m_currentSymbolTable = m_currentSymbolTable->GetParentScope();
    return Value();
}