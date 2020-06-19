#include <stdio.h>
#include "icode.h"
#include "lt.h"
#include "nt.h"

static void print_string_const(const char *str);

void ICommand::print() const {
    char line[32];
    if (label != 0)
        printf("%s ", Label(label).toString(line, 32));
    switch (type) {
    case int_const:
        printf("int_const %d", int_value);
        break;
    case double_const:
        printf("double_const %lf", double_value);
        break;
    case string_const:
        //... printf("string_const \"%s\"", string_value.c_str());
        printf("string_const \"");
        print_string_const(string_value.c_str());
        printf("\"");
        break;
    case assign:
        printf("assign");
        break;
    case name:
        printf("name %s", name_ptr->name.c_str());
        break;
    case name_lvalue:
        printf("name_lvalue %s", name_ptr->name.c_str());
        break;
    case def_variable:
        printf(
            "def_variable %s of type ", 
            name_ptr->name.c_str()
        );

        LBaseType: ;
        switch(ext) {
        case TYPE_INT:
            printf("int"); break;
        case TYPE_DOUBLE:
            printf("double"); break;
        case TYPE_STRING:
            printf("string"); break;
        }
        break;
    case def_array:
        printf(
            "def_array %s of type ", 
            name_ptr->name.c_str()
        );
        goto LBaseType;
        // break;
    case array_element:
        printf("array_element");
        break;
    case array_lvalue:
        printf("array_lvalue");
        break;
    case plus:
        printf("plus");
        break;
    case minus:
        printf("minus");
        break;
    case mul:
        printf("mul");
        break;
    case div:
        printf("div");
        break;
    case mod:
        printf("mod");
        break;
    case uminus:
        printf("uminus");
        break;
    case cmp:
        printf("cmp");
        break;
    case go_to:
        printf(
            "goto %s", Label(goto_label).toString(line, 32)
        );
        break;
    case if_goto:
        printf("if_goto ");
        switch (ext) {
        case eq:
            printf("eq ");
            break;
        case ne:
            printf("ne ");
            break;
        case gt:
            printf("gt ");
            break;
        case ge:
            printf("ge ");
            break;
        case lt:
            printf("lt ");
            break;
        case le:
            printf("le ");
            break;
        default:
            printf("illegal condition ");
        }
        printf("%s", Label(goto_label).toString(line, 32));
        break;
    case input:
        printf("input");
        break;
    case output:
        printf("output");
        break;
    case nop:
        printf("nop");
        break;
    case sound:
        printf("sound");
        break;
    case mouth:
        printf("mouth");
        break;
    case stop:
        printf("stop");
        break;
    case rc:
        printf("rc");
        break;
    case dima:
        printf("dima");
        break;
    case sleep:
        printf("sleep");
        break;
    case right:
        printf("right");
        break;
    case left:
        printf("left");
        break;
    case forward:
        printf("forward");
        break;
    case back:
        printf("back");
        break;
    default:
        printf("Illegal command");
    }
    printf("\n");
}

IProgram::iterator IProgram::addNewCommand() {
    push_back(ICommand());
    IProgram::iterator i = end();
    --i;
    return i;
}

void IProgram::print() const {
    IProgram::const_iterator i = begin();
    while (i != end()) {
        i->print();
        ++i;
    }
}

static void print_string_const(const char *str) {
    const char *p = str;
    if (p == NULL) {
        printf("NULL"); return;
    }
    char c;
    while ((c = *p) != 0) {
        switch(c) {
        case '\n': printf("\\n"); break;
        case '\r': printf("\\r"); break;
        case '\t': printf("\\r"); break;
        default:
            printf("%c", c); break;
        }
        ++p;
    }
}
