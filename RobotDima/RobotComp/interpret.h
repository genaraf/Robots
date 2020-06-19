
#ifndef INTERPRET_H
#define INTERPRET_H

#include <thread>        
#include <chrono>
#include <unistd.h>
#include "value.h"      // Stack values
#include "nt.h"         // Name table
#include "icode.h"      // Intermediate code
#include "lextypes.h"   // Types of lexemes
#include "lt.h"         // Label table

// Stack of interpretator

const int INITIAL_STACK_SIZE = 1024;
const int STACK_EXTENT = 512;
const int STACK_MAX_DEPTH = 65536;

class StackException {
public:
    const char* reason;
    StackException(): reason("") {}
    StackException(const char* cause): reason(cause) {}
};

class OutOfRangeException {
public:
    const char* reason;
    OutOfRangeException(): reason("") {}
    OutOfRangeException(const char* cause): reason(cause) {}
};

class ExecStack {
public:
    int max_size;
    int depth;
    Value* stack_elements;

    ExecStack():
        max_size(INITIAL_STACK_SIZE),
        depth(0),
        stack_elements(new Value[INITIAL_STACK_SIZE])
    {}

    ExecStack(int maxSize):
        max_size(maxSize),
        depth(0),
        stack_elements(new Value[maxSize])
    {}

    void push(const Value& v) {
        if (depth >= max_size)
            extend();
        stack_elements[depth] = v;
        ++depth;
    }

    Value pop() {
        if (depth <= 0)
            throw StackException("Stack empty");
        Value v = stack_elements[depth - 1];
        --depth;
        return v;
    }

    void pop(Value& v) {
        if (depth <= 0)
            throw StackException("Stack empty");
        v = stack_elements[depth - 1];
        --depth;
    }

    Value& top() {
        if (depth <= 0)
            throw StackException("Stack empty");
        return stack_elements[depth - 1];
    }

    const Value& top() const  {
        if (depth <= 0)
            throw StackException("Stack empty");
        return stack_elements[depth - 1];
    }

    void extend() {
        if (max_size + STACK_EXTENT > STACK_MAX_DEPTH)
            throw StackException("Stack overflow");
        Value* newStack = new Value[max_size + STACK_EXTENT];
        for (int i = 0; i < depth; ++i)
            newStack[i] = stack_elements[i];
        delete[] stack_elements;
        stack_elements = newStack;
        max_size += STACK_EXTENT;
    }

    void initialize() {
        depth = 0;
    }

    int size() const { return depth; }
};

class ICodeInterpretator {
public:
    ExecStack stack;
    //... const ICommand* currentCommand;
    const IProgram* program;
    IProgram::const_iterator currentCommand;
public:
    ICodeInterpretator():
        program(0),
        currentCommand()
    {}

    void initialize(const IProgram* icode);
    void execute();
    void executeCommand();
    void printStackTop() const;
};

#endif
