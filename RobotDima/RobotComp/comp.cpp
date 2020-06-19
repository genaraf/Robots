/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "comp.y" /* yacc.c:339  */

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


#line 104 "comp.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "comp.h".  */
#ifndef YY_YY_COMP_H_INCLUDED
# define YY_YY_COMP_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NAME = 258,
    INT_CONST = 259,
    DOUBLE_CONST = 260,
    STRING_CONST = 261,
    WHILE = 262,
    ENDWHILE = 263,
    IF = 264,
    ENDIF = 265,
    ELSE = 266,
    ELSEIF = 267,
    TYPE = 268,
    BREAK = 269,
    CONTINUE = 270,
    SM = 271,
    CM = 272,
    LBR = 273,
    RBR = 274,
    LPAR = 275,
    RPAR = 276,
    ILLEGAL = 277,
    INPUT = 278,
    OUTPUT = 279,
    OUTPUTLN = 280,
    END = 281,
    RC = 282,
    SLEEP = 283,
    RIGHT = 284,
    LEFT = 285,
    BACK = 286,
    FORWARD = 287,
    DIMA = 288,
    STOP = 289,
    SOUND = 290,
    MOUTH = 291,
    ASG = 292,
    RELOP = 293,
    LOR = 294,
    LAND = 295,
    LNOT = 296,
    PLUS = 297,
    MINUS = 298,
    MUL = 299,
    DIV = 300,
    MOD = 301,
    UMINUS = 302
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_COMP_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 203 "comp.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  76
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   405

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  91
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  176

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   302

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    54,    54,    59,    68,    71,    76,    79,    82,    85,
      88,    93,    98,   103,   106,   111,   131,   156,   157,   158,
     159,   166,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   188,   199,   200,   207,   218,
     227,   231,   235,   239,   243,   249,   257,   269,   275,   281,
     287,   290,   294,   294,   306,   306,   320,   329,   330,   333,
     334,   337,   340,   346,   353,   361,   368,   375,   381,   386,
     396,   413,   416,   421,   424,   429,   437,   442,   447,   452,
     457,   462,   470,   478,   483,   491,   499,   507,   516,   526,
     530,   537
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "INT_CONST", "DOUBLE_CONST",
  "STRING_CONST", "WHILE", "ENDWHILE", "IF", "ENDIF", "ELSE", "ELSEIF",
  "TYPE", "BREAK", "CONTINUE", "SM", "CM", "LBR", "RBR", "LPAR", "RPAR",
  "ILLEGAL", "INPUT", "OUTPUT", "OUTPUTLN", "END", "RC", "SLEEP", "RIGHT",
  "LEFT", "BACK", "FORWARD", "DIMA", "STOP", "SOUND", "MOUTH", "ASG",
  "RELOP", "LOR", "LAND", "LNOT", "PLUS", "MINUS", "MUL", "DIV", "MOD",
  "UMINUS", "$accept", "program", "stms", "stm", "decl", "type", "defs",
  "def", "oper", "assign", "lval", "lval_name", "expr", "if", "$@1", "$@2",
  "ifh", "elseifs", "elseiflist", "else", "elseif", "elseifh", "lexpr",
  "while", "whileh", "input", "inputlst", "output", "rc", "back",
  "forward", "right", "left", "dima", "sleep", "stop", "sound", "mouth",
  "outputln", "outputlst", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302
};
# endif

#define YYPACT_NINF -46

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-46)))

#define YYTABLE_NINF -63

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     307,    39,   -46,   -16,   -12,   -46,    -2,     8,    45,    96,
       7,    31,    37,    49,    57,    68,   101,   105,   107,   104,
     111,   125,   127,   -46,   -46,   162,   -46,   -46,   130,   150,
     -46,   307,   -46,   307,   -46,   -46,   -46,   -46,   -46,   -46,
     -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,
      13,    13,   -46,   -46,   -46,    18,   151,   -46,   -46,   -46,
      96,    96,   137,    27,   -46,    44,    96,    25,   -46,   -46,
     -46,   -46,   -46,   -46,    66,    92,   -46,   -46,   -46,   153,
      60,   -46,    96,    96,   163,   235,    13,    13,   209,   -14,
      43,   -46,    45,    96,   323,   -46,    96,    96,    96,    96,
      96,   -46,    96,   -46,    62,   168,   340,   169,   349,   173,
     354,    96,   -46,   162,   102,   304,   -46,   190,   191,   -46,
      -6,    93,   -46,    96,   -46,    13,    13,   -46,   -46,   309,
     -46,   -24,   -24,   -46,   -46,   -46,   137,    96,   -46,   188,
     -46,   189,   -46,   200,   314,   -46,   -46,   -46,   307,   197,
     195,   207,    99,   -46,   307,   -46,   137,   178,   -46,   -46,
     359,   -46,   -46,   -46,   -46,   271,   -46,    13,   -46,   -46,
     -46,   199,   203,    98,   -46,   -46
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    38,     0,     0,    12,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     6,     0,     7,    17,     0,    36,
      18,     0,    19,     0,    22,    23,    25,    27,    26,    28,
      29,    31,    30,    34,    32,    33,    24,    10,     9,     8,
       0,     0,    20,    21,    72,     0,    45,    47,    48,    49,
       0,     0,    90,     0,    88,     0,     0,     0,    78,    79,
      76,    77,    80,    83,     0,     0,     1,     3,     5,    15,
       0,    13,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    71,     0,     0,     0,    44,     0,     0,     0,     0,
       0,    74,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,    11,     0,     0,     0,    51,     0,     0,    69,
       0,     0,    67,     0,    70,     0,     0,    56,    73,     0,
      50,    39,    40,    41,    42,    43,    91,     0,    82,     0,
      85,     0,    87,     0,     0,    14,    35,    37,     0,     0,
       0,     0,    57,    59,     0,    68,    64,    65,    66,    46,
       0,    81,    84,    86,    16,     0,    53,     0,    55,    58,
      60,     0,     0,     0,    75,    63
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -46,   -46,   -28,   -22,   -46,   -46,   -46,   108,   -46,   -46,
       1,   -46,    -8,   -46,   -46,   -46,   -46,   -46,   -46,    73,
      85,   -46,   -45,   -46,   -46,   -46,   -46,   -46,   -46,   -46,
     -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,   210
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    21,    22,    23,    24,    25,    80,    81,    26,    27,
      28,    29,    88,    30,   117,   118,    31,   151,   152,   149,
     153,   154,    89,    32,    33,    34,    55,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    63
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      78,    62,    62,    84,    50,    85,    90,   124,    51,    54,
      56,    57,    58,    59,    52,   130,    56,    57,    58,    59,
      98,    99,   100,    64,    53,   125,   126,    60,    56,    57,
      58,    59,   123,    86,    91,    92,    96,    97,    98,    99,
     100,   121,   122,   101,   102,    60,   105,    47,     2,    48,
      61,    66,    94,    95,    87,    49,    61,    67,   104,   106,
     103,   102,    78,    78,   127,    68,   108,   110,    61,    56,
      57,    58,    59,    69,   114,   115,   112,   113,   120,   137,
     157,   158,   125,   126,    70,   129,    60,   107,   131,   132,
     133,   134,   135,   128,   136,    56,    57,    58,    59,    56,
      57,    58,    59,   144,    96,    97,    98,    99,   100,    61,
     148,   150,    60,   109,   155,   156,    60,    71,   146,   175,
     165,    72,   173,    73,    74,    76,   171,    -2,     1,   160,
       2,    75,   125,   126,     3,    61,     4,   125,   126,    61,
       5,     6,     7,    78,    96,    97,    98,    99,   100,    78,
       8,     9,    10,    77,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     1,    79,     2,    82,    83,    93,
       3,   111,     4,   116,   -52,   -54,     5,     6,     7,    96,
      97,    98,    99,   100,   138,   140,     8,     9,    10,   142,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
       1,   148,     2,   150,   161,   162,     3,   166,     4,   -62,
     -62,   -62,     5,     6,     7,   167,   163,   168,   126,   174,
      65,   145,     8,     9,    10,   169,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,     1,   170,     2,     0,
       0,     0,     3,   119,     4,     0,     0,   123,     5,     6,
       7,    96,    97,    98,    99,   100,     0,     0,     8,     9,
      10,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,     1,     0,     2,     0,     0,     0,     3,     0,
       4,   -61,     0,     0,     5,     6,     7,     0,     0,     0,
       0,     0,     0,     0,     8,     9,    10,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,     1,     0,
       2,     0,     0,     0,     3,     0,     4,     0,     0,     0,
       5,     6,     7,   147,     0,     0,     0,     0,   159,     0,
       8,     9,    10,   164,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,   130,     0,    96,    97,    98,    99,
     100,    96,    97,    98,    99,   100,    96,    97,    98,    99,
     100,   139,     0,     0,     0,    96,    97,    98,    99,   100,
     141,     0,     0,     0,     0,   143,     0,     0,     0,     0,
     172,     0,    96,    97,    98,    99,   100,     0,     0,     0,
       0,    96,    97,    98,    99,   100,    96,    97,    98,    99,
     100,    96,    97,    98,    99,   100
};

static const yytype_int16 yycheck[] =
{
      22,     9,    10,    31,    20,    33,    51,    21,    20,     8,
       3,     4,     5,     6,    16,    21,     3,     4,     5,     6,
      44,    45,    46,    16,    16,    39,    40,    20,     3,     4,
       5,     6,    38,    20,    16,    17,    42,    43,    44,    45,
      46,    86,    87,    16,    17,    20,    21,     8,     3,    10,
      43,    20,    60,    61,    41,    16,    43,    20,    66,    67,
      16,    17,    84,    85,    21,    16,    74,    75,    43,     3,
       4,     5,     6,    16,    82,    83,    16,    17,    86,    17,
     125,   126,    39,    40,    16,    93,    20,    21,    96,    97,
      98,    99,   100,    92,   102,     3,     4,     5,     6,     3,
       4,     5,     6,   111,    42,    43,    44,    45,    46,    43,
      11,    12,    20,    21,    21,   123,    20,    16,    16,    21,
     148,    16,   167,    16,    20,     0,   154,     0,     1,   137,
       3,    20,    39,    40,     7,    43,     9,    39,    40,    43,
      13,    14,    15,   165,    42,    43,    44,    45,    46,   171,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,     1,     3,     3,    37,    18,    18,
       7,    18,     9,    10,    11,    12,    13,    14,    15,    42,
      43,    44,    45,    46,    16,    16,    23,    24,    25,    16,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
       1,    11,     3,    12,    16,    16,     7,    10,     9,    10,
      11,    12,    13,    14,    15,    20,    16,    10,    40,    16,
      10,   113,    23,    24,    25,   152,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,     1,   152,     3,    -1,
      -1,    -1,     7,     8,     9,    -1,    -1,    38,    13,    14,
      15,    42,    43,    44,    45,    46,    -1,    -1,    23,    24,
      25,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,     1,    -1,     3,    -1,    -1,    -1,     7,    -1,
       9,    10,    -1,    -1,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    24,    25,    -1,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,     1,    -1,
       3,    -1,    -1,    -1,     7,    -1,     9,    -1,    -1,    -1,
      13,    14,    15,    19,    -1,    -1,    -1,    -1,    19,    -1,
      23,    24,    25,    19,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    21,    -1,    42,    43,    44,    45,
      46,    42,    43,    44,    45,    46,    42,    43,    44,    45,
      46,    21,    -1,    -1,    -1,    42,    43,    44,    45,    46,
      21,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      21,    -1,    42,    43,    44,    45,    46,    -1,    -1,    -1,
      -1,    42,    43,    44,    45,    46,    42,    43,    44,    45,
      46,    42,    43,    44,    45,    46
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     7,     9,    13,    14,    15,    23,    24,
      25,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    49,    50,    51,    52,    53,    56,    57,    58,    59,
      61,    64,    71,    72,    73,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,     8,    10,    16,
      20,    20,    16,    16,    58,    74,     3,     4,     5,     6,
      20,    43,    60,    87,    16,    87,    20,    20,    16,    16,
      16,    16,    16,    16,    20,    20,     0,    26,    51,     3,
      54,    55,    37,    18,    50,    50,    20,    41,    60,    70,
      70,    16,    17,    18,    60,    60,    42,    43,    44,    45,
      46,    16,    17,    16,    60,    21,    60,    21,    60,    21,
      60,    18,    16,    17,    60,    60,    10,    62,    63,     8,
      60,    70,    70,    38,    21,    39,    40,    21,    58,    60,
      21,    60,    60,    60,    60,    60,    60,    17,    16,    21,
      16,    21,    16,    21,    60,    55,    16,    19,    11,    67,
      12,    65,    66,    68,    69,    21,    60,    70,    70,    19,
      60,    16,    16,    16,    19,    50,    10,    20,    10,    67,
      68,    50,    21,    70,    16,    21
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    49,    50,    50,    51,    51,    51,    51,
      51,    52,    53,    54,    54,    55,    55,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    57,    58,    58,    59,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    61,    62,    61,    63,    61,    64,    65,    65,    66,
      66,    67,    68,    69,    70,    70,    70,    70,    70,    71,
      72,    73,    74,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    82,    83,    84,    84,    85,    85,    86,    86,
      87,    87
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     1,     1,     2,     2,
       2,     3,     1,     1,     3,     1,     4,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     1,     4,     1,     3,
       3,     3,     3,     3,     2,     1,     4,     1,     1,     1,
       3,     3,     0,     5,     0,     5,     4,     1,     2,     1,
       2,     2,     2,     4,     3,     3,     3,     2,     3,     3,
       4,     3,     1,     3,     3,     7,     2,     2,     2,     2,
       2,     5,     4,     2,     5,     4,     5,     4,     2,     3,
       1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 54 "comp.y" /* yacc.c:1646  */
    {
              checkNames();
              icode.print();
              printf("--------\n\n");
          }
#line 1455 "comp.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 59 "comp.y" /* yacc.c:1646  */
    {
              checkNames();
              icode.print();
              printf("--------\n\n");
              // YYACCEPT;
              return 0;
          }
#line 1467 "comp.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 68 "comp.y" /* yacc.c:1646  */
    {
              // printf("stms: stm\n"); 
          }
#line 1475 "comp.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 71 "comp.y" /* yacc.c:1646  */
    { 
              // printf("stms: stms stm\n");
          }
#line 1483 "comp.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 76 "comp.y" /* yacc.c:1646  */
    {
              // printf("stm: decl\n");
          }
#line 1491 "comp.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 79 "comp.y" /* yacc.c:1646  */
    {
              // printf("stm: oper\n"); 
          }
#line 1499 "comp.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 82 "comp.y" /* yacc.c:1646  */
    {
              // printf("stm: error SM\n"); 
          }
#line 1507 "comp.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 85 "comp.y" /* yacc.c:1646  */
    {
              // printf("stm: error ENDIF\n"); 
          }
#line 1515 "comp.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 88 "comp.y" /* yacc.c:1646  */
    { 
              // printf("stm: error WHILE\n"); 
          }
#line 1523 "comp.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 93 "comp.y" /* yacc.c:1646  */
    {
              // printf("decl\n"); 
          }
#line 1531 "comp.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 98 "comp.y" /* yacc.c:1646  */
    {
              base_type = (yyvsp[0]).int_value;
          }
#line 1539 "comp.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 103 "comp.y" /* yacc.c:1646  */
    {
              // printf("def\n"); 
          }
#line 1547 "comp.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 106 "comp.y" /* yacc.c:1646  */
    {
              // printf("defs CM def\n"); 
          }
#line 1555 "comp.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 111 "comp.y" /* yacc.c:1646  */
    {
              NameDef* n = name_table.addName((yyvsp[0]).name);
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
#line 1580 "comp.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 131 "comp.y" /* yacc.c:1646  */
    {
              // $3.tree->print();
              genExpr((yyvsp[-1]).tree);

              NameDef* n = name_table.addName((yyvsp[-3]).name);
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
#line 1608 "comp.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 159 "comp.y" /* yacc.c:1646  */
    {
              if (breakLabel == 0) {
                  yyerror("break out of loop");
              } else {
                  genGoto(breakLabel);
              }
          }
#line 1620 "comp.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 166 "comp.y" /* yacc.c:1646  */
    {
              if (continueLabel == 0) {
                  yyerror("continue out of loop");
              } else {
                  genGoto(continueLabel);
              }
          }
#line 1632 "comp.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 188 "comp.y" /* yacc.c:1646  */
    {
              // printf("assign: right expr:\n");
              // $3.tree->print();

              genExpr((yyvsp[-1]).tree);

              IProgram::iterator c = icode.addNewCommand();
              c->type = ICommand::assign;
          }
#line 1646 "comp.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 200 "comp.y" /* yacc.c:1646  */
    {
              genExpr((yyvsp[-1]).tree);
              IProgram::iterator c = icode.addNewCommand();
              c->type = ICommand::array_lvalue;
          }
#line 1656 "comp.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 207 "comp.y" /* yacc.c:1646  */
    {
                NameDef* n = name_table.addName((yyvsp[0]).name);
                // printf("l-value %s\n", n->name);
                ++(n->numWrites);

                IProgram::iterator c = icode.addNewCommand();
                c->type = ICommand::name_lvalue;
                c->name_ptr = n;
            }
#line 1670 "comp.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 218 "comp.y" /* yacc.c:1646  */
    {
              binOper = TreeNode::plus;
              LBinOper: ;
              TreeNode* t = new TreeNode();
              t->nodeType = binOper;
              t->addLeftSon((yyvsp[-2]).tree);
              t->addRightSon((yyvsp[0]).tree);
              (yyval).tree = t;
          }
#line 1684 "comp.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 227 "comp.y" /* yacc.c:1646  */
    {
              binOper = TreeNode::minus;
              goto LBinOper;
          }
#line 1693 "comp.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 231 "comp.y" /* yacc.c:1646  */
    {
              binOper = TreeNode::mul;
              goto LBinOper;
          }
#line 1702 "comp.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 235 "comp.y" /* yacc.c:1646  */
    {
              binOper = TreeNode::div;
              goto LBinOper;
          }
#line 1711 "comp.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 239 "comp.y" /* yacc.c:1646  */
    {
              binOper = TreeNode::mod;
              goto LBinOper;
          }
#line 1720 "comp.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 243 "comp.y" /* yacc.c:1646  */
    {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::uminus;
              t->addLeftSon((yyvsp[0]).tree);
              (yyval).tree = t;
          }
#line 1731 "comp.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 249 "comp.y" /* yacc.c:1646  */
    {
              TreeNode* t = new TreeNode();
              NameDef* n = name_table.addName((yyvsp[0]).name);
              ++(n->numReads);
              t->nodeType = TreeNode::name;
              t->nameDef = n;
              (yyval).tree = t;
          }
#line 1744 "comp.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 257 "comp.y" /* yacc.c:1646  */
    {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::array_idx;
              NameDef* n = name_table.addName((yyvsp[-3]).name);
              ++(n->numReads);
              TreeNode* t1 = new TreeNode();
              t1->nodeType = TreeNode::name;
              t1->nameDef = n;
              t->addLeftSon(t1);
              t->addRightSon((yyvsp[-1]).tree);
              (yyval).tree = t;
          }
#line 1761 "comp.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 269 "comp.y" /* yacc.c:1646  */
    {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::int_const;
              t->intValue = (yyvsp[0]).int_value;
              (yyval).tree = t;
          }
#line 1772 "comp.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 275 "comp.y" /* yacc.c:1646  */
    {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::double_const;
              t->doubleValue = (yyvsp[0]).double_value;
              (yyval).tree = t;
          }
#line 1783 "comp.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 281 "comp.y" /* yacc.c:1646  */
    {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::string_const;
              t->stringValue = (yyvsp[0]).name;
              (yyval).tree = t;
          }
#line 1794 "comp.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 287 "comp.y" /* yacc.c:1646  */
    { (yyval).tree = (yyvsp[-1]).tree; }
#line 1800 "comp.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 290 "comp.y" /* yacc.c:1646  */
    { 
              // printf("if: ifh stmd ENDIF\n");
              genNop((yyvsp[-2]).label1);        // Label of endif
          }
#line 1809 "comp.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 294 "comp.y" /* yacc.c:1646  */
    {
              const Label* endif_label = genLabel();
              (yyvsp[-1]).label2 = endif_label;
              (yyvsp[-1]).saved_label2 = endifLabel;
              endifLabel = endif_label;
              genGoto(endif_label);
              genNop((yyvsp[-1]).label1);
          }
#line 1822 "comp.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 302 "comp.y" /* yacc.c:1646  */
    {
              genNop((yyvsp[-4]).label2);
              endifLabel = (yyvsp[-4]).saved_label2;
          }
#line 1831 "comp.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 306 "comp.y" /* yacc.c:1646  */
    {
              const Label* endif_label = genLabel();
              (yyvsp[-1]).label2 = endif_label;
              (yyvsp[-1]).saved_label2 = endifLabel;  // Save previous endif label
              endifLabel = endif_label;
              genGoto(endif_label);
              genNop((yyvsp[-1]).label1);
          }
#line 1844 "comp.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 314 "comp.y" /* yacc.c:1646  */
    {
              genNop((yyvsp[-4]).label2);
              endifLabel = (yyvsp[-4]).saved_label2;
          }
#line 1853 "comp.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 320 "comp.y" /* yacc.c:1646  */
    { 
              // printf("ifh\n");
              const Label* l = genLabel();
              // $3.tree->print();
              genLExpr((yyvsp[-1]).tree, 0, l);
              (yyval).label1 = l;
          }
#line 1865 "comp.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 340 "comp.y" /* yacc.c:1646  */
    {
              genGoto(endifLabel);
              genNop((yyvsp[-1]).label1);
          }
#line 1874 "comp.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 346 "comp.y" /* yacc.c:1646  */
    {
              const Label* l = genLabel();
              genLExpr((yyvsp[-1]).tree, 0, l);
              (yyval).label1 = l;
          }
#line 1884 "comp.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 353 "comp.y" /* yacc.c:1646  */
    {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::relop;
              t->intValue = (yyvsp[-1]).int_value; // operation: RELOP_EQ, etc.
              t->addLeftSon((yyvsp[-2]).tree);
              t->addRightSon((yyvsp[0]).tree);
              (yyval).tree = t;
          }
#line 1897 "comp.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 361 "comp.y" /* yacc.c:1646  */
    {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::lor;
              t->addLeftSon((yyvsp[-2]).tree);
              t->addRightSon((yyvsp[0]).tree);
              (yyval).tree = t;
          }
#line 1909 "comp.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 368 "comp.y" /* yacc.c:1646  */
    {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::land;
              t->addLeftSon((yyvsp[-2]).tree);
              t->addRightSon((yyvsp[0]).tree);
              (yyval).tree = t;
          }
#line 1921 "comp.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 375 "comp.y" /* yacc.c:1646  */
    {
              TreeNode* t = new TreeNode();
              t->nodeType = TreeNode::lnot;
              t->addLeftSon((yyvsp[0]).tree);
              (yyval).tree = t;
          }
#line 1932 "comp.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 381 "comp.y" /* yacc.c:1646  */
    {
              (yyval).tree = (yyvsp[-1]).tree;
          }
#line 1940 "comp.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 386 "comp.y" /* yacc.c:1646  */
    { 
              // printf("while\n");
              // Restore loop labels
              genGoto((yyvsp[-2]).label1);
              genNop((yyvsp[-2]).label2);
              continueLabel = (yyvsp[-2]).saved_label1;
              breakLabel = (yyvsp[-2]).saved_label2;
          }
#line 1953 "comp.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 396 "comp.y" /* yacc.c:1646  */
    { 
              // printf("whileh\n");
              const Label* beginLab = genLabel();
              const Label* endLab = genLabel();
              (yyval).label1 = beginLab;
              (yyval).label2 = endLab;

              genNop(beginLab);
              genLExpr((yyvsp[-1]).tree, 0, endLab);

              (yyval).saved_label1 = continueLabel;
              (yyval).saved_label2 = breakLabel;
              continueLabel = beginLab;
              breakLabel = endLab;
          }
#line 1973 "comp.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 416 "comp.y" /* yacc.c:1646  */
    {
              LInput: ;
              IProgram::iterator c = icode.addNewCommand();
              c->type = ICommand::input;
          }
#line 1983 "comp.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 421 "comp.y" /* yacc.c:1646  */
    { goto LInput; }
#line 1989 "comp.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 429 "comp.y" /* yacc.c:1646  */
    {
            genExpr((yyvsp[-4]).tree);
            genExpr((yyvsp[-2]).tree);
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::rc;
        }
#line 2000 "comp.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 437 "comp.y" /* yacc.c:1646  */
    {
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::back;
        }
#line 2009 "comp.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 442 "comp.y" /* yacc.c:1646  */
    {
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::forward;
        }
#line 2018 "comp.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 447 "comp.y" /* yacc.c:1646  */
    {
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::right;
        }
#line 2027 "comp.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 452 "comp.y" /* yacc.c:1646  */
    {
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::left;
        }
#line 2036 "comp.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 457 "comp.y" /* yacc.c:1646  */
    {
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::dima;
        }
#line 2045 "comp.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 462 "comp.y" /* yacc.c:1646  */
    {
            genExpr((yyvsp[-2]).tree);
            IProgram::iterator c = icode.addNewCommand();
           // c->type = ICommand::int_const;
            //c->int_value = $3.int_value;
            //c = icode.addNewCommand();
            c->type = ICommand::sleep;
        }
#line 2058 "comp.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 470 "comp.y" /* yacc.c:1646  */
    {
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::int_const;
            c->int_value = 1000;
            c = icode.addNewCommand();
            c->type = ICommand::sleep;
        }
#line 2070 "comp.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 478 "comp.y" /* yacc.c:1646  */
    {
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::stop;
        }
#line 2079 "comp.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 483 "comp.y" /* yacc.c:1646  */
    {
            genExpr((yyvsp[-2]).tree);
            IProgram::iterator c = icode.addNewCommand();
           // c->type = ICommand::int_const;
            //c->int_value = $3.int_value;
            //c = icode.addNewCommand();
            c->type = ICommand::sound;
        }
#line 2092 "comp.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 491 "comp.y" /* yacc.c:1646  */
    {
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::int_const;
            c->int_value = 1;
            c = icode.addNewCommand();
            c->type = ICommand::sound;
        }
#line 2104 "comp.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 499 "comp.y" /* yacc.c:1646  */
    {
            genExpr((yyvsp[-2]).tree);
            IProgram::iterator c = icode.addNewCommand();
           // c->type = ICommand::int_const;
            //c->int_value = $3.int_value;
            //c = icode.addNewCommand();
            c->type = ICommand::mouth;
        }
#line 2117 "comp.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 507 "comp.y" /* yacc.c:1646  */
    {
            IProgram::iterator c = icode.addNewCommand();
            c->type = ICommand::int_const;
            c->int_value = 1;
            c = icode.addNewCommand();
            c->type = ICommand::mouth;
        }
#line 2129 "comp.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 516 "comp.y" /* yacc.c:1646  */
    {
              LNewLine: ;   
              // Output the "new line" character
              IProgram::iterator c = icode.addNewCommand();
              c->type = ICommand::string_const;
              c->string_value = "\n";
                
              c = icode.addNewCommand();
              c->type = ICommand::output;
          }
#line 2144 "comp.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 526 "comp.y" /* yacc.c:1646  */
    { goto LNewLine; }
#line 2150 "comp.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 530 "comp.y" /* yacc.c:1646  */
    {
              LOutput: ;
              genExpr((yyvsp[0]).tree);

              IProgram::iterator c = icode.addNewCommand();
              c->type = ICommand::output;
          }
#line 2162 "comp.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 537 "comp.y" /* yacc.c:1646  */
    { goto LOutput; }
#line 2168 "comp.cpp" /* yacc.c:1646  */
    break;


#line 2172 "comp.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 541 "comp.y" /* yacc.c:1906  */


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
