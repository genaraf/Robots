//
// Values used by stack machine
//
#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <cstring>

using std::string;

class InterpretatorException {
public:
    char reason[128];
    InterpretatorException() { reason[0] = 0; }
    InterpretatorException(const char* cause) {
        strncpy(reason, cause, 127);
        reason[127] = 0;
    }
};

class NameDef;

// Bits in value_type:
const int VALUE_TYPE_INT = 1;
const int VALUE_TYPE_DOUBLE = 2;
const int VALUE_TYPE_STRING = 4;
const int VALUE_TYPE_VARIABLE = 8;
const int VALUE_TYPE_ARRAY = 16;

class Value {
public:
    int value_type;
    int int_value; // For array, it is an offset of array element
    double double_value;
    string string_value;
    NameDef* name_ptr;

    Value():
        value_type(0),
        int_value(0),
        double_value(0.0),
        string_value(),
        name_ptr(0)
    {}

    Value(const Value& v):
        value_type(v.value_type),
        int_value(v.int_value),
        double_value(v.double_value),
        string_value(v.string_value),
        name_ptr(v.name_ptr)
    {}

    Value& operator=(const Value& v) {
        value_type = v.value_type;
        int_value = v.int_value;
        double_value = v.double_value;
        string_value = v.string_value;
        name_ptr = v.name_ptr;
        return *this;
    }

    bool isNumber() const {
        return (
            value_type == VALUE_TYPE_INT ||
            value_type == VALUE_TYPE_DOUBLE
        );
    }

    bool isInt() const {
        return (value_type == VALUE_TYPE_INT);
    }

    bool isDouble() const {
        return (value_type == VALUE_TYPE_DOUBLE);
    }

    bool isString() const {
        return (
            value_type == VALUE_TYPE_STRING
        );
    }

    void convertToInt(Value& v) const;
    void convertToDouble(Value& v) const;
    void convertToString(Value& v) const;

    // Operations:
    static void add(
        const Value& v0, const Value& v1, Value& res
    );
    static void sub(
        const Value& v0, const Value& v1, Value& res
    );
    static void mul(
        const Value& v0, const Value& v1, Value& res
    );
    static void div(
        const Value& v0, const Value& v1, Value& res
    );
    static void mod(
        const Value& v0, const Value& v1, Value& res
    );
    static void negate(
        const Value& v0, Value& res
    );
    static void compare(
        const Value& v0, const Value& v1, Value& res
    );
    void print() const;
    void scanf();
};

#endif
