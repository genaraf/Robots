//
// Intermediate code used for interpretation
//
#ifndef ICODE_H
#define ICODE_H

#include <list>
#include <string>
//... #include "nt.h"         // Name table
//... #include "lt.h"         // Label table

using std::list;
using std::string;

class Label;
class NameDef;

class ICommand {
public:
    enum {
        int_const = 0,
        double_const,
        string_const,
        assign,
        name,           // Value of name
        name_lvalue,    // Reference to name
        def_variable,   // Variable definition
        def_array,      // Array definition
        array_element,  // Value of array element
        array_lvalue,   // l-value of array element
        plus,
        minus,
        mul,
        div,
        mod,
        uminus,
        cmp,
        go_to,
        if_goto,        // Conditional goto
        eq, ne, gt, ge, lt, le, // conditions
        input,
        output,
        rc,
        dima,
        right,
        left,
        forward,
        back,
        sleep,
        sound,
        stop,
        mouth,
        nop             // No operation
    };

    int label;          // Label of this command
    const Label* label_ptr; // Pointer to label table
    int type;           // Type of command
    int ext;            // Additional information (base type, 
                        //     condition for if_goto, etc.)
    NameDef* name_ptr;  // Pointer to name table
    int goto_label;     // Label for goto
    const Label* goto_label_ptr; // Pointer to label table
    int int_value;      // Used for constants, but may contain
                        //     an additional information
    double double_value;
    string string_value;

    ICommand():
        label(0),
        label_ptr(0),
        type(0),
        ext(0),
        name_ptr(0),
        goto_label(0),
        goto_label_ptr(0),
        int_value(0),
        double_value(0),
        string_value()
    {}

    void print() const;
};


class IProgram: public list<ICommand> {
public:
    IProgram::iterator addNewCommand();
    void print() const;
};

#endif
