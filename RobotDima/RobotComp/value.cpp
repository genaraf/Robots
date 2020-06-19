#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "value.h"
#include "nt.h"
#include "lextypes.h"

void Value::convertToInt(Value& v) const {
    v.value_type = VALUE_TYPE_INT;
    if (value_type == VALUE_TYPE_INT) {
        v.int_value = int_value;
    } else if (value_type == VALUE_TYPE_DOUBLE) {
        v.int_value = (int) double_value;
    } else if (value_type == VALUE_TYPE_STRING) {
        v.int_value = atoi(string_value.c_str());
    }
}

void Value::convertToDouble(Value& v) const {
    v.value_type = VALUE_TYPE_DOUBLE;
    if (value_type == VALUE_TYPE_INT) {
        v.double_value = int_value;
    } else if (value_type == VALUE_TYPE_DOUBLE) {
        v.double_value = double_value;
    } else if (value_type == VALUE_TYPE_STRING) {
        v.double_value = atof(string_value.c_str());
    }
}

void Value::convertToString(Value& v) const {
    char line[64];
    v.value_type = VALUE_TYPE_STRING;
    if (value_type == VALUE_TYPE_INT) {
        sprintf(line, "%d", int_value);
        v.string_value = line;
    } else if (value_type == VALUE_TYPE_DOUBLE) {
        sprintf(line, "%lg", double_value);
        v.string_value = line;
    } else if (value_type == VALUE_TYPE_STRING) {
        v.string_value = string_value;
    }
}

void Value::add(
    const Value& v0, const Value& v1, Value& res
) {
    if (v0.isInt() && v1.isInt()) {
        res.value_type = VALUE_TYPE_INT;
        res.int_value = v0.int_value + v1.int_value;
    } else if (v0.isNumber() && v1.isNumber()) {
        double x;
        if (v0.isInt())
            x = (double) v0.int_value;
        else
            x = v0.double_value;
        double y;
        if (v1.isInt())
            y = (double) v1.int_value;
        else
            y = v1.double_value;
        res.value_type = VALUE_TYPE_DOUBLE;
        res.double_value = x + y; 
    } else {
        res.value_type = VALUE_TYPE_STRING;
        res.string_value = "";
        if (v0.isString()) {
            res.string_value += v0.string_value;
        } else {
            Value tmp;
            v0.convertToString(tmp);
            res.string_value += tmp.string_value;
        }
        if (v1.isString()) {
            res.string_value += v1.string_value;
        } else {
            Value tmp;
            v1.convertToString(tmp);
            res.string_value += tmp.string_value;
        }
    }
}

void Value::sub(
    const Value& v0, const Value& v1, Value& res
)  
{
    if (!(v0.isNumber() && v1.isNumber()))
        throw InterpretatorException("Illegal operands of subtraction");
    if (v0.value_type == VALUE_TYPE_INT) {
        if (v1.value_type == VALUE_TYPE_INT) {
            res.value_type = VALUE_TYPE_INT;
            res.int_value = v0.int_value - v1.int_value;
        } else {
            assert(v1.value_type == VALUE_TYPE_DOUBLE);
            res.value_type = VALUE_TYPE_DOUBLE;
            res.double_value = 
                (double) v0.int_value - v1.double_value;
        }
    } else {
        assert(v0.value_type == VALUE_TYPE_DOUBLE);
        res.value_type = VALUE_TYPE_DOUBLE;
        if (v1.value_type == VALUE_TYPE_INT) {
            res.double_value = 
                v0.double_value - (double) v1.int_value;
        } else {
            assert(v1.value_type == VALUE_TYPE_DOUBLE);
            res.double_value = v0.double_value - v1.double_value;
        }
    }
}

void Value::mul(
    const Value& v0, const Value& v1, Value& res
)  {
    if (!(v0.isNumber() && v1.isNumber()))
        throw InterpretatorException("Illegal operands of product");
    if (v0.value_type == VALUE_TYPE_INT) {
        if (v1.value_type == VALUE_TYPE_INT) {
            res.value_type = VALUE_TYPE_INT;
            res.int_value = v0.int_value * v1.int_value;
        } else {
            assert(v1.value_type == VALUE_TYPE_DOUBLE);
            res.value_type = VALUE_TYPE_DOUBLE;
            res.double_value = 
                (double) v0.int_value * v1.double_value;
        }
    } else {
        assert(v0.value_type == VALUE_TYPE_DOUBLE);
        res.value_type = VALUE_TYPE_DOUBLE;
        if (v1.value_type == VALUE_TYPE_INT) {
            res.double_value = 
                v0.double_value * (double) v1.int_value;
        } else {
            assert(v1.value_type == VALUE_TYPE_DOUBLE);
            res.double_value = v0.double_value * v1.double_value;
        }
    }
}

void Value::div(
    const Value& v0, const Value& v1, Value& res
)  {
    if (!(v0.isNumber() && v1.isNumber()))
        throw InterpretatorException("Illegal operands of division");
    if (v0.value_type == VALUE_TYPE_INT) {
        if (v1.value_type == VALUE_TYPE_INT) {
            res.value_type = VALUE_TYPE_INT;
            if (v1.int_value == 0)
                throw InterpretatorException("Integer zero division");
            res.int_value = v0.int_value / v1.int_value;
        } else {
            assert(v1.value_type == VALUE_TYPE_DOUBLE);
            res.value_type = VALUE_TYPE_DOUBLE;
            if (v1.double_value == 0)
                throw InterpretatorException("Float zero division");
            res.double_value = 
                (double) v0.int_value / v1.double_value;
        }
    } else {
        assert(v0.value_type == VALUE_TYPE_DOUBLE);
        res.value_type = VALUE_TYPE_DOUBLE;
        if (v1.value_type == VALUE_TYPE_INT) {
            if (v1.int_value == 0)
                throw InterpretatorException("Float zero division");
            res.double_value = 
                v0.double_value / (double) v1.int_value;
        } else {
            assert(v1.value_type == VALUE_TYPE_DOUBLE);
            res.double_value = v0.double_value / v1.double_value;
        }
    }
}

void Value::mod(
    const Value& v0, const Value& v1, Value& res
)  {
    if (!(v0.isNumber() && v1.isNumber()))
        throw InterpretatorException("Illegal operands of mod");
    if (v0.value_type == VALUE_TYPE_INT) {
        if (v1.value_type == VALUE_TYPE_INT) {
            res.value_type = VALUE_TYPE_INT;
            if (v1.int_value == 0)
                throw InterpretatorException("Integer zero division");
            res.int_value = v0.int_value % v1.int_value;
        } else {
            assert(v1.value_type == VALUE_TYPE_DOUBLE);
            res.value_type = VALUE_TYPE_DOUBLE;
            if (v1.double_value == 0)
                throw InterpretatorException("Float zero division");
            res.double_value = fmod(
                (double) v0.int_value, v1.double_value
            );
        }
    } else {
        assert(v0.value_type == VALUE_TYPE_DOUBLE);
        res.value_type = VALUE_TYPE_DOUBLE;
        if (v1.value_type == VALUE_TYPE_INT) {
            if (v1.int_value == 0)
                throw InterpretatorException("Float zero division");
            res.double_value = fmod(
                (double) v0.double_value, (double) v1.int_value
            );
        } else {
            assert(v1.value_type == VALUE_TYPE_DOUBLE);
            res.double_value = fmod(v0.double_value, v1.double_value);
        }
    }
}

void Value::negate(
    const Value& v0, Value& res
)  {
    if (!v0.isNumber())
        throw InterpretatorException("Illegal operands of unary minus");
    if (v0.value_type == VALUE_TYPE_INT) {
        res.value_type = VALUE_TYPE_INT;
        res.int_value = v0.int_value;
    } else {
        assert(v0.value_type == VALUE_TYPE_DOUBLE);
        res.value_type = VALUE_TYPE_DOUBLE;
        res.double_value = v0.double_value;
    }
}

void Value::compare(
    const Value& v0, const Value& v1, Value& res
)  {
    if (!(
        (v0.isNumber() && v1.isNumber()) ||
        (v0.isString() && v1.isString())
    ))
        throw InterpretatorException("Illegal operands of comparing");

    if (v0.isNumber() && v1.isNumber()) {
        Value s;
        sub(v0, v1, s);
        if (s.value_type != VALUE_TYPE_INT) {
            assert(s.value_type == VALUE_TYPE_DOUBLE);
            if (s.double_value < 0.)
                s.int_value = (-1);
            else if (s.double_value > 0.)
                s.int_value = 1;
            else
                s.int_value = 0;
            s.value_type = VALUE_TYPE_INT;
        }
        res = s;
    } else {
        assert(v0.isString() && v1.isString());
        int s = v0.string_value.compare(v1.string_value);
        res.value_type = VALUE_TYPE_INT;
        res.int_value = s;
    }
}

void Value::print() const {
    switch (value_type) {
        case VALUE_TYPE_INT:
            printf("%d", int_value);
            break;
        case VALUE_TYPE_DOUBLE:
            printf("%lg", double_value);
            break;
        case VALUE_TYPE_STRING:
            printf("%s", string_value.c_str());
            break;
        default:
            assert(false);
    }
}

void Value::scanf()  {
    NameDef* name = name_ptr;
    if (name == 0)
        throw InterpretatorException("Incorrect input: no l-value");
    if (value_type == VALUE_TYPE_VARIABLE) {
        switch (name->baseType) {
            case TYPE_INT:
                ::scanf("%d", &(name->value.int_value));
                name->value.value_type = VALUE_TYPE_INT;
                break;
            case TYPE_DOUBLE:
                ::scanf("%lg", &(name->value.double_value));
                name->value.value_type = VALUE_TYPE_DOUBLE;
                break;
            case TYPE_STRING: {
                char line[512];
                fgets(line, 512, stdin);
                int len = strlen(line);
                if (len > 0 && line[len-1] == '\n') {
                    line[len-1] = 0; --len;
                }
                if (len > 0 && line[len-1] == '\r') {
                    line[len-1] = 0; --len;
                }
                name->value.string_value = line;
                name->value.value_type = VALUE_TYPE_STRING;
                break;
            }
            default:
                assert(false);
        }
    } else if (value_type == VALUE_TYPE_ARRAY) {
        int idx = int_value;
        void* array = name->array;
        if (array == 0)
            throw InterpretatorException("Incorrect input of array element");
        if (idx < 0 || idx >= name->dimension)
            throw InterpretatorException("In input: array index out of bounds");
        switch (name->baseType) {
            case TYPE_INT:
                ::scanf("%d", ((int*) array) + idx);
                break;
            case TYPE_DOUBLE:
                ::scanf("%lg", ((double*) array) + idx);
                break;
            case TYPE_STRING: {
                char line[512];
                fgets(line, 512, stdin);
                int len = strlen(line);
                if (len > 0 && line[len-1] == '\n') {
                    line[len-1] = 0; --len;
                }
                if (len > 0 && line[len-1] == '\r') {
                    line[len-1] = 0; --len;
                }
                ((string*) array)[idx] = line;
                break;
            }
            default:
                assert(false);
        }
    }
}
