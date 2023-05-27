#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Value.hpp"

class TokenNode;
class SymbolTable
{
public:
    SymbolTable(const std::string& name, SymbolTable* parentScope);
    ~SymbolTable();

    bool VarExists(const std::string& varName, bool global = false) const;
    void RegisterVar(const std::string& name, const Value& value);
    Value GetVar(const std::string& name, bool global = false) const;
    SymbolTable* GetVarScope(const std::string& name) const;
    void DestroyVar(const std::string& name);
    
    bool IsKeyword(const std::string& name) const;
    bool IsBuiltInFunction(const std::string& name) const;
    bool IsUserFunction(const std::string& name, bool global = false) const;
    SymbolTable* GetUserFunctionScope(const std::string& name) const;
    void DestroyUserFunction(const std::string& name);

    Value CallBuiltInFunction(const std::string& name, const std::vector<Value>& args);

    void RegisterUserFunction(const std::string& name, TokenNode* node);
    TokenNode* GetUserFunction(const std::string& name, bool global = false) const;

    bool RegisterObject(const std::string& name, const std::string& parentName = "");
    bool ObjectExists(const std::string& name, const std::string& moduleName = "", bool global = false) const;
    SymbolTable* GetObjectScope(const std::string& name, const std::string& moduleName = "") const;
    std::string GetObjectScopeName(const std::string& name) const;

    bool AddObjectInstance(const std::string& name, const std::string& objectName, const std::string& moduleName = "");
    bool ObjectInstanceExists(const std::string& name, bool global = false) const;
    std::string GetObjectInstanceScopeName(const std::string& name, bool global = false) const;
    SymbolTable* GetObjectInstanceScope(const std::string& name) const;
    void DestroyObjectInstance(const std::string& name);

    bool RegisterModule(const std::string& name, SymbolTable* module);
    bool RegisterModule(const std::string& name);
    bool ModuleExists(const std::string& name) const;
    SymbolTable* GetModule(const std::string& name);
    void DestroyModule(const std::string& name);

    SymbolTable* GetParentScope() const;
    void AddScope(const std::string& name);
    SymbolTable* GetScope(const std::string& name) const;
    void RemoveScope(const std::string& name);
    unsigned int GetScopeCount() const;

    const std::string& GetName() const;

    void CleanUp();


protected:
    std::unordered_map<std::string, Value> m_variables;
    std::vector<std::string> m_keywords;

    typedef Value(SymbolTable::*BuiltInFunction)(const std::vector<Value>&);
    
    std::unordered_map<std::string, BuiltInFunction> m_builtInFunctions;
    std::unordered_map<std::string, TokenNode*> m_userFunctions;

    std::string m_name;

    std::unordered_map<std::string, SymbolTable*> m_modules;
    std::unordered_map<std::string, SymbolTable*> m_scopes;
    SymbolTable* m_parentScope;

    std::unordered_map<std::string, std::string> m_objectNames;
    std::unordered_map<std::string, std::string> m_objectInstanceNames;

private:
    Value Print(const std::vector<Value>& args);
    Value Substring(const std::vector<Value>& args);
    Value stringlength(const std::vector<Value>& args);
    Value input(const std::vector<Value>& args);
    Value toString(const std::vector<Value>& args);
    Value toNumber(const std::vector<Value>& args);
    Value arraySize(const std::vector<Value>& args);
    Value seed(const std::vector<Value>& args);
    Value round(const std::vector<Value>& args);
    Value floor(const std::vector<Value>& args);
    Value ceil(const std::vector<Value>& args);
    Value random(const std::vector<Value>& args);
    Value abs(const std::vector<Value>& args);
    Value sin(const std::vector<Value>& args);
    Value cos(const std::vector<Value>& args);
    Value tan(const std::vector<Value>& args);
    Value asin(const std::vector<Value>& args);
    Value acos(const std::vector<Value>& args);
    Value atan(const std::vector<Value>& args);
    Value atan2(const std::vector<Value>& args);
    Value sqrt(const std::vector<Value>& args);
    Value log(const std::vector<Value>& args);
    Value log10(const std::vector<Value>& args);
};

static SymbolTable g_symbolTable("Global", NULL);

static bool RegisterModule(const std::string& name, SymbolTable* module)
{
    return g_symbolTable.RegisterModule(name, module);
}