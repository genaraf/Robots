%{
#include "parser.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const int MAX_STR_LEN = 1023;
static char curstr[MAX_STR_LEN+1];
static int curstr_len = 0;
static bool longString = false;
void scanError(const char *s, int lineno);

%}
        /* Macro defining the exponent in double constant */
EXP     ((E|e)("+"|"-")?[0-9]+)

%option yylineno
%Start S CCOMM CPPCOMM STR
%%
            {
                BEGIN S;
            }
<S>"if"     {
                // printf ("scan: IF\n");
                return IF; 
            }
<S>"else"   { return ELSE; }
<S>"elseif" { return ELSEIF; }
<S>"endif"  { return ENDIF; }
<S>"while"  { return WHILE; }
<S>"break"  { return BREAK; }
<S>"continue" { return CONTINUE; }
<S>"endwhile" { return ENDWHILE; }
<S>"input"  { return INPUT; }
<S>"output" { return OUTPUT; }
<S>"outputln" { return OUTPUTLN; }
<S>"RC" { return RC; }
<S>"sleep" { return SLEEP; }
<S>"goLeft()" { return LEFT; }
<S>"goRight()" { return RIGHT; }
<S>"goBack()" { return BACK; }
<S>"goForward()" { return FORWARD; }
<S>"stop()" { return STOP; }
<S>"sound" { return SOUND; }
<S>"mouth" { return MOUTH; }
<S>"dothedimadance()" { return DIMA; }

<S>"end"    { return END; }
<S>"int"    {
                // printf ("scan: TYPE (int)\n");
                yylval.int_value = TYPE_INT;
                return TYPE;
            }
<S>"double" {
                // printf ("scan: TYPE (double)\n");
                yylval.int_value = TYPE_DOUBLE;
                return TYPE;
            }
<S>"string" {
                // printf ("scan: TYPE (string)\n");
                yylval.int_value = TYPE_STRING;
                return TYPE;
            }
<S>"+"      { return PLUS; }
<S>"-"      { return MINUS; }
<S>"*"      { return MUL; }
<S>"/"      { return DIV; }
<S>"%"      { return MOD; }
<S>"("      {
                // printf ("scan: LPAR\n");
                return LPAR; 
            }
<S>")"      {
                // printf ("scan: RPAR\n");
                return RPAR;
            }
<S>"["      { return LBR; }
<S>"]"      { return RBR; }
<S>","      { return CM; }
<S>";"      { return SM; }
<S>"="      { return ASG; }
<S>"=="     {
                // printf ("scan: RELOP_EQ\n");
                yylval.int_value = RELOP_EQ;
                return RELOP;
            }
<S>"!="     {
                // printf ("scan: RELOP_NE\n");
                yylval.int_value = RELOP_NE;
                return RELOP;
            }
<S>">"      {
                // printf ("scan: RELOP_GT\n");
                yylval.int_value = RELOP_GT;
                return RELOP;
            }
<S>">="     {
                // printf ("scan: RELOP_GE\n");
                yylval.int_value = RELOP_GE;
                return RELOP;
            }
<S>"<"      {
                // printf ("scan: RELOP_LT\n");
                yylval.int_value = RELOP_LT;
                return RELOP;
            }
<S>"<="     {
                // printf ("scan: RELOP_LE\n");
                yylval.int_value = RELOP_LE;
                return RELOP;
            }
<S>"||"     { return LOR; }
<S>"&&"     { return LAND; }
<S>"!"      { return LNOT; }

<S>[a-zA-Z][a-zA-Z0-9]* {
                // printf("NAME %s\n", yytext);

                yylval.name = yytext;

                // printf("NAME %s is added to string pool\n", yytext);
                return NAME;
            }

<S>(" "|\t)*   {}                  /* Skip space */

            /* Integer constant */
<S>[0-9]+   {
                yylval.int_value = atoi(yytext);
                return INT_CONST;
            }

            /* Double constant may be in one of the following 3 forms:        */
            /* 1) .5, .5e-20, 1.2, 123.456E+10 (mandatory point and fraction) */
            /* 2) 1., 123.e+10, 0.2E-20, 35.3  (mandatory integer and point)  */
            /* 3) 1e+10, 2e20, 1E-12        (mandatory integer and exponent)  */
            /* The expression for double is the union "|" of 3 subexpressions */

<S>([0-9]*"."[0-9]+{EXP}?)|([0-9]+"."[0-9]*{EXP}?)|([0-9]+{EXP}) {
                yylval.double_value = atof(yytext);
                return DOUBLE_CONST;
            }

<S>\n|\r\n  { /* return ENDL; */ } /* End of line in Unix or MS DOS form */

<S>\"       {
                BEGIN STR;
                curstr_len = 0; longString = false;
            }

<STR>[^"\n\r\t] {
                if (curstr_len < MAX_STR_LEN) {
                    curstr[curstr_len] = yytext[0];
                    ++curstr_len;
                } else {
                    LLongString: ;
                    if (!longString) {
                        longString = true;
                        scanError(
                            "String constant is too long",
                            yylineno
                        );
                    }
                }
            }
<STR>\\\"   {
                if (curstr_len < MAX_STR_LEN) {
                    curstr[curstr_len] = '\"';
                    ++curstr_len;
                } else {
                    goto LLongString;
                }
            }
<STR>\\n    {
                if (curstr_len < MAX_STR_LEN) {
                    curstr[curstr_len] = '\n';
                    ++curstr_len;
                } else {
                    goto LLongString;
                }
            }
<STR>\\r    {
                if (curstr_len < MAX_STR_LEN) {
                    curstr[curstr_len] = '\r';
                    ++curstr_len;
                } else {
                    goto LLongString;
                }
            }
<STR>\\t    {
                if (curstr_len < MAX_STR_LEN) {
                    curstr[curstr_len] = '\t';
                    ++curstr_len;
                } else {
                    goto LLongString;
                }
            }
<STR>\"     {
                LOffString: ;
                curstr[curstr_len] = 0;
                yylval.name = curstr;
                // printf("scan: STRING_CONST: %s\n", yylval.name.c_str());
                BEGIN S;
                return STRING_CONST;
            }
<STR>\n|\r\n {
                scanError(
                    "Nonterminated string constant", 
                    yylineno-1
                );
                goto LOffString;
            }
<S>"/*"     { BEGIN CCOMM; }
<CCOMM>(.|\n) {}
<CCOMM>"*/" { BEGIN S; }
<S>"//"     { BEGIN CPPCOMM; }
<CPPCOMM>.  {}
<CPPCOMM>\n { BEGIN S; }
<S>.        { return ILLEGAL; } /* Any other character */

%%
            /*============= The user programs section ===================*/

int yywrap() { return 1; }      /* Stop at end of file */

void scanError(const char *s, int line) {
    fprintf(
        stderr, "Scanner error: %s in line %d\n", 
        s, line
    );
}
