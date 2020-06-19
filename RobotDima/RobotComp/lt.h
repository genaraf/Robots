//
// Label table
//
#ifndef LT_H
#define LT_H
#include <set>
#include <string.h>
#include "icode.h"

using std::set;

class ICommand;

// Labels look like ".L102:"
const char* const LABEL_PREFIX = ".L";
const char* const LABEL_SUFFIX = ":";

class Label {
public:
    int number;
    mutable IProgram::const_iterator location;

    Label():
        number(0),
        location()
    {}

    Label(int n):
        number(n),
        location()
    {}

    Label(const char* str);

    bool operator==(const Label& l) const {
        return (number == l.number);
    }

    bool operator!=(const Label& n) const {
        return (!(*this == n));
    }

    bool operator<=(const Label& l) const {
        return (number <= l.number);
    }

    bool operator<(const Label& l) const {
        return (number < l.number);
    }

    bool operator>=(const Label& l) const {
        return (number >= l.number);
    }

    bool operator>(const Label& l) const {
        return (number > l.number);
    }

    operator const char*() const;

    char* toString(char *buffer, int maxLen) const;
};

//
// Label table
//
class LabelTable: public set<Label> {
public:
    const Label* findLabel(int n) const;
    const Label* addLabel(int n);
};

#endif
