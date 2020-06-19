/*
 * Parser & Scanner Common Definitions
 */
#ifndef PARSER_H
#define PARSER_H

#include "lextypes.h"
#include "nt.h"
#include "lt.h"
#include "icode.h"
#include "tree.h"

using std::string;

extern NameTable name_table;

/* The type of values associated with terminals and nonterminals */
struct yys {
    string name;
    int int_value;
    double double_value;
    TreeNode* tree;
    NameDef* nameDef;
    const Label* label1;
    const Label* label2;
    const Label* saved_label1;
    const Label* saved_label2;
};

#define YYSTYPE yys

#include "comp.h"   	/* File generated automatically by bison: */
                        /* contains the definitions of terminals  */

extern FILE* yyin;      /* Scanner input */

int yylex();              /* Prototype of the scanner function "next lexeme" */
int yyerror(const char*); /* Prototype of the error diagnostics function     */

extern int yylineno;

#endif
