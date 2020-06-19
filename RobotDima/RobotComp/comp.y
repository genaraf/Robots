%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "interpret.h"

static int base_type = TYPE_UNDEFINED;
NameTable name_table;
LabelTable label_table;
IProgram icode;
static char error_text[128];

static int binOper = 0;

static const Label* breakLabel = 0;
static const Label* continueLabel = 0;
static const Label* endifLabel = 0;

static void genExpr(const TreeNode* tree);

// Either trueLabel or falseLabel == 0
static void genLExpr(
    const TreeNode* tree,   // Represents a logical expression
    const Label* trueLabel,
    const Label* falseLabel
);

static void genNop(const Label* label);
static void genGoto(const Label* label);
static const Label* genLabel();
static void checkNames();
static void printWarning(const char *text);
static void printError(const char *text);

%}

%term NAME INT_CONST DOUBLE_CONST STRING_CONST
%term WHILE ENDWHILE IF ENDIF ELSE ELSEIF TYPE BREAK CONTINUE
%term SM CM LBR RBR LPAR RPAR ILLEGAL
%term INPUT OUTPUT OUTPUTLN END RC SLEEP RIGHT LEFT BACK FORWARD DIMA STOP SOUND MOUTH
%right ASG
%nonassoc RELOP
%left LOR
%left LAND
%left LNOT
%left PLUS MINUS
%left MUL DIV MOD
%nonassoc UMINUS

%%

program : stms {
              checkNames();
              icode.print();
              printf("--------\n\n");
          }
        | stms END {
              checkNames();
              icode.print();
              printf("--------\n\n");
              // YYACCEPT;
              return 0;
          }
        ;

stms    : stm {
              // printf("stms: stm\n"); 
          }
        | stms stm { 
              // printf("stms: stms stm\n");
          }
        ;

stm     : decl {
              // printf("stm: decl\n");
          }
        | oper {
              // printf("stm: oper\n"); 
          }
        | error SM {
              // printf("stm: error SM\n"); 
          }
        | error ENDIF {
              // printf("stm: error ENDIF\n"); 
          }
        | error ENDWHILE { 
              // printf("stm: error WHILE\n"); 
          }
        ;

decl    : type defs SM {
              // printf("decl\n"); 
          }
        ;

type    : TYPE {
              base_type = $1.int_value;
          }
        ;

defs    : def {
              // printf("def\n"); 
          }
        | defs CM def {
              // printf("defs CM def\n"); 
          }
        ;

def     : NAME {
              NameDef* n = name_table.addName($1.name);
              n->baseType = base_type;
              n->dimension = 0;
              ++(n->numDefinitions);
              n->modifiers |= NameDef::NAME_VARIABLE;
              if (n->numDefinitions > 1) {
                  sprintf(
                      error_text, "Multiple definition of %s",
                      n->name.c_str()
                  );
                  yyerror(error_text);
              }
              // printf("variable %s\n", n->name);

              IProgram::iterator c = icode.addNewCommand();
              c->type = ICommand::def_variable;
              c->ext = base_type;
              c->name_ptr = n;
          }
        | NAME LBR expr RBR {
              // $3.tree->print();
              genExpr($3.tree);

              NameDef* n = name_table.addName($1.name);
              n->baseType = base_type;
              n->dimension = 1; // Array
              ++(n->numDefinitions);
              n->modifiers |= NameDef::NAME_ARRAY;
              if (n->numDefinitions > 1) {
                  sprintf(
                      error_text, "Multiple definition of %s",
                      n->name.c_str()
                  );
                  yyerror(error_text);
              }
              // printf("variable %s\n", n->name);

              IProgram::iterator c = icode.addNewCommand();
              c->type = ICommand::def_array;
              c->ext = base_type;
              c->name_ptr = n;
          }
        ;

oper    : assign
        | if
        | while
        | BREAK SM {
              if (breakLabel == 0) {
                  yyerror("break out of loop");
              } else {
                  genGoto(breakLabel);
              }
          }
        | CONTINUE SM {
              if (continueLabel == 0) {
                  yyerror("continue out of loop");
              } else {
                  genGoto(continueLabel);
              }
          }
        | input
        | output
        | outputln
        | rc
        | forward
        | back
        | right
        | left
        | sleep
        | dima
        | sound
        | mouth
        | stop
        ;

assign  : lval ASG expr SM {
              // printf("assign: right expr:\n");
              // $3.tree->print();

              genExpr($3.tree);

              IProgram::iterator c = icode.addNewCommand();
              c->type = ICommand::assign;
          }
        ;

lval    : lval_name
        | lval_name LBR expr RBR {
              genExpr($3.tree);
              IProgram::iterator c = icode.addNewCommand();
              c->type = ICommand::array_lvalue;
          }
        ;

lval_name : NAME {
                NameDef* n = name_table.addName($1.name);
                // printf("l-value %s\n", n->name);
                ++(n->numWrites);

                IProgram::iterator c = icode.addNewCommand();
                c->type = ICommand::name_lvalue;
                c->name_ptr = n;
            }
          ;

expr    : expr PLUS expr {
              binOper = TreeNode::plus;
              LBinOper: ;
              TreeNode* t = new TreeNode();
              t->nodeType = binOper;
              t->addLeftSon($1.tree);
              t->addRightSon($3.tree);
              $$.tree = t;
          }
        | expr MINUS expr {
              binOper = TreeNode::minus;
              goto LBinOper;
          }
        | expr MUL expr {
              binOper = TreeNode::mul;
              goto LBinOper;
          }
        | expr DIV expr {
              binOper = TreeNode::div;
              goto LBinOper;
          }
        | expr MOD expr {
              binOper = TreeNode::mod;
              goto LBinOper;
          }
        | MINUS expr %prec UMINUS {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::uminus;
              t->addLeftSon($2.tree);
              $$.tree = t;
          }
        | NAME {
              TreeNode* t = new TreeNode();
              NameDef* n = name_table.addName($1.name);
              ++(n->numReads);
              t->nodeType = TreeNode::name;
              t->nameDef = n;
              $$.tree = t;
          }
        | NAME LBR expr RBR {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::array_idx;
              NameDef* n = name_table.addName($1.name);
              ++(n->numReads);
              TreeNode* t1 = new TreeNode();
              t1->nodeType = TreeNode::name;
              t1->nameDef = n;
              t->addLeftSon(t1);
              t->addRightSon($3.tree);
              $$.tree = t;
          }
        | INT_CONST {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::int_const;
              t->intValue = $1.int_value;
              $$.tree = t;
          }
        | DOUBLE_CONST {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::double_const;
              t->doubleValue = $1.double_value;
              $$.tree = t;
          }
        | STRING_CONST {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::string_const;
              t->stringValue = $1.name;
              $$.tree = t;
          }
        | LPAR expr RPAR { $$.tree = $2.tree; }
        ;

if      : ifh stms ENDIF { 
              // printf("if: ifh stmd ENDIF\n");
              genNop($1.label1);        // Label of endif
          }
        | ifh stms {
              const Label* endif_label = genLabel();
              $1.label2 = endif_label;
              $1.saved_label2 = endifLabel;
              endifLabel = endif_label;
              genGoto(endif_label);
              genNop($1.label1);
          }
          else ENDIF {
              genNop($1.label2);
              endifLabel = $1.saved_label2;
          }
        | ifh stms {
              const Label* endif_label = genLabel();
              $1.label2 = endif_label;
              $1.saved_label2 = endifLabel;  // Save previous endif label
              endifLabel = endif_label;
              genGoto(endif_label);
              genNop($1.label1);
          }
          elseifs ENDIF {
              genNop($1.label2);
              endifLabel = $1.saved_label2;
          }
        ;

ifh     : IF LPAR lexpr RPAR { 
              // printf("ifh\n");
              const Label* l = genLabel();
              // $3.tree->print();
              genLExpr($3.tree, 0, l);
              $$.label1 = l;
          }
        ;

elseifs : elseiflist
        | elseiflist else
        ;

elseiflist : elseif
           | elseiflist elseif
           ;

else    : ELSE stms
        ;

elseif  : elseifh stms {
              genGoto(endifLabel);
              genNop($1.label1);
          }
        ;

elseifh : ELSEIF LPAR lexpr RPAR {
              const Label* l = genLabel();
              genLExpr($3.tree, 0, l);
              $$.label1 = l;
          }
        ;

lexpr   : expr RELOP expr {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::relop;
              t->intValue = $2.int_value; // operation: RELOP_EQ, etc.
              t->addLeftSon($1.tree);
              t->addRightSon($3.tree);
              $$.tree = t;
          }
        | lexpr LOR lexpr {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::lor;
              t->addLeftSon($1.tree);
              t->addRightSon($3.tree);
              $$.tree = t;
          }
        | lexpr LAND lexpr {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::land;
              t->addLeftSon($1.tree);
              t->addRightSon($3.tree);
              $$.tree = t;
          }
        | LNOT lexpr {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::lnot;
              t->addLeftSon($2.tree);
              $$.tree = t;
          }
        | LPAR lexpr RPAR {
              $$.tree = $2.tree;
          }
        ;

while   : whileh stms ENDWHILE { 
              // printf("while\n");
              // Restore loop labels
              genGoto($1.label1);
              genNop($1.label2);
              continueLabel = $1.saved_label1;
              breakLabel = $1.saved_label2;
          }
        ;

whileh  : WHILE LPAR lexpr RPAR { 
              // printf("whileh\n");
              const Label* beginLab = genLabel();
              const Label* endLab = genLabel();
              $$.label1 = beginLab;
              $$.label2 = endLab;

              genNop(beginLab);
              genLExpr($3.tree, 0, endLab);

              $$.saved_label1 = continueLabel;
              $$.saved_label2 = breakLabel;
              continueLabel = beginLab;
              breakLabel = endLab;
          }
        ;

input   : INPUT inputlst SM
        ;

inputlst : lval {
              LInput: ;
              IProgram::iterator c = icode.addNewCommand();
              c->type = ICommand::input;
          }
        | inputlst CM lval { goto LInput; }
        ;

output  : OUTPUT outputlst SM
        ;



rc : RC LPAR expr CM expr RPAR SM{
            genExpr($3.tree);
            genExpr($5.tree);
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::rc;
        }
    ;

back : BACK SM{
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::back;
        }
    ;
forward : FORWARD SM{
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::forward;
        }
    ;
right : RIGHT SM{
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::right;
        }
    ;
left : LEFT SM{
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::left;
        }
    ;
dima : DIMA SM{
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::dima;
        }
    ;
sleep : SLEEP LPAR expr RPAR SM{
            genExpr($3.tree);
            IProgram::iterator c = icode.addNewCommand();
           // c->type = ICommand::int_const;
            //c->int_value = $3.int_value;
            //c = icode.addNewCommand();
            c->type = ICommand::sleep;
        }
        | SLEEP LPAR RPAR SM{
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::int_const;
            c->int_value = 1000;
            c = icode.addNewCommand();
            c->type = ICommand::sleep;
        }
    ;
stop : STOP SM{
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::stop;
        }
    ;
sound : SOUND LPAR expr RPAR SM{
            genExpr($3.tree);
            IProgram::iterator c = icode.addNewCommand();
           // c->type = ICommand::int_const;
            //c->int_value = $3.int_value;
            //c = icode.addNewCommand();
            c->type = ICommand::sound;
        }
        | SOUND LPAR RPAR SM{
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::int_const;
            c->int_value = 1;
            c = icode.addNewCommand();
            c->type = ICommand::sound;
        }
    ;
mouth : MOUTH LPAR expr RPAR SM{
            genExpr($3.tree);
            IProgram::iterator c = icode.addNewCommand();
           // c->type = ICommand::int_const;
            //c->int_value = $3.int_value;
            //c = icode.addNewCommand();
            c->type = ICommand::mouth;
        }
        | MOUTH LPAR RPAR SM{
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::int_const;
            c->int_value = 1;
            c = icode.addNewCommand();
            c->type = ICommand::mouth;
        }
    ;
    
outputln : OUTPUTLN SM {
              LNewLine: ;   
              // Output the "new line" character
              IProgram::iterator c = icode.addNewCommand();
              c->type = ICommand::string_const;
              c->string_value = "\n";
                
              c = icode.addNewCommand();
              c->type = ICommand::output;
          }
        | OUTPUTLN outputlst SM { goto LNewLine; }
        ;


outputlst : expr {
              LOutput: ;
              genExpr($1.tree);

              IProgram::iterator c = icode.addNewCommand();
              c->type = ICommand::output;
          }
        | outputlst CM expr { goto LOutput; }
        ;


%%

int main(int argc, char *argv[]) {
    FILE* f = NULL;
    if (argc > 1) {
        // Open an input file
        FILE* f = fopen(argv[1], "r");
        if (f != NULL)
            yyin = f;
    }
    yyparse();
    if (f != NULL)
        fclose(f);

    ICodeInterpretator interpretator;
    interpretator.initialize(&icode);
    try {
        interpretator.execute();
        /*
        printf("stack depth = %d\n", interpretator.stack.size());
        if (interpretator.stack.size() > 0) {
            printf("stack top: ");
            interpretator.printStackTop();
            printf("\n");
        }
        */
    } catch (InterpretatorException& e) {
        printf(
            "InterpretatorException: %s\n",
            e.reason
        );
        exit(1);
    } catch (StackException& e) {
        printf(
            "StackException: %s\n",
            e.reason
        );
        exit(1);
    } catch (OutOfRangeException& e) {
        printf(
            "OutOfRangeException: %s\n",
            e.reason
        );
        exit(1);
    }
    return 0;
}

int yyerror(const char *s) {
    printf("%s in line %d\n", s, yylineno);
    return 0;
}

static void genExpr(const TreeNode* tree) {
    if (tree == 0)
        return;
    IProgram::iterator c;
    int binop;

    switch(tree->nodeType) {
    case TreeNode::int_const:
        c = icode.addNewCommand();
        c->type = ICommand::int_const;
        c->int_value = tree->intValue;
        break;
    case TreeNode::double_const:
        c = icode.addNewCommand();
        c->type = ICommand::double_const;
        c->double_value = tree->doubleValue;
        break;
    case TreeNode::string_const:
        c = icode.addNewCommand();
        c->type = ICommand::string_const;
        c->string_value = tree->stringValue;
        break;
    case TreeNode::name:
        c = icode.addNewCommand();
        c->type = ICommand::name;
        c->name_ptr = tree->nameDef;
        break;
    case TreeNode::array_idx:
        if (tree->left == 0 || tree->right == 0) {
            yyerror("Illegal expression");
            return;
        }
        c = icode.addNewCommand();
        c->type = ICommand::name_lvalue;
        c->name_ptr = tree->left->nameDef;

        genExpr(tree->right);

        c = icode.addNewCommand();
        c->type = ICommand::array_element;
        break;
    case TreeNode::plus:
        binop = ICommand::plus;
        LBinOp: ;
        if (tree->left == 0 || tree->right == 0) {
            yyerror("Illegal expression");
            return;
        }
        genExpr(tree->left);
        genExpr(tree->right);
        c = icode.addNewCommand();
        c->type = binop;
        break;
    case TreeNode::minus:
        binop = ICommand::minus;
        goto LBinOp;
    case TreeNode::mul:
        binop = ICommand::mul;
        goto LBinOp;
    case TreeNode::div:
        binop = ICommand::div;
        goto LBinOp;
    case TreeNode::mod:
        binop = ICommand::mod;
        goto LBinOp;
    case TreeNode::uminus:
        if (tree->left == 0) {
            yyerror("Illegal expression");
            return;
        }
        genExpr(tree->left);
        c = icode.addNewCommand();
        c->type = ICommand::uminus;
        break;
    default:
        yyerror("Illegal expression");
    }
}

static void genNop(const Label* label) {
    IProgram::iterator c = icode.addNewCommand();
    label->location = c;
    c->type = ICommand::nop;
    c->label_ptr = label;
    c->label = label->number;
}

static void genGoto(const Label* label) {
    IProgram::iterator c = icode.addNewCommand();
    c->type = ICommand::go_to;
    c->goto_label_ptr = label;
    c->goto_label = label->number;
}

static int baseLabel = 0;

static const Label* genLabel() {
    ++baseLabel;
    const Label* lab = label_table.addLabel(baseLabel);
    return lab;
}

static void genLExpr(
    const TreeNode* tree,   // Represents a logical expression
    const Label* trueLabel,
    const Label* falseLabel
) {
    IProgram::iterator c;

    switch (tree->nodeType) {
    case TreeNode::relop:
        if (tree->left == 0 || tree->right == 0) {
            yyerror("Illegal comparing");
            return;
        }
        genExpr(tree->left);
        genExpr(tree->right);
        c = icode.addNewCommand();
        c->type = ICommand::cmp;        // Compare

        c = icode.addNewCommand();
        c->type = ICommand::if_goto;

        if (trueLabel != 0) {
            switch (tree->intValue) {   // relation
                case RELOP_EQ: c->ext = ICommand::eq; break;
                case RELOP_NE: c->ext = ICommand::ne; break;
                case RELOP_LT: c->ext = ICommand::lt; break;
                case RELOP_LE: c->ext = ICommand::le; break;
                case RELOP_GT: c->ext = ICommand::gt; break;
                case RELOP_GE: c->ext = ICommand::ge; break;
            }
            c->goto_label_ptr = trueLabel;
            c->goto_label = trueLabel->number;
        } else {
            assert(falseLabel != 0);
            switch (tree->intValue) {  // relation
                case RELOP_EQ: c->ext = ICommand::ne; break; // Inverse
                case RELOP_NE: c->ext = ICommand::eq; break;
                case RELOP_LT: c->ext = ICommand::ge; break;
                case RELOP_LE: c->ext = ICommand::gt; break;
                case RELOP_GT: c->ext = ICommand::le; break;
                case RELOP_GE: c->ext = ICommand::lt; break;
            }
            c->goto_label_ptr = falseLabel;
            c->goto_label = falseLabel->number;
        }
        break;
    case TreeNode::lor:
        if (tree->left == 0 || tree->right == 0) {
            yyerror("Illegal logical expression");
            return;
        }
        if (trueLabel != 0) {
            genLExpr(tree->left, trueLabel, 0);
            genLExpr(tree->right, trueLabel, 0);
        } else {
            const Label* lab = genLabel();
            genLExpr(tree->left, lab, 0);
            genLExpr(tree->right, 0, falseLabel);
            genNop(lab);
        }
        break;
    case TreeNode::land:
        if (falseLabel != 0) {
            genLExpr(tree->left, 0, falseLabel);
            genLExpr(tree->right, 0, falseLabel);
        } else {
            const Label* lab = genLabel();
            genLExpr(tree->left, 0, lab);
            genLExpr(tree->right, trueLabel, 0);
            genNop(lab);
        }
        break;
    case TreeNode::lnot:
        genLExpr(tree->left, falseLabel, trueLabel);
        break;
    }
}

static void checkNames() {
    NameTable::const_iterator i = name_table.begin();
    while (i != name_table.end()) {
        const NameDef* n = &(*i);
        if ((n->modifiers & NameDef::NAME_FUNCTION) == 0) {
            // n is not a name of function
            if (n->numDefinitions == 0) {
                sprintf(
                    error_text, "Udefined variable %s", 
                    n->name.c_str()
                );
                printError(error_text);
            }
            if (n->numReads == 0) {
                sprintf(
                    error_text, "Unused variable %s", 
                    n->name.c_str()
                );
                printWarning(error_text);
            }
            if (
                n->numWrites == 0 && 
                (n->modifiers & NameDef::NAME_ARRAY) == 0
            ) {
                sprintf(
                    error_text, 
                    "Variable %s was never assigned any value",
                    n->name.c_str()
                );
                printWarning(error_text);
            }
        }
        ++i;
    }
}

static void printWarning(const char *text) {
    fprintf(stderr, "Warning: %s\n", text);
}

static void printError(const char *text) {
    fprintf(stderr, "Error: %s\n", text);
}
