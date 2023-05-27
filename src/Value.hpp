#pragma once

#include <string>
#include <vector>

struct ObjectInstancePointer
{
    std::string instance;

    bool operator==(const ObjectInstancePointer& other) const
    {
        return instance == other.instance;
    }

    bool operator!=(const ObjectInstancePointer& other) const
    {
        return instance != other.instance;
    }
};

class Value
{
public:
    Value();
    ~Value();

    Value(const Value& other);
    Value(float value);
    Value(const std::string& value);
    Value(const std::vector<Value>& value);
    Value(const ObjectInstancePointer& value);

    float getNumber() const { return m_number; }
    std::string getString() const { return m_string; }
    std::vector<Value> getArray() const { return m_array; }
    ObjectInstancePointer getPointer() const { return m_objectInstance; }

    void operator=(const Value& other);
    void operator=(float value);
    void operator=(const std::string& value);
    void operator=(const std::vector<Value>& value);
    void operator=(const ObjectInstancePointer& value);

    Value operator+(const Value& other) const;
    Value operator+(float value) const;
    Value operator+(const std::string& value) const;
    Value operator+(const std::vector<Value>& value) const;

    Value operator-(const Value& other) const;
    Value operator-(float value) const;

    Value operator*(const Value& other) const;
    Value operator*(float value) const;

    Value operator/(const Value& other) const;
    Value operator/(float value) const;

    Value operator^(const Value& other) const;
    Value operator^(float value) const;

    Value operator-() const;

    bool operator==(const Value& other) const;
    bool operator==(float value) const;
    bool operator==(const std::string& value) const;
    bool operator==(const std::vector<Value>& value) const;
    bool operator==(const ObjectInstancePointer& value) const;

    bool operator!=(const Value& other) const;
    bool operator!=(float value) const;
    bool operator!=(const std::string& value) const;
    bool operator!=(const std::vector<Value>& value) const;
    bool operator!=(const ObjectInstancePointer& value) const;

    bool operator>(const Value& other) const;
    bool operator>(float value) const;

    bool operator<(const Value& other) const;
    bool operator<(float value) const;

    bool operator>=(const Value& other) const;
    bool operator>=(float value) const;

    bool operator<=(const Value& other) const;
    bool operator<=(float value) const;

    bool isNumber() const { return m_type == Type::Number; }
    bool isString() const { return m_type == Type::String; }
    bool isArray() const { return m_type == Type::Array; }
    bool isPointer() const { return m_type == Type::ObjectPointer; }
    bool isNull() const { return m_type == Type::Null; }

    // Array
    size_t size() const;
    Value& operator[](size_t index);
    const Value& operator[](size_t index) const;

    std::string toString() const;

private:
    enum class Type
    {
        Number,
        String,
        Array,
        ObjectPointer,
        Null
    };

    union
    {
        float m_number;
        std::string m_string;
        std::vector<Value> m_array;
        ObjectInstancePointer m_objectInstance;
    };

    Type m_type;
};