/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CONSTANT = 258,
     IDENTIFIER = 259,
     TEMPLATE = 260,
     STRING_LITERAL = 261,
     URGENT = 262,
     BROADCAST = 263,
     META = 264,
     PTR_OP = 265,
     INC_OP = 266,
     DEC_OP = 267,
     LEFT_OP = 268,
     RIGHT_OP = 269,
     LE_OP = 270,
     GE_OP = 271,
     EQ_OP = 272,
     NE_OP = 273,
     AND_OP = 274,
     OR_OP = 275,
     MUL_ASSIGN = 276,
     DIV_ASSIGN = 277,
     MOD_ASSIGN = 278,
     ADD_ASSIGN = 279,
     SUB_ASSIGN = 280,
     LEFT_ASSIGN = 281,
     RIGHT_ASSIGN = 282,
     AND_ASSIGN = 283,
     XOR_ASSIGN = 284,
     OR_ASSIGN = 285,
     TYPE_NAME = 286,
     TYPEDEF = 287,
     INT = 288,
     CONST = 289,
     VOID = 290,
     CLOCK = 291,
     CHAN = 292,
     BOOL = 293,
     TRUE_Y = 294,
     FALSE_Y = 295,
     STRUCT = 296,
     ELLIPSIS = 297,
     COMMENT = 298,
     CASE = 299,
     DEFAULT = 300,
     IF = 301,
     ELSE = 302,
     SWITCH = 303,
     WHILE = 304,
     DO = 305,
     FOR = 306,
     CONTINUE = 307,
     BREAK = 308,
     RETURN = 309,
     SYSTEM = 310,
     IFX = 311
   };
#endif
/* Tokens.  */
#define CONSTANT 258
#define IDENTIFIER 259
#define TEMPLATE 260
#define STRING_LITERAL 261
#define URGENT 262
#define BROADCAST 263
#define META 264
#define PTR_OP 265
#define INC_OP 266
#define DEC_OP 267
#define LEFT_OP 268
#define RIGHT_OP 269
#define LE_OP 270
#define GE_OP 271
#define EQ_OP 272
#define NE_OP 273
#define AND_OP 274
#define OR_OP 275
#define MUL_ASSIGN 276
#define DIV_ASSIGN 277
#define MOD_ASSIGN 278
#define ADD_ASSIGN 279
#define SUB_ASSIGN 280
#define LEFT_ASSIGN 281
#define RIGHT_ASSIGN 282
#define AND_ASSIGN 283
#define XOR_ASSIGN 284
#define OR_ASSIGN 285
#define TYPE_NAME 286
#define TYPEDEF 287
#define INT 288
#define CONST 289
#define VOID 290
#define CLOCK 291
#define CHAN 292
#define BOOL 293
#define TRUE_Y 294
#define FALSE_Y 295
#define STRUCT 296
#define ELLIPSIS 297
#define COMMENT 298
#define CASE 299
#define DEFAULT 300
#define IF 301
#define ELSE 302
#define SWITCH 303
#define WHILE 304
#define DO 305
#define FOR 306
#define CONTINUE 307
#define BREAK 308
#define RETURN 309
#define SYSTEM 310
#define IFX 311




/* Copy the first part of user declarations.  */
#line 1 "src/uppaalparser.y"

  #include<iostream>
  #include<string>
  #include <stdio.h>
  #include<map>
  #include<vector>
  #include <utility>
  
  #include "channel.h"

  #include "util/dbmutil.hpp"
  #include "io/uppaaldata.h"
  #include "io/uppaalmodelparser.h"
  #include "model/ta.hpp"
  
  using std::string;
  using std::map;
  using std::vector;
  using std::make_pair;
      
  using namespace graphsat;
  
  int yydebug=1;   
  using std::cerr;
  using std::endl;
  using std::cout;

  vector<string> symbol_table;
  
  extern  int lineNum;


  UppaalTemplateData* system_data;
  UppaalTemplateData* current_data;
  UppaalParser* model_parser;
  
  extern void yyerror(const char *); 
  extern void yyerror(const string&);
  extern int yylex();
  extern int yyparse();
  extern FILE* yyin;


  
 

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 47 "src/uppaalparser.y"
{
  int int_value;
  TYPE_T type_value;
  vector<string> * str_vec_pointer;
  COMP_OPERATOR com_op;
  
 }
/* Line 193 of yacc.c.  */
#line 262 "src/uppaalparser.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 275 "src/uppaalparser.cpp"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  42
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   116

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  49
/* YYNRULES -- Number of states.  */
#define YYNSTATES  91

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   311

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    61,     2,     2,     2,     2,     2,     2,
      63,    64,     2,     2,    57,    60,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    65,
      58,    62,    59,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    66,     2,    67,     2,     2,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    14,    17,    19,
      23,    25,    27,    31,    35,    37,    39,    41,    43,    45,
      47,    49,    52,    54,    56,    58,    60,    62,    64,    68,
      70,    73,    77,    81,    87,    89,    92,    94,    98,   102,
     106,   113,   115,   117,   119,   123,   130,   136,   143,   153
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      73,     0,    -1,    33,    -1,    36,    -1,    38,    -1,    37,
      -1,     7,    37,    -1,     8,    37,    -1,     4,    -1,    70,
      57,     4,    -1,     4,    -1,     5,    -1,    71,    57,     4,
      -1,    71,    57,     5,    -1,    17,    -1,    15,    -1,    16,
      -1,    58,    -1,    59,    -1,    18,    -1,    74,    -1,    73,
      74,    -1,    76,    -1,    81,    -1,    75,    -1,    78,    -1,
      82,    -1,    77,    -1,    75,    19,    77,    -1,    43,    -1,
      76,    43,    -1,     4,    72,    80,    -1,     4,    72,     4,
      -1,     4,    60,     4,    72,    80,    -1,     4,    -1,    61,
       4,    -1,    79,    -1,    78,    57,    79,    -1,     4,    62,
      80,    -1,     4,    62,     4,    -1,     4,    62,     5,    63,
      70,    64,    -1,     3,    -1,    39,    -1,    40,    -1,    69,
      70,    65,    -1,    69,     4,    66,    80,    67,    65,    -1,
      69,     4,    62,    80,    65,    -1,    34,    69,     4,    62,
      80,    65,    -1,    32,    33,    66,    80,    57,    80,    67,
       4,    65,    -1,    55,    71,    65,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    98,    98,   102,   106,   110,   114,   118,   126,   131,
     138,   144,   149,   154,   164,   168,   172,   176,   180,   184,
     189,   190,   191,   195,   196,   197,   198,   201,   203,   207,
     209,   214,   219,   224,   256,   261,   269,   270,   273,   279,
     291,   319,   335,   339,   345,   395,   440,   456,   475,   487
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CONSTANT", "IDENTIFIER", "TEMPLATE",
  "STRING_LITERAL", "URGENT", "BROADCAST", "META", "PTR_OP", "INC_OP",
  "DEC_OP", "LEFT_OP", "RIGHT_OP", "LE_OP", "GE_OP", "EQ_OP", "NE_OP",
  "AND_OP", "OR_OP", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN",
  "ADD_ASSIGN", "SUB_ASSIGN", "LEFT_ASSIGN", "RIGHT_ASSIGN", "AND_ASSIGN",
  "XOR_ASSIGN", "OR_ASSIGN", "TYPE_NAME", "TYPEDEF", "INT", "CONST",
  "VOID", "CLOCK", "CHAN", "BOOL", "TRUE_Y", "FALSE_Y", "STRUCT",
  "ELLIPSIS", "COMMENT", "CASE", "DEFAULT", "IF", "ELSE", "SWITCH",
  "WHILE", "DO", "FOR", "CONTINUE", "BREAK", "RETURN", "SYSTEM", "IFX",
  "','", "'<'", "'>'", "'-'", "'!'", "'='", "'('", "')'", "';'", "'['",
  "']'", "$accept", "type_specifier", "identifier_list", "ta_list",
  "compare_relation", "translation_unit", "external_declaration",
  "constraint_statement", "comment_list", "atomic_constraint",
  "assign_statement", "single_assign_statement", "const_expression",
  "variable_declaration", "system_declaration", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,    44,    60,    62,
      45,    33,    61,    40,    41,    59,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    68,    69,    69,    69,    69,    69,    69,    70,    70,
      71,    71,    71,    71,    72,    72,    72,    72,    72,    72,
      73,    73,    73,    74,    74,    74,    74,    75,    75,    76,
      76,    77,    77,    77,    77,    77,    78,    78,    79,    79,
      79,    80,    80,    80,    81,    81,    81,    81,    81,    82
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     2,     2,     1,     3,
       1,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     3,     1,
       2,     3,     3,     5,     1,     2,     1,     3,     3,     3,
       6,     1,     1,     1,     3,     6,     5,     6,     9,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    34,     0,     0,     0,     2,     0,     3,     5,     4,
      29,     0,     0,     0,     0,    20,    24,    22,    27,    25,
      36,    23,    26,    15,    16,    14,    19,    17,    18,     0,
       0,     0,     6,     7,     0,     0,    10,    11,     0,    35,
       8,     0,     1,    21,     0,    30,     0,     0,    41,    39,
       0,    42,    43,    38,    32,    31,     0,     0,     0,    49,
       0,     0,     0,    44,    34,    28,     0,    37,     0,     0,
       0,     0,    12,    13,     0,     0,     9,    33,     8,     0,
       0,     0,    46,     0,    40,     0,    47,    45,     0,     0,
      48
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    13,    41,    38,    31,    14,    15,    16,    17,    18,
      19,    20,    53,    21,    22
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -58
static const yytype_int8 yypact[] =
{
       7,     5,   -34,   -31,     2,   -58,    69,   -58,   -58,   -58,
     -58,    26,    15,    74,     0,   -58,    60,    37,   -58,    24,
     -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,    78,
      48,     9,   -58,   -58,    17,    80,   -58,   -58,   -55,   -58,
     -57,   -41,   -58,   -58,    -3,   -58,    81,    57,   -58,   -58,
      23,   -58,   -58,   -58,   -58,   -58,    53,    27,    55,   -58,
      53,    53,    86,   -58,    11,   -58,    29,   -58,    53,    90,
      38,    53,   -58,   -58,    31,    30,   -58,   -58,   -58,   -39,
      53,    33,   -58,    34,   -58,    36,   -58,   -58,    96,    39,
     -58
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -58,    95,    40,   -58,    61,   -58,    97,   -58,   -58,    66,
     -58,    67,   -14,   -58,   -58
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      42,    64,    58,    32,     1,    60,    33,     2,     3,    61,
      59,     1,    48,    54,     2,     3,    62,    55,    62,    39,
      23,    24,    25,    26,    63,    84,    23,    24,    25,    26,
      36,    37,     4,     5,     6,    34,     7,     8,     9,     4,
       5,     6,    70,     7,     8,     9,    74,    75,    51,    52,
      10,    48,    49,    50,    77,    11,    48,    81,    12,    72,
      73,    12,    11,    27,    28,    29,    85,    30,    12,    27,
      28,    29,    23,    24,    25,    26,     2,     3,    40,    44,
      45,    46,    47,    56,    57,    66,    69,    51,    52,    71,
      76,    30,    51,    52,    78,    80,    82,    83,    86,    87,
      89,    35,     5,    88,    90,     7,     8,     9,    68,    79,
      65,    43,     0,    67,     0,    27,    28
};

static const yytype_int8 yycheck[] =
{
       0,     4,    57,    37,     4,    62,    37,     7,     8,    66,
      65,     4,     3,     4,     7,     8,    57,    31,    57,     4,
      15,    16,    17,    18,    65,    64,    15,    16,    17,    18,
       4,     5,    32,    33,    34,    33,    36,    37,    38,    32,
      33,    34,    56,    36,    37,    38,    60,    61,    39,    40,
      43,     3,     4,     5,    68,    55,     3,    71,    61,     4,
       5,    61,    55,    58,    59,    60,    80,    62,    61,    58,
      59,    60,    15,    16,    17,    18,     7,     8,     4,    19,
      43,    57,     4,    66,     4,     4,    63,    39,    40,    62,
       4,    62,    39,    40,     4,    57,    65,    67,    65,    65,
       4,     6,    33,    67,    65,    36,    37,    38,    47,    69,
      44,    14,    -1,    46,    -1,    58,    59
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     7,     8,    32,    33,    34,    36,    37,    38,
      43,    55,    61,    69,    73,    74,    75,    76,    77,    78,
      79,    81,    82,    15,    16,    17,    18,    58,    59,    60,
      62,    72,    37,    37,    33,    69,     4,     5,    71,     4,
       4,    70,     0,    74,    19,    43,    57,     4,     3,     4,
       5,    39,    40,    80,     4,    80,    66,     4,    57,    65,
      62,    66,    57,    65,     4,    77,     4,    79,    72,    63,
      80,    62,     4,     5,    80,    80,     4,    80,     4,    70,
      57,    80,    65,    67,    64,    80,    65,    65,    67,     4,
      65
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
     `$$ = $1'.

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
#line 99 "src/uppaalparser.y"
    {
  (yyval.type_value)=INT_T;
;}
    break;

  case 3:
#line 103 "src/uppaalparser.y"
    {
  (yyval.type_value)=CLOCK_T;
;}
    break;

  case 4:
#line 107 "src/uppaalparser.y"
    {
  (yyval.type_value)=BOOL_T;
;}
    break;

  case 5:
#line 111 "src/uppaalparser.y"
    {
  (yyval.type_value)=CHAN_T;
;}
    break;

  case 6:
#line 115 "src/uppaalparser.y"
    {
  (yyval.type_value)=URGENT_CHAN_T;
;}
    break;

  case 7:
#line 119 "src/uppaalparser.y"
    {
  (yyval.type_value)=BROADCAST_CHAN_T;
;}
    break;

  case 8:
#line 127 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=new vector<string> ( );
  (yyval.str_vec_pointer)->push_back( symbol_table[(yyvsp[(1) - (1)].int_value)]);
;}
    break;

  case 9:
#line 132 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=(yyvsp[(1) - (3)].str_vec_pointer);
  (yyval.str_vec_pointer)->push_back(symbol_table[(yyvsp[(3) - (3)].int_value)]);
;}
    break;

  case 10:
#line 139 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=new vector<string> ( );
  (yyval.str_vec_pointer)->push_back( symbol_table[(yyvsp[(1) - (1)].int_value)]);
;}
    break;

  case 11:
#line 145 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=new vector<string> ( );
  (yyval.str_vec_pointer)->push_back( symbol_table[(yyvsp[(1) - (1)].int_value)]);
;}
    break;

  case 12:
#line 150 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=(yyvsp[(1) - (3)].str_vec_pointer);
  (yyval.str_vec_pointer)->push_back(symbol_table[(yyvsp[(3) - (3)].int_value)]);
;}
    break;

  case 13:
#line 155 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=(yyvsp[(1) - (3)].str_vec_pointer);
  (yyval.str_vec_pointer)->push_back(symbol_table[(yyvsp[(3) - (3)].int_value)]);
;}
    break;

  case 14:
#line 164 "src/uppaalparser.y"
    {
  (yyval.com_op)=EQ;
;}
    break;

  case 15:
#line 168 "src/uppaalparser.y"
    {
  (yyval.com_op)=LE;
;}
    break;

  case 16:
#line 172 "src/uppaalparser.y"
    {
  (yyval.com_op)=GE;
;}
    break;

  case 17:
#line 176 "src/uppaalparser.y"
    {
  (yyval.com_op)=LT;
;}
    break;

  case 18:
#line 180 "src/uppaalparser.y"
    {
  (yyval.com_op)=GT;
;}
    break;

  case 19:
#line 184 "src/uppaalparser.y"
    {
  (yyval.com_op)=NE;
;}
    break;

  case 31:
#line 215 "src/uppaalparser.y"
    {
  model_parser->parseConstraint( current_data, symbol_table[(yyvsp[(1) - (3)].int_value)], (yyvsp[(2) - (3)].com_op), (yyvsp[(3) - (3)].int_value) );
;}
    break;

  case 32:
#line 220 "src/uppaalparser.y"
    {
  model_parser->parseConstraint( current_data, symbol_table[ (yyvsp[(1) - (3)].int_value)], (yyvsp[(2) - (3)].com_op),symbol_table[ (yyvsp[(3) - (3)].int_value)] );
;}
    break;

  case 33:
#line 225 "src/uppaalparser.y"
    {
  model_parser->parseConstraint( current_data, symbol_table[ (yyvsp[(1) - (5)].int_value)], symbol_table[ (yyvsp[(3) - (5)].int_value)], (yyvsp[(4) - (5)].com_op), (yyvsp[(5) - (5)].int_value) );
    
  /* if(getType( symbol_table[$1])==CLOCK_T &&getType( symbol_table[$3])==CLOCK_T  ){ */
  /*   int clock_id1=current_data->getId( CLOCK_STR, symbol_table[$1])+1; */
  /*   int clock_id2=current_data->getId( CLOCK_STR, symbol_table[$3])+1; */
  /*   if( $4==EQ){ */
      
  /*     INT_TAS_t::CS_t *cs=new      INT_TAS_t::CS_t(clock_id1, clock_id2, GE,  $5  ); //x-y< rhs */

  /*     current_data->addPointer( CLOCK_CS,CLOCK_CS, cs); */
      
  /*     cs=new      INT_TAS_t::CS_t(clock_id1, clock_id2, LE,  $5  ); //x-y< rhs */

  /*     current_data->addPointer( CLOCK_CS,CLOCK_CS, cs);                      */
  /*   }else{ */
  /*     INT_TAS_t::CS_t *cs=new      INT_TAS_t::CS_t(clock_id1, clock_id2, $4,  $5  ); //x-y< rhs */
  /*     current_data->addPointer( CLOCK_CS,CLOCK_CS, cs);                      */
  /*   } */

  /* } */
  /* else if(getType( symbol_table[$1])==INT_T &&getType( symbol_table[$3])==INT_T  ){ */
  /*   int counter_id1=system_data->getId( INT_STR, symbol_table[$1]); */
  /*   int counter_id2=system_data->getId( INT_STR, symbol_table[$3]); */
  /*   DiaCounterConstraint *cs=InstanceFactory::getInstance().createDiaCounterConstraint( counter_id1, counter_id2, $4, $5); */
  /*   current_data->addPointer( INT_CS,INT_CS, cs); */
  /* } */
  

;}
    break;

  case 34:
#line 257 "src/uppaalparser.y"
    {
  model_parser->parseConstraint( current_data, symbol_table[ (yyvsp[(1) - (1)].int_value)] );
;}
    break;

  case 35:
#line 262 "src/uppaalparser.y"
    {
  model_parser->parseConstraint( current_data, symbol_table[ (yyvsp[(2) - (2)].int_value)], false );
  
;}
    break;

  case 38:
#line 274 "src/uppaalparser.y"
    {
  model_parser->parseAssign( current_data, symbol_table[(yyvsp[(1) - (3)].int_value)], (yyvsp[(3) - (3)].int_value) );
  
;}
    break;

  case 39:
#line 280 "src/uppaalparser.y"
    {
  assert(getType(symbol_table[(yyvsp[(1) - (3)].int_value)] )==INT_T );
  int counter_id=system_data->getId( INT_STR, symbol_table[(yyvsp[(1) - (3)].int_value)]);
    
  int parameter_id=getParameterId(symbol_table[(yyvsp[(3) - (3)].int_value)] );

  SimpleCounterPAction *cs  =InstanceFactory::getInstance( ).createSimpleCounterPAction( counter_id,  parameter_id);
  current_data->addPointer( INT_UPDATE,INT_UPDATE, cs);
  
;}
    break;

  case 40:
#line 292 "src/uppaalparser.y"
    {
  
  TaDec * ta=new TaDec( );
  ta->name=symbol_table[ (yyvsp[(1) - (6)].int_value)];
  
  for(auto name: ( *( (yyvsp[(5) - (6)].str_vec_pointer)) ) ){
    
    TYPE_T type=getType(name );
    int id=-1;
    if(INT_T==type ){
      id= system_data->getId(INT_STR,  name );
    }else if(BOOL_T==type ){
      id= system_data->getId( BOOL_STR, name);
    }else if( CHAN_T==type){
      id=system_data->getId( CHAN_STR, name);
    }
    assert( id>-1);
    ta->param_list.push_back( id);
  }
  
  system_data->addPointer(TEMPLATE_STR,symbol_table[ (yyvsp[(1) - (6)].int_value)],  ta);
  
  delete (yyvsp[(5) - (6)].str_vec_pointer);
;}
    break;

  case 41:
#line 320 "src/uppaalparser.y"
    {
  (yyval.int_value)=(yyvsp[(1) - (1)].int_value);
;}
    break;

  case 42:
#line 335 "src/uppaalparser.y"
    {
  (yyval.int_value)=1;
;}
    break;

  case 43:
#line 339 "src/uppaalparser.y"
    {
  (yyval.int_value)=0;
;}
    break;

  case 44:
#line 346 "src/uppaalparser.y"
    {
  switch( (yyvsp[(1) - (3)].type_value)){
    case INT_T:{
      for( auto v: *(yyvsp[(2) - (3)].str_vec_pointer)){
        system_data->setValue(INT_STR, current_data->getVarFullName(v)); //All the variables  in system_data
      }
      delete (yyvsp[(2) - (3)].str_vec_pointer);
      break ;
    }
    case CLOCK_T:{
      for( auto v: *(yyvsp[(2) - (3)].str_vec_pointer)){
        current_data->setValue(CLOCK_STR, v);  //clock variable only can declare in template
      }
      delete (yyvsp[(2) - (3)].str_vec_pointer);
      break ;
    }

    case BOOL_T: {
      for( auto v: *(yyvsp[(2) - (3)].str_vec_pointer)){
        system_data->setValue(BOOL_STR, current_data->getVarFullName(v));
      }
      delete (yyvsp[(2) - (3)].str_vec_pointer);
      break ;
    }
    case CHAN_T: {
      for( auto v: *(yyvsp[(2) - (3)].str_vec_pointer)){
        system_data->setValue(CHAN_STR, current_data->getVarFullName(v), ONE2ONE_CH);
      }
      delete (yyvsp[(2) - (3)].str_vec_pointer);
      break ;
    }

    case URGENT_CHAN_T:{
      for( auto v: *(yyvsp[(2) - (3)].str_vec_pointer)){
        system_data->setValue(CHAN_STR, current_data->getVarFullName(v), URGENT_CH);
      }
      delete (yyvsp[(2) - (3)].str_vec_pointer);
      break ;
    }
    case BROADCAST_CHAN_T:{
      for( auto v: *(yyvsp[(2) - (3)].str_vec_pointer)){
        system_data->setValue(CHAN_STR, current_data->getVarFullName(v), BROADCAST_CH);
      }
      delete (yyvsp[(2) - (3)].str_vec_pointer);
      break ;
    }
  }
;}
    break;

  case 45:
#line 396 "src/uppaalparser.y"
    {
  string name=symbol_table[ (yyvsp[(2) - (6)].int_value)];
  switch( (yyvsp[(1) - (6)].type_value)){
    case INT_T:{
      for( int i=0; i< (yyvsp[(4) - (6)].int_value); i++){
        system_data->setValue( INT_STR, current_data->getVarFullName(arrayToVar(name, i)) );
      }
      break;
    }
    case CLOCK_T: {
      for( int i=0; i< (yyvsp[(4) - (6)].int_value); i++){
        current_data->setValue( CLOCK_STR, arrayToVar(name, i ));
      }
      break;
    }

    case BOOL_T:{
      for( int i=0; i< (yyvsp[(4) - (6)].int_value); i++){
        system_data->setValue( BOOL_STR, current_data->getVarFullName(arrayToVar(name, i )));
      }
      break;
    }
     
    case CHAN_T:{
      for( int i=0; i< (yyvsp[(4) - (6)].int_value); i++){
        system_data->setValue( CHAN_STR, current_data->getVarFullName(arrayToVar(name, i )), ONE2ONE_CH);
      }
      break;
    }
    case URGENT_CHAN_T:{
      for( int i=0; i< (yyvsp[(4) - (6)].int_value); i++){
        system_data->setValue( CHAN_STR, current_data->getVarFullName(arrayToVar(name, i )), URGENT_CH);
      }
      break;
    }
    case BROADCAST_CHAN_T: {
      for( int i=0; i< (yyvsp[(4) - (6)].int_value); i++){
        system_data->setValue( CHAN_STR, current_data->getVarFullName(arrayToVar(name, i )), BROADCAST_CH);
      }
      break;
    }
  }
;}
    break;

  case 46:
#line 441 "src/uppaalparser.y"
    {
  switch( (yyvsp[(1) - (5)].type_value)){
    case INT_T:
      system_data->setValue(INT_STR, current_data->getVarFullName(symbol_table[(yyvsp[(2) - (5)].int_value)]), (yyvsp[(4) - (5)].int_value));
      break;
    case CLOCK_T:
      current_data->setValue(CLOCK_STR, symbol_table[(yyvsp[(2) - (5)].int_value)], (yyvsp[(4) - (5)].int_value));
      break;
      
    case BOOL_T:
      system_data->setValue(BOOL_STR, current_data->getVarFullName(symbol_table[(yyvsp[(2) - (5)].int_value)]), (yyvsp[(4) - (5)].int_value));
      break;
  }
  
;}
    break;

  case 47:
#line 458 "src/uppaalparser.y"
    {
  switch( (yyvsp[(2) - (6)].type_value)){
    case INT_T:
      system_data->setValue(INT_STR, current_data->getVarFullName(symbol_table[(yyvsp[(3) - (6)].int_value)]), (yyvsp[(5) - (6)].int_value));
      break;
    case CLOCK_T:
      current_data->setValue(CLOCK_STR, symbol_table[(yyvsp[(3) - (6)].int_value)], (yyvsp[(5) - (6)].int_value));
      break;

    case BOOL_T:
      system_data->addValue(BOOL_STR, current_data->getVarFullName(symbol_table[(yyvsp[(3) - (6)].int_value)]), (yyvsp[(5) - (6)].int_value));
      break;
  }
  
;}
    break;

  case 48:
#line 476 "src/uppaalparser.y"
    {

  vector<int> temp;
  for( int i=(yyvsp[(4) - (9)].int_value); i<=(yyvsp[(6) - (9)].int_value); i++  ){
    temp.push_back( i);
  }
  system_data->addIntArray(current_data->getVarFullName(symbol_table[(yyvsp[(8) - (9)].int_value)]),temp);
;}
    break;

  case 49:
#line 488 "src/uppaalparser.y"
    {
  SystemDec * sys=new SystemDec( );
  for( size_t i=0; i< (yyvsp[(2) - (3)].str_vec_pointer)->size( ); i++){
    string name=(*(yyvsp[(2) - (3)].str_vec_pointer))[ i];
    if(system_data->hasValue(TEMPLATE_STR, name  )){
      
      TaDec* temp=new TaDec( );
      temp->no_parameter=true;
      temp->name= name;
      sys->ta_list.push_back(temp );
    }else{
      void* sys_dec=system_data->getPointer(TEMPLATE_STR, name );
      sys->ta_list.push_back((TaDec*)sys_dec );
      
    }
  }
  
  system_data->clearPoints(TEMPLATE_STR );
  
  system_data->addPointer(SYSTEM_STR, SYSTEM_STR,sys );
  delete (yyvsp[(2) - (3)].str_vec_pointer);
;}
    break;


/* Line 1267 of yacc.c.  */
#line 2002 "src/uppaalparser.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 511 "src/uppaalparser.y"


#include "uppaalscan.h"
   
/* extern int column; */

void yyerror(const char *s)
{
    fflush(stdout);
    assert( false);
    printf("\n jjj error\n");
}
void yyerror(const string &s){

  fflush(stdout);
  assert( false);
  printf("\n kkk error\n");
  
}





namespace graphsat{
  void parseProblem( const string &str, UppaalParser* parser,  UppaalTemplateData *pd,   UppaalTemplateData* d){
    
    model_parser=parser;
    system_data=pd;
    current_data=d;
    yyin=tmpfile();
    fputs( str.c_str(), yyin);
    rewind(yyin);
    yyparse();
    fclose(yyin);
  
  }
} 



