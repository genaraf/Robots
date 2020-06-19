//
// Name table
//
#ifndef NT_H
#define NT_H
#include <set>
#include <assert.h>
#include "lextypes.h"
#include "value.h"

using std::string;
using std::set;

class NameDef {
public:
    string name;
    int baseType;
    int dimension;
    int numDefinitions;
    int numReads;
    int numWrites;
    int modifiers;
    // Bits in modifiers
    enum {
        NAME_VARIABLE = 1,
        NAME_ARRAY = 2,
        NAME_FUNCTION = 4
    };
    Value value; // value of simple variable
    void* array; // array of values for array variable

    NameDef():
        name(),
        baseType(TYPE_UNDEFINED),
        dimension(0),
        numDefinitions(0),
        numReads(0),
        numWrites(0),
        modifiers(0),
        value(),
        array(0)
    {}

    NameDef(const NameDef& n):
        name(n.name),
        baseType(n.baseType),
        dimension(n.dimension),
        numDefinitions(n.numDefinitions),
        numReads(n.numReads),
        numWrites(n.numWrites),
        modifiers(n.modifiers),
        value(n.value),
        array(0)
    {}

    NameDef(const char* str):
        name(str),
        baseType(TYPE_UNDEFINED),
        dimension(0),
        numDefinitions(0),
        numReads(0),
        numWrites(0),
        modifiers(0),
        value(),
        array(0)
    {}

    NameDef& operator=(const NameDef& n) {
        name = n.name;
        baseType = n.baseType;
        dimension = n.dimension;
        numDefinitions = n.numDefinitions;
        numReads = n.numReads;
        numWrites = n.numWrites;
        modifiers = n.modifiers;
        value = n.value;
        array = 0;
        return *this;
    }

    bool operator==(const NameDef& n) const {
        return (name.compare(n.name) == 0);
    }

    bool operator==(const char* str) const {
        return (name.compare(str) == 0);
    }

    bool operator<=(const char* str) const {
        return (name.compare(str) <= 0);
    }

    bool operator<(const char* str) const {
        return (name.compare(str) < 0);
    }

    bool operator!=(const NameDef& n) const {
        return (!(*this == n));
    }

    bool operator!=(const char* str) const {
        return (!(*this == str));
    }

    bool operator>(const char* str) const {
        return (!(*this <= str));
    }

    bool operator>=(const char* str) const {
        return (!(*this < str));
    }

    operator const char*() const {
        return name.c_str();
    }

    ~NameDef() {
        if (array != 0) {
            switch (baseType) {
            case TYPE_INT:
                delete[] ((int*) array); break;
            case TYPE_DOUBLE:
                delete[] ((double*) array); break;
            case TYPE_STRING:
                delete[] ((string*) array); break;
            default:
                assert(false); // Illegal type of array
            }
        }
    }

    void allocateArray(int n);
};

//
// Name table
//
class NameTable: public set<NameDef> {
public:
    NameDef* findName(const char *name) const;
    NameDef* findName(const string n) const {
        return findName(n.c_str());
    }

    NameDef* addName(const char *name);
    NameDef* addName(const string n) {
        return addName(n.c_str());
    }
};

#endif
