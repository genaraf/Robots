#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <utility>
#include "lt.h"

static char cur_label[32];

Label::operator const char*() const {
    return toString(cur_label, 32);
}

char* Label::toString(char *buffer, int maxLen) const {
    char line[32];
    sprintf(line, "%s%d%s", LABEL_PREFIX, number, LABEL_SUFFIX);
    strncpy(buffer, line, maxLen);
    return buffer;
}

const Label* LabelTable::findLabel(int n) const {
    Label ln(n);
    const_iterator i = find(ln);
    if (i != end())
        return &(*i);
    else
        return 0;
}

const Label* LabelTable::addLabel(int n) {
    const Label *lab = findLabel(n);
    if (lab != 0)
        return lab;
    std::pair<iterator, bool> p = insert(Label(n));
    assert(p.second);
    return &(*p.first);
}
