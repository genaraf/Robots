#include <utility>
#include "nt.h"

void NameDef::allocateArray(int n) {
    assert(n > 0);
    if (n <= 0)
        n = 1;
    if (baseType == TYPE_INT) {
        array = (void *) new int[n];
    } else if (baseType == TYPE_DOUBLE) {
        array = (void *) new double[n];
    } else if (baseType == TYPE_STRING) {
        array = (void *) new string[n];
    } else {
        assert(false);
    }
    dimension = n;
}

NameDef* NameTable::findName(const char *name) const {
    const_iterator i = find(name);
    if (i == end())
        return 0;
    else
        return const_cast<NameDef*>(&(*i));
}

NameDef* NameTable::addName(const char *name) {
    iterator i = find(name);
    if (i == end()) {
        std::pair<iterator, bool> p = insert(name);
        assert(p.second);
        i = p.first;
    }        
    return const_cast<NameDef*>(&(*i));
}
