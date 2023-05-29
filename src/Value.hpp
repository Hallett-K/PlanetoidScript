#pragma once

#include <string>
#include <vector>

class Value
{
public:
    Value();
    ~Value();

    Value(const Value& other);
    Value(float value);
    Value(const std::string& value, bool isString = true);
    Value(const std::vector<Value>& value);

    float getNumber() const { return m_number; }
    std::string getString() const { return m_string; }
    std::vector<Value> getArray() const { return m_array; }

    void operator=(const Value& other);
    void operator=(float value);
    void operator=(const std::string& value);
    void operator=(const std::vector<Value>& value);

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

    bool operator!=(const Value& other) const;
    bool operator!=(float value) const;
    bool operator!=(const std::string& value) const;
    bool operator!=(const std::vector<Value>& value) const;

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
    };

    Type m_type;
};