#include "SymbolTable.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "TokenNode.hpp"

SymbolTable::SymbolTable(const std::string& name, SymbolTable* parentScope)
    : m_name(name)
    , m_parentScope(parentScope)
{
    if (m_parentScope == NULL)
    {
        m_keywords = { "if", "else", "while", "do", "for", "true", "false", "null", "break", "continue", "return", "func", "object", "this" };
        m_builtInFunctions["print"] = &SymbolTable::Print;
        m_builtInFunctions["substring"] = &SymbolTable::Substring;
        m_builtInFunctions["strlen"] = &SymbolTable::stringlength;
        m_builtInFunctions["input"] = &SymbolTable::input;
        m_builtInFunctions["tostring"] = &SymbolTable::toString;
        m_builtInFunctions["tonumber"] = &SymbolTable::toNumber;
        m_builtInFunctions["sizeof"] = &SymbolTable::arraySize;
        m_builtInFunctions["seed"] = &SymbolTable::seed;
        m_builtInFunctions["round"] = &SymbolTable::round;
        m_builtInFunctions["floor"] = &SymbolTable::floor;
        m_builtInFunctions["ceil"] = &SymbolTable::ceil;
        m_builtInFunctions["random"] = &SymbolTable::random;
        m_builtInFunctions["abs"] = &SymbolTable::abs;
        m_builtInFunctions["sin"] = &SymbolTable::sin;
        m_builtInFunctions["cos"] = &SymbolTable::cos;
        m_builtInFunctions["tan"] = &SymbolTable::tan;
        m_builtInFunctions["asin"] = &SymbolTable::asin;
        m_builtInFunctions["acos"] = &SymbolTable::acos;
        m_builtInFunctions["atan"] = &SymbolTable::atan;
        m_builtInFunctions["atan2"] = &SymbolTable::atan2;
        m_builtInFunctions["sqrt"] = &SymbolTable::sqrt;
        m_builtInFunctions["log"] = &SymbolTable::log;
        m_builtInFunctions["log10"] = &SymbolTable::log10;
    }
}

SymbolTable::~SymbolTable()
{
    /*m_variables.clear();
    m_parentScope = NULL;

    for (auto& func : m_builtInFunctions)
    {
        func.second = NULL;
    }
    m_builtInFunctions.clear();

    m_keywords.clear();

    for (auto& func : m_userFunctions)
    {
        delete func.second;
        func.second = NULL;
    }

    m_userFunctions.clear();

    m_objectNames.clear();
    m_objectInstanceNames.clear();

    for (auto& scope : m_scopes)
    {
        delete scope.second;
        scope.second = NULL;
    }

    m_scopes.clear();*/
}

bool SymbolTable::VarExists(const std::string& varName, bool global) const
{
    if (m_variables.find(varName) != m_variables.end())
    {
        return true;
    }
    else if (m_parentScope != NULL && global)
    {
        return m_parentScope->VarExists(varName, true);
    }
    return false;
}

void SymbolTable::RegisterVar(const std::string& name, const Value& value)
{
    if (m_variables.find(name) != m_variables.end())
    {
        m_variables[name] = value;
    }
    else if (m_parentScope != NULL)
    {
        SymbolTable* scope = m_parentScope->GetVarScope(name);
        if (scope != NULL)
        {
            scope->RegisterVar(name, value);
        }
        else
        {
            m_variables[name] = value;
        }
    }
    else
    {
        m_variables[name] = value;
    }
}

Value SymbolTable::GetVar(const std::string& name, bool global) const
{
    if (m_variables.find(name) != m_variables.end())
    {
        return m_variables.at(name);
    }
    else if (m_parentScope != NULL && global)
    {
        return m_parentScope->GetVar(name, true);
    }

    return Value(-1);
}

SymbolTable* SymbolTable::GetVarScope(const std::string& name) const
{
    if (m_variables.find(name) != m_variables.end())
    {
        return const_cast<SymbolTable*>(this);
    }
    else if (m_parentScope != NULL)
    {
        return m_parentScope->GetVarScope(name);
    }

    return NULL;
}

void SymbolTable::DestroyVar(const std::string& name)
{
    if (m_variables.find(name) != m_variables.end())
    {
        m_variables.erase(name);
    }
}

bool SymbolTable::IsKeyword(const std::string& name) const
{
    bool isKeyword = false;
    if (m_parentScope != NULL)
    {
        isKeyword = m_parentScope->IsKeyword(name);
    }
    return isKeyword || std::find(m_keywords.begin(), m_keywords.end(), name) != m_keywords.end();
}

bool SymbolTable::IsBuiltInFunction(const std::string& name) const
{
    if (m_parentScope != NULL)
    {
        return g_symbolTable.IsBuiltInFunction(name);
    }
    return m_builtInFunctions.find(name) != m_builtInFunctions.end();
}

bool SymbolTable::IsUserFunction(const std::string& name, bool global) const
{
    bool isFunction = false;
    if (m_parentScope != NULL && global)
    {
        isFunction = m_parentScope->IsUserFunction(name, true);
    }
    return isFunction || m_userFunctions.find(name) != m_userFunctions.end();
}

SymbolTable* SymbolTable::GetUserFunctionScope(const std::string& name) const
{
    if (m_userFunctions.find(name) != m_userFunctions.end())
    {
        return const_cast<SymbolTable*>(this);
    }
    else if (m_parentScope != NULL)
    {
        return m_parentScope->GetUserFunctionScope(name);
    }

    return NULL;
}

void SymbolTable::DestroyUserFunction(const std::string& name)
{
    if (m_userFunctions.find(name) != m_userFunctions.end())
    {
        m_userFunctions.erase(name);
    }
}

Value SymbolTable::CallBuiltInFunction(const std::string& name, const std::vector<Value>& args)
{
    if (m_parentScope != NULL)
    {
        return m_parentScope->CallBuiltInFunction(name, args);
    }
    return (this->*m_builtInFunctions.at(name))(args);
}

void SymbolTable::RegisterUserFunction(const std::string& name, TokenNode* body)
{
    m_userFunctions[name] = body;
}

TokenNode* SymbolTable::GetUserFunction(const std::string& name, bool global) const
{
    TokenNode* function = NULL;
    if (m_parentScope != NULL && global)
    {
        function = m_parentScope->GetUserFunction(name);
    }
    if (function == NULL)
    {
        function = m_userFunctions.at(name);
    }
    return function;
}

bool SymbolTable::RegisterObject(const std::string& name)
{
    if (ObjectExists(name))
    {
        return false;
    }

    unsigned int scopeCount = GetScopeCount();
    AddScope("ObjectDef" + name + std::to_string(scopeCount));
    m_objectNames[name] = "ObjectDef" + name + std::to_string(scopeCount);

    return true;
}

bool SymbolTable::ObjectExists(const std::string& name, bool global) const
{
    bool exists = false;
    if (global)
    {
        if (m_parentScope != NULL)
        {
            exists = m_parentScope->ObjectExists(name, global);
        }
    }
    exists = exists || m_objectNames.find(name) != m_objectNames.end();
    return exists;
}

SymbolTable* SymbolTable::GetObjectScope(const std::string& name) const
{
    if (m_objectNames.find(name) != m_objectNames.end())
    {
        return const_cast<SymbolTable*>(this);
    }
    else if (m_parentScope != NULL)
    {
        return m_parentScope->GetObjectScope(name);
    }

    return NULL;
}

std::string SymbolTable::GetObjectScopeName(const std::string& name) const
{
    if (m_objectNames.find(name) != m_objectNames.end())
    {
        return m_objectNames.at(name);
    }
    else if (m_parentScope != NULL)
    {
        return m_parentScope->GetObjectScopeName(name);
    }

    return "";
}

bool SymbolTable::AddObjectInstance(const std::string& name, const std::string& objectName)
{
    if (!ObjectExists(objectName, true))
    {
        return false;
    }

    unsigned int scopeCount = GetScopeCount();
    AddScope("ObjectInst" + name + std::to_string(scopeCount));
    m_objectNames[name] = "ObjectInst" + name + std::to_string(scopeCount);

    SymbolTable* objectDefinition = GetObjectScope(objectName);
    objectDefinition = objectDefinition->GetScope(GetObjectScopeName(objectName));

    SymbolTable* objectInstance = GetScope("ObjectInst" + name + std::to_string(scopeCount));
    
    for (auto& var : objectDefinition->m_variables)
    {
        objectInstance->m_variables[var.first] = var.second;
    }

    for (auto& func : objectDefinition->m_userFunctions)
    {
        objectInstance->m_userFunctions[func.first] = func.second;
    }

    return true;
}

bool SymbolTable::ObjectInstanceExists(const std::string& name, bool global) const
{
    bool exists = false;
    if (global)
    {
        if (m_parentScope != NULL)
        {
            exists = m_parentScope->ObjectInstanceExists(name, global);
        }
    }
    exists = exists || m_objectNames.find(name) != m_objectNames.end();
    return exists;
}

std::string SymbolTable::GetObjectInstanceScopeName(const std::string& name, bool global) const
{
    if (m_objectNames.find(name) != m_objectNames.end())
    {
        return m_objectNames.at(name);
    }
    else if (m_parentScope != NULL && global)
    {
        return m_parentScope->GetObjectInstanceScopeName(name);
    }

    return "";
}

SymbolTable* SymbolTable::GetObjectInstanceScope(const std::string& name) const
{
    if (m_objectNames.find(name) != m_objectNames.end())
    {
        return const_cast<SymbolTable*>(this);
    }
    else if (m_parentScope != NULL)
    {
        return m_parentScope->GetObjectInstanceScope(name);
    }

    return NULL;
}

void SymbolTable::DestroyObjectInstance(const std::string& name)
{
    if (m_objectNames.find(name) != m_objectNames.end())
    {
        m_objectNames.erase(name);
    }
}

SymbolTable* SymbolTable::GetParentScope() const
{
    return m_parentScope;
}

void SymbolTable::AddScope(const std::string& name)
{
    m_scopes[name] = new SymbolTable(name, this);
}

SymbolTable* SymbolTable::GetScope(const std::string& name) const
{
    return m_scopes.at(name);
}

void SymbolTable::RemoveScope(const std::string& name)
{
    delete m_scopes.at(name);
    m_scopes.erase(name);
}

unsigned int SymbolTable::GetScopeCount() const
{
    return m_scopes.size();
}

const std::string& SymbolTable::GetName() const
{
    return m_name;
}

Value SymbolTable::Print(const std::vector<Value>& args)
{
    for (auto& arg : args)
    {
        std::cout << arg.toString();
    }
    std::cout << '\n';

    return Value();
}

Value SymbolTable::Substring(const std::vector<Value>& args)
{
    if (args.size() != 3 || !args[0].isString() || !args[1].isNumber() || !args[2].isNumber())
    {
        return Value();
    }

    std::string str = args[0].toString();
    size_t start = args[1].getNumber();
    size_t end = args[2].getNumber();

    if (start < 0 || end < 0 || start > end || end > str.length())
    {
        return Value();
    }

    return Value(str.substr(start, end));
}

Value SymbolTable::stringlength(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isString())
    {
        return Value();
    }

    return Value(args[0].toString().length());
}

Value SymbolTable::input(const std::vector<Value>& args)
{
    if (args.size() != 0)
    {
        for (auto& arg : args)
        {
            std::cout << arg.toString();
        }
    }

    std::string input;
    std::getline(std::cin, input);

    return Value(input);
}

Value SymbolTable::toString(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber())
    {
        return Value();
    }

    if (args[0].getNumber() == std::floor(args[0].getNumber()))
    {
        return Value(std::to_string((int)args[0].getNumber()));
    }
    return Value(std::to_string(args[0].getNumber()));
}

Value SymbolTable::toNumber(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isString())
    {
        return Value();
    }

    try
    {
        return Value(std::stof(args[0].getString()));
    }
    catch(const std::exception& e)
    {
        return Value();
    }   
}

Value SymbolTable::arraySize(const std::vector<Value>& args)
{
    if (args.size() != 1 || (!args[0].isArray() && !args[0].isString()))
    {
        return Value();
    }

    if (args[0].isArray())
    {
        return Value(args[0].getArray().size());
    }
    else
    {
        return Value(args[0].getString().length());
    }
}

Value SymbolTable::seed(const std::vector<Value>& args)
{
    if (args.size() == 1)
        srand(args[0].getNumber());
    else if (args.size() == 0)
        srand(time(NULL));
    return Value();
}

Value SymbolTable::round(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber())
    {
        return Value();
    }

    return Value(std::round(args[0].getNumber()));
}

Value SymbolTable::floor(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber())
    {
        return Value();
    }

    return Value(std::floor(args[0].getNumber()));
}

Value SymbolTable::ceil(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber())
    {
        return Value();
    }

    return Value(std::ceil(args[0].getNumber()));
}

Value SymbolTable::random(const std::vector<Value>& args)
{
    if (args.size() != 2 || !args[0].isNumber() || !args[1].isNumber())
    {
        return Value();
    }

    return Value(float(rand()) / RAND_MAX * (args[1].getNumber() - args[0].getNumber()) + args[0].getNumber());
}

Value SymbolTable::abs(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber())
    {
        return Value();
    }

    return Value(std::abs(args[0].getNumber()));
}

Value SymbolTable::sin(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber())
    {
        return Value();
    }

    return Value(std::sin(args[0].getNumber()));
}

Value SymbolTable::cos(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber())
    {
        return Value();
    }

    return Value(std::cos(args[0].getNumber()));
}

Value SymbolTable::tan(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber())
    {
        return Value();
    }

    return Value(std::tan(args[0].getNumber()));
}

Value SymbolTable::asin(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber() || args[0].getNumber() < -1 || args[0].getNumber() > 1)
    {
        return Value();
    }

    return Value(std::asin(args[0].getNumber()));
}

Value SymbolTable::acos(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber() || args[0].getNumber() < -1 || args[0].getNumber() > 1)
    {
        return Value();
    }

    return Value(std::acos(args[0].getNumber()));
}

Value SymbolTable::atan(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber())
    {
        return Value();
    }

    return Value(std::atan(args[0].getNumber()));
}

Value SymbolTable::atan2(const std::vector<Value>& args)
{
    if (args.size() != 2 || !args[0].isNumber() || !args[1].isNumber())
    {
        return Value();
    }

    return Value(std::atan2(args[0].getNumber(), args[1].getNumber()));
}

Value SymbolTable::sqrt(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber() || args[0].getNumber() < 0)
    {
        return Value();
    }

    return Value(std::sqrt(args[0].getNumber()));
}

Value SymbolTable::log(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber() || args[0].getNumber() <= 0)
    {
        return Value();
    }

    return Value(std::log(args[0].getNumber()));
}

Value SymbolTable::log10(const std::vector<Value>& args)
{
    if (args.size() != 1 || !args[0].isNumber() || args[0].getNumber() <= 0)
    {
        return Value();
    }

    return Value(std::log10(args[0].getNumber()));
}