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
     TEMPLATE_YY = 260,
     AUTOMATA_YY = 261,
     FORMAL_PARAMETER_YY = 262,
     PARAMETER_INT_YY = 263,
     REF_PARAMETER_INT_YY = 264,
     PARAMETER_CLOCK_YY = 265,
     REF_PARAMETER_CLOCK_YY = 266,
     REF_PARAMETER_BOOL_YY = 267,
     REF_PARAMETER_CHAN_YY = 268,
     CLOCK_YY = 269,
     INT_YY = 270,
     CHAN_YY = 271,
     SELF_DEF_YY = 272,
     STRING_LITERAL = 273,
     URGENT = 274,
     BROADCAST = 275,
     META = 276,
     PTR_OP = 277,
     INC_OP = 278,
     DEC_OP = 279,
     LEFT_OP = 280,
     RIGHT_OP = 281,
     LE_OP = 282,
     GE_OP = 283,
     EQ_OP = 284,
     NE_OP = 285,
     AND_OP = 286,
     OR_OP = 287,
     MUL_ASSIGN = 288,
     DIV_ASSIGN = 289,
     MOD_ASSIGN = 290,
     ADD_ASSIGN = 291,
     SUB_ASSIGN = 292,
     LEFT_ASSIGN = 293,
     RIGHT_ASSIGN = 294,
     AND_ASSIGN = 295,
     XOR_ASSIGN = 296,
     OR_ASSIGN = 297,
     TYPE_NAME = 298,
     TYPEDEF = 299,
     INT = 300,
     CONST = 301,
     VOID = 302,
     CLOCK = 303,
     CHAN = 304,
     BOOL = 305,
     TRUE_YY = 306,
     FALSE_YY = 307,
     STRUCT = 308,
     ELLIPSIS = 309,
     COMMENT = 310,
     CASE = 311,
     DEFAULT = 312,
     IF = 313,
     ELSE = 314,
     SWITCH = 315,
     WHILE = 316,
     DO = 317,
     FOR = 318,
     CONTINUE = 319,
     BREAK = 320,
     RETURN = 321,
     SYSTEM = 322,
     ARGUMENT = 323,
     IFX = 324
   };
#endif
/* Tokens.  */
#define CONSTANT 258
#define IDENTIFIER 259
#define TEMPLATE_YY 260
#define AUTOMATA_YY 261
#define FORMAL_PARAMETER_YY 262
#define PARAMETER_INT_YY 263
#define REF_PARAMETER_INT_YY 264
#define PARAMETER_CLOCK_YY 265
#define REF_PARAMETER_CLOCK_YY 266
#define REF_PARAMETER_BOOL_YY 267
#define REF_PARAMETER_CHAN_YY 268
#define CLOCK_YY 269
#define INT_YY 270
#define CHAN_YY 271
#define SELF_DEF_YY 272
#define STRING_LITERAL 273
#define URGENT 274
#define BROADCAST 275
#define META 276
#define PTR_OP 277
#define INC_OP 278
#define DEC_OP 279
#define LEFT_OP 280
#define RIGHT_OP 281
#define LE_OP 282
#define GE_OP 283
#define EQ_OP 284
#define NE_OP 285
#define AND_OP 286
#define OR_OP 287
#define MUL_ASSIGN 288
#define DIV_ASSIGN 289
#define MOD_ASSIGN 290
#define ADD_ASSIGN 291
#define SUB_ASSIGN 292
#define LEFT_ASSIGN 293
#define RIGHT_ASSIGN 294
#define AND_ASSIGN 295
#define XOR_ASSIGN 296
#define OR_ASSIGN 297
#define TYPE_NAME 298
#define TYPEDEF 299
#define INT 300
#define CONST 301
#define VOID 302
#define CLOCK 303
#define CHAN 304
#define BOOL 305
#define TRUE_YY 306
#define FALSE_YY 307
#define STRUCT 308
#define ELLIPSIS 309
#define COMMENT 310
#define CASE 311
#define DEFAULT 312
#define IF 313
#define ELSE 314
#define SWITCH 315
#define WHILE 316
#define DO 317
#define FOR 318
#define CONTINUE 319
#define BREAK 320
#define RETURN 321
#define SYSTEM 322
#define ARGUMENT 323
#define IFX 324




/* Copy the first part of user declarations.  */
#line 1 "src/uppaalparser.y"

  #include<iostream>
  #include<string>
  #include <stdio.h>
  #include<map>
  #include<vector>
  #include <utility>
  #include "model/channel.h"
  #include "util/dbmutil.hpp"
  #include "io/uppaaldata.h"

  #include "model/ta.hpp"
#define  LEX_RETURN( T) case T##_T: return T##_YY 
  using std::string;
  using std::map;
  using std::vector;
  using std::make_pair;
      
  using namespace graphsat;
  
  int yydebug=1;   
  using std::cerr;
  using std::endl;
  using std::cout;
  extern  int lineNum;


  UppaalData* current_data;
  
  extern void yyerror(const char *); 
  extern void yyerror(const string&);
  extern int yylex();
  extern int yyparse();
  extern FILE* yyin;

  struct Token{
    int id;
    string  symbol;
  };
  
 

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
#line 43 "src/uppaalparser.y"
{
  int int_value;
  Token* token_value;
  TYPE_T type_value;
  vector<string> * str_vec_pointer;
  vector<RealParameterItem>* item_vec_pointer;
  vector<FormalParameterItem>* para_element_pointer;
  COMP_OPERATOR com_op;

 }
/* Line 193 of yacc.c.  */
#line 287 "src/uppaalparser.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 300 "src/uppaalparser.cpp"

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
#define YYFINAL  69
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   327

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  83
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  26
/* YYNRULES -- Number of rules.  */
#define YYNRULES  108
/* YYNRULES -- Number of states.  */
#define YYNSTATES  189

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   324

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    79,     2,     2,     2,     2,    70,     2,
      74,    75,     2,     2,    73,    78,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    82,
      76,    81,    77,    80,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    71,     2,    72,     2,     2,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    14,    17,    19,
      22,    25,    29,    31,    36,    38,    43,    45,    50,    52,
      56,    59,    64,    67,    71,    73,    75,    77,    81,    85,
      89,    91,    93,    97,   101,   105,   111,   113,   115,   117,
     119,   121,   123,   125,   128,   130,   133,   135,   137,   139,
     141,   143,   145,   147,   151,   153,   155,   157,   159,   161,
     165,   169,   173,   177,   181,   185,   189,   195,   199,   203,
     207,   211,   217,   223,   229,   235,   237,   240,   242,   244,
     247,   250,   253,   256,   258,   262,   266,   270,   274,   278,
     282,   286,   290,   294,   298,   302,   310,   314,   318,   321,
     324,   326,   328,   330,   334,   341,   347,   357,   360
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      94,     0,    -1,    45,    -1,    48,    -1,    50,    -1,    49,
      -1,    19,    49,    -1,    20,    49,    -1,    84,    -1,    46,
      84,    -1,    84,    70,    -1,    46,    84,    70,    -1,    14,
      -1,    86,    71,   105,    72,    -1,    15,    -1,    87,    71,
     105,    72,    -1,    16,    -1,    88,    71,   105,    72,    -1,
       4,    -1,    89,    73,     4,    -1,    85,     4,    -1,    90,
      73,    85,     4,    -1,    17,     4,    -1,    46,    17,     4,
      -1,    98,    -1,   100,    -1,   105,    -1,    91,    73,    98,
      -1,    91,    73,   100,    -1,    91,    73,   105,    -1,     6,
      -1,     5,    -1,     5,    74,    75,    -1,    92,    73,     6,
      -1,    92,    73,     5,    -1,    92,    73,     5,    74,    75,
      -1,    29,    -1,    27,    -1,    28,    -1,    76,    -1,    77,
      -1,    30,    -1,    95,    -1,    94,    95,    -1,    55,    -1,
      94,    55,    -1,   106,    -1,    96,    -1,   102,    -1,   104,
      -1,   107,    -1,   108,    -1,   101,    -1,    96,    31,   101,
      -1,    86,    -1,    87,    -1,     7,    -1,     8,    -1,    88,
      -1,    97,    93,   105,    -1,   105,    93,    97,    -1,    97,
      93,    99,    -1,    99,    93,    97,    -1,    99,    93,   105,
      -1,   105,    93,    99,    -1,    99,    93,    99,    -1,    99,
      78,    99,    93,   105,    -1,    98,    93,   105,    -1,   105,
      93,    98,    -1,    98,    93,    99,    -1,    99,    93,    98,
      -1,    97,    78,    97,    93,   105,    -1,    97,    78,    97,
      93,    99,    -1,    98,    78,    98,    93,   105,    -1,    98,
      78,    98,    93,    99,    -1,    98,    -1,    79,    98,    -1,
       8,    -1,     9,    -1,    79,     8,    -1,    79,     9,    -1,
      13,    80,    -1,    13,    79,    -1,   103,    -1,   102,    73,
     103,    -1,    97,    81,   105,    -1,    98,    81,   105,    -1,
      98,    81,    98,    -1,    98,    81,    99,    -1,    98,    36,
     105,    -1,    98,    36,    98,    -1,    98,    36,    99,    -1,
      98,    37,   105,    -1,    98,    37,    98,    -1,    98,    37,
      99,    -1,     4,    81,     5,    74,    91,    75,    82,    -1,
       8,    81,   105,    -1,     9,    81,   105,    -1,   100,    79,
      -1,   100,    80,    -1,     3,    -1,    51,    -1,    52,    -1,
      85,    89,    82,    -1,    85,     4,    71,   105,    72,    82,
      -1,    85,     4,    81,   105,    82,    -1,    44,    45,    71,
     105,    73,   105,    72,     4,    82,    -1,    68,    90,    -1,
      67,    92,    82,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   135,   135,   139,   143,   147,   151,   155,   163,   168,
     173,   178,   185,   190,   198,   203,   213,   218,   226,   232,
     242,   253,   264,   276,   290,   299,   307,   315,   324,   333,
     344,   352,   359,   365,   371,   377,   389,   393,   397,   401,
     405,   409,   414,   415,   416,   417,   421,   422,   423,   424,
     425,   426,   429,   431,   435,   441,   448,   453,   460,   467,
     472,   478,   483,   489,   498,   509,   519,   529,   539,   551,
     561,   572,   577,   582,   591,   601,   610,   619,   631,   643,
     656,   669,   679,   691,   692,   696,   702,   710,   718,   726,
     734,   742,   751,   759,   767,   776,   790,   800,   814,   819,
     826,   831,   835,   841,   883,   933,   951,   962,   976
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CONSTANT", "IDENTIFIER", "TEMPLATE_YY",
  "AUTOMATA_YY", "FORMAL_PARAMETER_YY", "PARAMETER_INT_YY",
  "REF_PARAMETER_INT_YY", "PARAMETER_CLOCK_YY", "REF_PARAMETER_CLOCK_YY",
  "REF_PARAMETER_BOOL_YY", "REF_PARAMETER_CHAN_YY", "CLOCK_YY", "INT_YY",
  "CHAN_YY", "SELF_DEF_YY", "STRING_LITERAL", "URGENT", "BROADCAST",
  "META", "PTR_OP", "INC_OP", "DEC_OP", "LEFT_OP", "RIGHT_OP", "LE_OP",
  "GE_OP", "EQ_OP", "NE_OP", "AND_OP", "OR_OP", "MUL_ASSIGN", "DIV_ASSIGN",
  "MOD_ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "LEFT_ASSIGN", "RIGHT_ASSIGN",
  "AND_ASSIGN", "XOR_ASSIGN", "OR_ASSIGN", "TYPE_NAME", "TYPEDEF", "INT",
  "CONST", "VOID", "CLOCK", "CHAN", "BOOL", "TRUE_YY", "FALSE_YY",
  "STRUCT", "ELLIPSIS", "COMMENT", "CASE", "DEFAULT", "IF", "ELSE",
  "SWITCH", "WHILE", "DO", "FOR", "CONTINUE", "BREAK", "RETURN", "SYSTEM",
  "ARGUMENT", "IFX", "'&'", "'['", "']'", "','", "'('", "')'", "'<'",
  "'>'", "'-'", "'!'", "'?'", "'='", "';'", "$accept",
  "base_type_specifier", "type_specifier", "clock_yy", "int_yy", "chan_yy",
  "identifier_list", "formal_argument_list", "real_argument_list",
  "timed_automata_list", "compare_relation", "translation_unit",
  "external_declaration", "constraint_statement", "clock_identifier",
  "counter_identifier", "parameter_identifier", "chan_identifier",
  "atomic_constraint", "assign_statement", "single_assign_statement",
  "communicate_statement", "const_expression", "variable_declaration",
  "template_declaration", "system_declaration", 0
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
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
      38,    91,    93,    44,    40,    41,    60,    62,    45,    33,
      63,    61,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    83,    84,    84,    84,    84,    84,    84,    85,    85,
      85,    85,    86,    86,    87,    87,    88,    88,    89,    89,
      90,    90,    90,    90,    91,    91,    91,    91,    91,    91,
      92,    92,    92,    92,    92,    92,    93,    93,    93,    93,
      93,    93,    94,    94,    94,    94,    95,    95,    95,    95,
      95,    95,    96,    96,    97,    98,    99,    99,   100,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   102,   102,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   104,   104,
     105,   105,   105,   106,   106,   106,   106,   107,   108
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     2,     2,     1,     2,
       2,     3,     1,     4,     1,     4,     1,     4,     1,     3,
       2,     4,     2,     3,     1,     1,     1,     3,     3,     3,
       1,     1,     3,     3,     3,     5,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     5,     3,     3,     3,
       3,     5,     5,     5,     5,     1,     2,     1,     1,     2,
       2,     2,     2,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     7,     3,     3,     2,     2,
       1,     1,     1,     3,     6,     5,     9,     2,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   100,     0,    56,    77,    78,     0,    12,    14,    16,
       0,     0,     0,     2,     0,     3,     5,     4,   101,   102,
      44,     0,     0,     0,     8,     0,    54,    55,    58,     0,
      42,    47,     0,    75,     0,     0,    52,    48,    83,    49,
       0,    46,    50,    51,     0,     0,     0,    82,    81,     6,
       7,     0,     9,    31,    30,     0,     0,     0,     0,   107,
      79,    80,    76,    10,    18,     0,     0,     0,     0,     1,
      45,    43,     0,    37,    38,    36,    41,    39,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
      99,     0,     0,     0,    96,    97,     0,    11,     0,     0,
     108,    22,     0,    20,     0,     0,     0,     0,   103,     0,
       0,     0,    77,    78,     0,    75,    53,     0,    85,    57,
      61,    59,    90,    91,    89,    93,    94,    92,     0,    87,
      88,    86,    69,    67,     0,    62,    70,    65,    63,     0,
       0,     0,     0,    84,    60,    68,    64,     0,     0,    32,
      34,    33,    23,     0,     0,     0,    19,    13,    15,    17,
       0,     0,     0,     0,    24,    25,    26,     0,     0,    21,
       0,   105,    72,    71,    74,    73,    66,     0,     0,     0,
      35,   104,    27,    28,    29,    95,     0,     0,   106
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    24,    25,    26,    27,    28,    65,    59,   163,    55,
      81,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -73
static const yytype_int16 yypact[] =
{
     137,   -73,   -60,   -73,   166,   -31,   -43,   -73,   -73,   -73,
       4,     9,    25,   -73,   277,   -73,   -73,   -73,   -73,   -73,
     -73,   101,    28,   140,    22,   107,    76,    88,    93,   123,
     -73,   127,   179,    85,   185,    30,   -73,    92,   -73,   -73,
      52,   -73,   -73,   -73,   155,    39,    39,   -73,   -73,   -73,
     -73,    99,   106,   103,   -73,     2,   175,   262,   176,   111,
     -73,   -73,   -73,   -73,   -49,    35,    39,    39,    39,   -73,
     -73,   -73,    20,   -73,   -73,   -73,   -73,   -73,   -73,   183,
      39,    49,   243,   243,   184,   243,    49,    87,   226,   -73,
     -73,    89,   223,   124,   -73,   -73,    39,   -73,   125,   128,
     -73,   -73,   197,   -73,   268,    39,    39,   199,   -73,   138,
     139,   160,   190,   -73,   194,   198,   -73,    52,   -73,   -73,
     -73,   -73,   -73,   -73,   -73,   -73,   -73,   -73,    52,   -73,
     -73,   -73,   -73,   -73,    52,   -73,   -73,   -73,   -73,   154,
     -31,   158,   -30,   -73,   -73,   -73,   -73,    15,   163,   -73,
     171,   -73,   -73,   244,   177,   170,   -73,   -73,   -73,   -73,
      49,    49,    39,   -56,   -73,   -73,   -73,    39,   178,   -73,
     172,   -73,   -73,   -73,   -73,   -73,   -73,    15,   182,   187,
     -73,   -73,   -73,   -73,   -73,   -73,   261,   202,   -73
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -73,    -2,   -17,   -73,   -73,   -73,   -73,   -73,   -73,   -73,
     -32,   -73,   240,   -73,   201,   -23,   -72,   -61,   211,   -73,
     195,   -73,   -42,   -73,   -73,   -73
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -58
static const yytype_int16 yytable[] =
{
      62,    86,    88,    94,    95,    58,    82,    83,    92,   120,
     123,   126,    52,   130,   132,   134,   137,   177,     1,   178,
     146,    44,   105,     1,   109,   110,   111,     3,   112,   113,
       8,     9,   106,     6,     7,     8,    47,    48,   118,   121,
     124,   127,     1,   131,   133,    56,   138,    10,    11,   115,
      46,    85,     1,    49,   148,    52,     3,   119,    50,   122,
     125,   128,   129,   154,   155,   136,    18,    19,   142,   145,
      51,    18,    19,    13,    57,    99,    15,    16,    17,    73,
      74,    75,    76,    86,   100,   160,   165,   153,   172,   174,
      18,    19,    63,     2,     3,   119,   161,   139,   140,    23,
      18,    19,   162,     7,     8,   166,    53,    54,   107,    89,
      90,    64,    73,    74,    75,    76,   183,   108,   173,   175,
     176,    82,    83,    69,   164,   179,     1,     2,    77,    78,
       3,     4,     5,   150,   151,   184,     6,     7,     8,     9,
       1,     2,    10,    11,     3,     4,     5,    66,    60,    61,
       6,     7,     8,     9,   182,     8,    10,    11,    72,    67,
      93,    77,    78,    84,    68,    91,    85,    12,    13,    14,
      96,    15,    16,    17,    18,    19,    97,    98,    70,   101,
     103,    12,    13,    14,   104,    15,    16,    17,    18,    19,
      21,    22,    20,   -57,   -57,   -57,   -57,     7,   147,     8,
     149,   152,    23,   156,    21,    22,    73,    74,    75,    76,
     157,   158,    73,    74,    75,    76,    23,   -57,   -57,   -57,
     -57,    73,    74,    75,    76,    73,    74,    75,    76,     1,
       3,   119,   159,     3,   119,    45,   167,     7,     8,    80,
       7,     8,   -57,   -57,   -57,   168,     1,    45,   169,   170,
       3,   119,   171,   180,   181,    77,    78,    79,     8,   186,
      80,    77,    78,    87,   185,   187,   -57,   -57,   -57,    71,
      77,    78,    79,   114,    77,    78,    84,    18,    19,   102,
     117,    10,    11,   116,   188,     0,   143,    10,    11,   135,
       0,     0,   141,   144,    18,    19,    10,    11,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    13,     0,     0,
      15,    16,    17,    13,    14,     0,    15,    16,    17,     0,
       0,     0,    13,     0,     0,    15,    16,    17
};

static const yytype_int16 yycheck[] =
{
      23,    33,    34,    45,    46,    22,    36,    37,    40,    81,
      82,    83,    14,    85,    86,    87,    88,    73,     3,    75,
      92,    81,    71,     3,    66,    67,    68,     7,     8,     9,
      15,    16,    81,    13,    14,    15,    79,    80,    80,    81,
      82,    83,     3,    85,    86,    17,    88,    19,    20,    72,
      81,    81,     3,    49,    96,    57,     7,     8,    49,    82,
      83,    84,    85,   105,   106,    88,    51,    52,    91,    92,
      45,    51,    52,    45,    46,    73,    48,    49,    50,    27,
      28,    29,    30,   115,    82,   117,   147,   104,   160,   161,
      51,    52,    70,     4,     7,     8,   128,     8,     9,    79,
      51,    52,   134,    14,    15,   147,     5,     6,    73,    79,
      80,     4,    27,    28,    29,    30,   177,    82,   160,   161,
     162,    36,    37,     0,   147,   167,     3,     4,    76,    77,
       7,     8,     9,     5,     6,   177,    13,    14,    15,    16,
       3,     4,    19,    20,     7,     8,     9,    71,     8,     9,
      13,    14,    15,    16,   177,    15,    19,    20,    31,    71,
       5,    76,    77,    78,    71,    73,    81,    44,    45,    46,
      71,    48,    49,    50,    51,    52,    70,    74,    55,     4,
       4,    44,    45,    46,    73,    48,    49,    50,    51,    52,
      67,    68,    55,    27,    28,    29,    30,    14,    74,    15,
      75,     4,    79,     4,    67,    68,    27,    28,    29,    30,
      72,    72,    27,    28,    29,    30,    79,    27,    28,    29,
      30,    27,    28,    29,    30,    27,    28,    29,    30,     3,
       7,     8,    72,     7,     8,    81,    73,    14,    15,    81,
      14,    15,    76,    77,    78,    74,     3,    81,     4,    72,
       7,     8,    82,    75,    82,    76,    77,    78,    15,    72,
      81,    76,    77,    78,    82,     4,    76,    77,    78,    29,
      76,    77,    78,    72,    76,    77,    78,    51,    52,    17,
      79,    19,    20,    72,    82,    -1,    91,    19,    20,    88,
      -1,    -1,    91,    92,    51,    52,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    50,    45,    46,    -1,    48,    49,    50,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    50
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     7,     8,     9,    13,    14,    15,    16,
      19,    20,    44,    45,    46,    48,    49,    50,    51,    52,
      55,    67,    68,    79,    84,    85,    86,    87,    88,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,    81,    81,    81,    79,    80,    49,
      49,    45,    84,     5,     6,    92,    17,    46,    85,    90,
       8,     9,    98,    70,     4,    89,    71,    71,    71,     0,
      55,    95,    31,    27,    28,    29,    30,    76,    77,    78,
      81,    93,    36,    37,    78,    81,    93,    78,    93,    79,
      80,    73,    93,     5,   105,   105,    71,    70,    74,    73,
      82,     4,    17,     4,    73,    71,    81,    73,    82,   105,
     105,   105,     8,     9,    97,    98,   101,    97,   105,     8,
      99,   105,    98,    99,   105,    98,    99,   105,    98,    98,
      99,   105,    99,   105,    99,    97,    98,    99,   105,     8,
       9,    97,    98,   103,    97,    98,    99,    74,   105,    75,
       5,     6,     4,    85,   105,   105,     4,    72,    72,    72,
      93,    93,    93,    91,    98,   100,   105,    73,    74,     4,
      72,    82,    99,   105,    99,   105,   105,    73,    75,   105,
      75,    82,    98,   100,   105,    82,    72,     4,    82
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
#line 136 "src/uppaalparser.y"
    {
  (yyval.type_value)=INT_T;
;}
    break;

  case 3:
#line 140 "src/uppaalparser.y"
    {
  (yyval.type_value)=CLOCK_T;
;}
    break;

  case 4:
#line 144 "src/uppaalparser.y"
    {
  (yyval.type_value)=INT_T;
;}
    break;

  case 5:
#line 148 "src/uppaalparser.y"
    {
  (yyval.type_value)=CHAN_T;
;}
    break;

  case 6:
#line 152 "src/uppaalparser.y"
    {
  (yyval.type_value)=URGENT_CHAN_T;
;}
    break;

  case 7:
#line 156 "src/uppaalparser.y"
    {
  (yyval.type_value)=BROADCAST_CHAN_T;
;}
    break;

  case 8:
#line 164 "src/uppaalparser.y"
    {
  (yyval.type_value)=(yyvsp[(1) - (1)].type_value);
;}
    break;

  case 9:
#line 169 "src/uppaalparser.y"
    {
   (yyval.type_value)=(TYPE_T)(((int)(yyvsp[(2) - (2)].type_value))+2);
;}
    break;

  case 10:
#line 174 "src/uppaalparser.y"
    {
   (yyval.type_value)=(TYPE_T)(((int)(yyvsp[(1) - (2)].type_value))+1);
;}
    break;

  case 11:
#line 179 "src/uppaalparser.y"
    {
  (yyval.type_value)=(TYPE_T)(((int)(yyvsp[(2) - (3)].type_value))+3);
;}
    break;

  case 12:
#line 186 "src/uppaalparser.y"
    {
  (yyval.token_value)=(yyvsp[(1) - (1)].token_value);
;}
    break;

  case 13:
#line 191 "src/uppaalparser.y"
    {
   (yyval.token_value)=(yyvsp[(1) - (4)].token_value);
   (yyval.token_value)->symbol=arrayToVar((yyval.token_value)->symbol, (yyvsp[(3) - (4)].int_value));
;}
    break;

  case 14:
#line 199 "src/uppaalparser.y"
    {
  (yyval.token_value)=(yyvsp[(1) - (1)].token_value);
;}
    break;

  case 15:
#line 204 "src/uppaalparser.y"
    {
  (yyval.token_value)=(yyvsp[(1) - (4)].token_value);
  (yyval.token_value)->symbol=arrayToVar((yyval.token_value)->symbol, (yyvsp[(3) - (4)].int_value));
;}
    break;

  case 16:
#line 214 "src/uppaalparser.y"
    {
  (yyval.token_value)=(yyvsp[(1) - (1)].token_value);
;}
    break;

  case 17:
#line 219 "src/uppaalparser.y"
    {
  (yyval.token_value)=(yyvsp[(1) - (4)].token_value);
  (yyval.token_value)->symbol=arrayToVar((yyval.token_value)->symbol, (yyvsp[(3) - (4)].int_value));
;}
    break;

  case 18:
#line 227 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=new vector<string> ( );
  (yyval.str_vec_pointer)->push_back( (yyvsp[(1) - (1)].token_value)->symbol);
  delete (yyvsp[(1) - (1)].token_value);
;}
    break;

  case 19:
#line 233 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=(yyvsp[(1) - (3)].str_vec_pointer);
  (yyval.str_vec_pointer)->push_back((yyvsp[(3) - (3)].token_value)->symbol);
  delete (yyvsp[(3) - (3)].token_value);
;}
    break;

  case 20:
#line 243 "src/uppaalparser.y"
    {
  (yyval.para_element_pointer)=new vector<FormalParameterItem> ( );
  FormalParameterItem elem;
  elem.name=(yyvsp[(2) - (2)].token_value)->symbol;
  elem.type_name=getTypeStr( (yyvsp[(1) - (2)].type_value));
  elem.type=(yyvsp[(1) - (2)].type_value);
  (yyval.para_element_pointer)->push_back(elem);
  delete (yyvsp[(2) - (2)].token_value);
;}
    break;

  case 21:
#line 254 "src/uppaalparser.y"
    {
  (yyval.para_element_pointer)=(yyvsp[(1) - (4)].para_element_pointer);
  FormalParameterItem elem;
  elem.name=(yyvsp[(4) - (4)].token_value)->symbol;
  elem.type_name=getTypeStr( (yyvsp[(3) - (4)].type_value));
  elem.type=(yyvsp[(3) - (4)].type_value);
  (yyval.para_element_pointer)->push_back(elem);
  delete (yyvsp[(4) - (4)].token_value);
;}
    break;

  case 22:
#line 265 "src/uppaalparser.y"
    {
  (yyval.para_element_pointer)=new vector<FormalParameterItem> ( );
  FormalParameterItem elem;
  elem.name= (yyvsp[(2) - (2)].token_value)->symbol;
  elem.type_name=(yyvsp[(1) - (2)].token_value)->symbol;
  elem.type=SELF_DEF_T;
  (yyval.para_element_pointer)->push_back(elem);
  delete  (yyvsp[(1) - (2)].token_value);
  delete  (yyvsp[(2) - (2)].token_value);
;}
    break;

  case 23:
#line 277 "src/uppaalparser.y"
    {
  (yyval.para_element_pointer)=new vector<FormalParameterItem> ( );
  FormalParameterItem elem;
  elem.name= (yyvsp[(3) - (3)].token_value)->symbol;
  elem.type=SELF_DEF_T;
  elem.type_name=(yyvsp[(2) - (3)].token_value)->symbol;
  (yyval.para_element_pointer)->push_back(elem);
  delete  (yyvsp[(2) - (3)].token_value);
  delete (yyvsp[(3) - (3)].token_value);
;}
    break;

  case 24:
#line 291 "src/uppaalparser.y"
    {
  (yyval.item_vec_pointer)=new vector<RealParameterItem> ( );
  RealParameterItem item;
  item.is_constant=false;
  item.id=(yyvsp[(1) - (1)].int_value);
  (yyval.item_vec_pointer)->push_back( item);
;}
    break;

  case 25:
#line 300 "src/uppaalparser.y"
    {
  (yyval.item_vec_pointer)=new vector<RealParameterItem> ( );
  RealParameterItem item;
  item.is_constant=false;
  item.id=(yyvsp[(1) - (1)].int_value);
  (yyval.item_vec_pointer)->push_back( item);
;}
    break;

  case 26:
#line 308 "src/uppaalparser.y"
    {
  (yyval.item_vec_pointer)=new vector<RealParameterItem> ( );
  RealParameterItem item;
  item.is_constant=true;
  item.id=(yyvsp[(1) - (1)].int_value);
  (yyval.item_vec_pointer)->push_back( item);
;}
    break;

  case 27:
#line 316 "src/uppaalparser.y"
    {
  (yyval.item_vec_pointer)=(yyvsp[(1) - (3)].item_vec_pointer);
  RealParameterItem item;
  item.is_constant=false;
  item.id=(yyvsp[(3) - (3)].int_value);
  (yyval.item_vec_pointer)->push_back(item);
;}
    break;

  case 28:
#line 325 "src/uppaalparser.y"
    {
  (yyval.item_vec_pointer)=(yyvsp[(1) - (3)].item_vec_pointer);
  RealParameterItem item;
  item.is_constant=false;
  item.id=(yyvsp[(3) - (3)].int_value);
  (yyval.item_vec_pointer)->push_back(item);
;}
    break;

  case 29:
#line 334 "src/uppaalparser.y"
    {
  (yyval.item_vec_pointer)=(yyvsp[(1) - (3)].item_vec_pointer);
  RealParameterItem item;
  item.is_constant=true;
  item.id=(yyvsp[(3) - (3)].int_value);
  (yyval.item_vec_pointer)->push_back(item);
;}
    break;

  case 30:
#line 345 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=new vector<string> ( );
  (yyval.str_vec_pointer)->push_back((yyvsp[(1) - (1)].token_value)->symbol);
  delete (yyvsp[(1) - (1)].token_value);
;}
    break;

  case 31:
#line 353 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=new vector<string> ( );
  (yyval.str_vec_pointer)->push_back((yyvsp[(1) - (1)].token_value)->symbol);
  delete (yyvsp[(1) - (1)].token_value);
;}
    break;

  case 32:
#line 360 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=new vector<string> ( );
  (yyval.str_vec_pointer)->push_back((yyvsp[(1) - (3)].token_value)->symbol);
  delete (yyvsp[(1) - (3)].token_value);
;}
    break;

  case 33:
#line 366 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=(yyvsp[(1) - (3)].str_vec_pointer);
  (yyval.str_vec_pointer)->push_back((yyvsp[(3) - (3)].token_value)->symbol);
  delete (yyvsp[(3) - (3)].token_value);
;}
    break;

  case 34:
#line 372 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=(yyvsp[(1) - (3)].str_vec_pointer);
  (yyval.str_vec_pointer)->push_back((yyvsp[(3) - (3)].token_value)->symbol);
  delete (yyvsp[(3) - (3)].token_value);
;}
    break;

  case 35:
#line 378 "src/uppaalparser.y"
    {
  (yyval.str_vec_pointer)=(yyvsp[(1) - (5)].str_vec_pointer);
  (yyval.str_vec_pointer)->push_back((yyvsp[(3) - (5)].token_value)->symbol);
  delete (yyvsp[(3) - (5)].token_value);
;}
    break;

  case 36:
#line 389 "src/uppaalparser.y"
    {
  (yyval.com_op)=EQ;
;}
    break;

  case 37:
#line 393 "src/uppaalparser.y"
    {
  (yyval.com_op)=LE;
;}
    break;

  case 38:
#line 397 "src/uppaalparser.y"
    {
  (yyval.com_op)=GE;
;}
    break;

  case 39:
#line 401 "src/uppaalparser.y"
    {
  (yyval.com_op)=LT;
;}
    break;

  case 40:
#line 405 "src/uppaalparser.y"
    {
  (yyval.com_op)=GT;
;}
    break;

  case 41:
#line 409 "src/uppaalparser.y"
    {
  (yyval.com_op)=NE;
;}
    break;

  case 54:
#line 435 "src/uppaalparser.y"
    {
  (yyval.int_value)=current_data->getClockId((yyvsp[(1) - (1)].token_value)->symbol);
  delete (yyvsp[(1) - (1)].token_value);
;}
    break;

  case 55:
#line 441 "src/uppaalparser.y"
    {
  (yyval.int_value)=current_data->getGlobalCounterId(  (yyvsp[(1) - (1)].token_value)->symbol);
  delete (yyvsp[(1) - (1)].token_value);
;}
    break;

  case 56:
#line 448 "src/uppaalparser.y"
    {
  (yyval.int_value)=current_data->getFormalParameterId( (yyvsp[(1) - (1)].token_value)->symbol);
  delete (yyvsp[(1) - (1)].token_value);
;}
    break;

  case 57:
#line 453 "src/uppaalparser.y"
    {
  (yyval.int_value)=current_data->getFormalParameterId( (yyvsp[(1) - (1)].token_value)->symbol);
  delete (yyvsp[(1) - (1)].token_value);
;}
    break;

  case 58:
#line 460 "src/uppaalparser.y"
    {
  (yyval.int_value)=current_data->getGlobalChannelId( (yyvsp[(1) - (1)].token_value)->symbol );
  delete (yyvsp[(1) - (1)].token_value);
;}
    break;

  case 59:
#line 468 "src/uppaalparser.y"
    {
  current_data->addClockConstraint( (yyvsp[(1) - (3)].int_value), GLOBAL_CLOCK_ID, (yyvsp[(2) - (3)].com_op), (yyvsp[(3) - (3)].int_value));
;}
    break;

  case 60:
#line 473 "src/uppaalparser.y"
    {
  COMP_OPERATOR nop=negation( (yyvsp[(2) - (3)].com_op));
  current_data->addClockConstraint(  (yyvsp[(3) - (3)].int_value) , GLOBAL_CLOCK_ID, nop, (yyvsp[(1) - (3)].int_value));
;}
    break;

  case 61:
#line 479 "src/uppaalparser.y"
    {
  current_data->addClockConstraint(  (yyvsp[(1) - (3)].int_value), GLOBAL_CLOCK_ID, (yyvsp[(2) - (3)].com_op), 0, (yyvsp[(3) - (3)].int_value) ); 
;}
    break;

  case 62:
#line 484 "src/uppaalparser.y"
    {
  COMP_OPERATOR nop=negation( (yyvsp[(2) - (3)].com_op));
  current_data->addClockConstraint(  (yyvsp[(3) - (3)].int_value), GLOBAL_CLOCK_ID, nop, 0, (yyvsp[(1) - (3)].int_value) ); 
;}
    break;

  case 63:
#line 490 "src/uppaalparser.y"
    {
  Argument first( PARAMETER_ARG, (yyvsp[(1) - (3)].int_value));
  Argument second( EMPTY_ARG, 0);
  Argument rhs( CONST_ARG, (yyvsp[(3) - (3)].int_value));
  void*  cs=createConstraint( first, second, (yyvsp[(2) - (3)].com_op), rhs );
  current_data->addPointer( INT_CS_T,  cs );
;}
    break;

  case 64:
#line 499 "src/uppaalparser.y"
    {
  COMP_OPERATOR nop=negation( (yyvsp[(2) - (3)].com_op));
  Argument first( PARAMETER_ARG, (yyvsp[(3) - (3)].int_value));
  Argument second( EMPTY_ARG, 0);
  Argument rhs( CONST_ARG, (yyvsp[(1) - (3)].int_value));

  void*  cs=createConstraint( first, second, nop, rhs );
  current_data->addPointer( INT_CS_T, cs );
;}
    break;

  case 65:
#line 510 "src/uppaalparser.y"
    {
  Argument first( PARAMETER_ARG, (yyvsp[(1) - (3)].int_value));
  Argument second( PARAMETER_ARG, (yyvsp[(3) - (3)].int_value));
  Argument rhs( CONST_ARG, 0);
  
  void* cs = createConstraint( first, second, (yyvsp[(2) - (3)].com_op), rhs);
  current_data->addPointer( INT_CS_T,  cs );
;}
    break;

  case 66:
#line 520 "src/uppaalparser.y"
    {
  Argument first( PARAMETER_ARG, (yyvsp[(1) - (5)].int_value));
  Argument second( PARAMETER_ARG, (yyvsp[(3) - (5)].int_value));
  Argument rhs( CONST_ARG, (yyvsp[(5) - (5)].int_value));
  void* cs = createConstraint( first, second, (yyvsp[(4) - (5)].com_op), rhs);

  current_data->addPointer( INT_CS_T,  cs );
;}
    break;

  case 67:
#line 530 "src/uppaalparser.y"
    {

  Argument first( COUNTER_ARG, (yyvsp[(1) - (3)].int_value));
  Argument second( EMPTY_ARG, 0);
  Argument rhs( CONST_ARG, (yyvsp[(3) - (3)].int_value));
  void* cs = createConstraint( first, second, (yyvsp[(2) - (3)].com_op), rhs);
  current_data->addPointer( INT_CS_T,  cs );
;}
    break;

  case 68:
#line 540 "src/uppaalparser.y"
    {
  COMP_OPERATOR nop=negation( (yyvsp[(2) - (3)].com_op));
  
  Argument first( COUNTER_ARG, (yyvsp[(3) - (3)].int_value));
  Argument second( EMPTY_ARG, 0);
  Argument rhs( CONST_ARG, (yyvsp[(1) - (3)].int_value));
  void* cs = createConstraint( first, second, nop, rhs);
  
  current_data->addPointer( INT_CS_T,  cs );
;}
    break;

  case 69:
#line 552 "src/uppaalparser.y"
    {
  Argument first( COUNTER_ARG, (yyvsp[(1) - (3)].int_value));
  Argument second( PARAMETER_ARG, (yyvsp[(3) - (3)].int_value));
  Argument rhs( CONST_ARG, 0);
  void* cs = createConstraint( first, second, (yyvsp[(2) - (3)].com_op), rhs);

  current_data->addPointer( INT_CS_T, cs );
;}
    break;

  case 70:
#line 561 "src/uppaalparser.y"
    {
  COMP_OPERATOR nop=negation( (yyvsp[(2) - (3)].com_op));

  Argument first( COUNTER_ARG, (yyvsp[(3) - (3)].int_value));
  Argument second( PARAMETER_ARG, (yyvsp[(1) - (3)].int_value));
  Argument rhs( CONST_ARG, 0);
  void* cs = createConstraint( first, second, nop, rhs);

  current_data->addPointer(INT_CS_T , cs );
;}
    break;

  case 71:
#line 573 "src/uppaalparser.y"
    {
  current_data->addClockConstraint( (yyvsp[(1) - (5)].int_value), (yyvsp[(3) - (5)].int_value), (yyvsp[(4) - (5)].com_op), (yyvsp[(5) - (5)].int_value) );
;}
    break;

  case 72:
#line 578 "src/uppaalparser.y"
    {
   current_data->addClockConstraint( (yyvsp[(1) - (5)].int_value), (yyvsp[(3) - (5)].int_value), (yyvsp[(4) - (5)].com_op), 0, (yyvsp[(5) - (5)].int_value) );
;}
    break;

  case 73:
#line 583 "src/uppaalparser.y"
    {
  Argument first( COUNTER_ARG, (yyvsp[(1) - (5)].int_value));
  Argument second( COUNTER_ARG, (yyvsp[(3) - (5)].int_value));
  Argument rhs( CONST_ARG, (yyvsp[(5) - (5)].int_value));
  void* cs = createConstraint( first, second, (yyvsp[(4) - (5)].com_op), rhs);
  current_data->addPointer( INT_CS_T, cs );
;}
    break;

  case 74:
#line 592 "src/uppaalparser.y"
    {
  Argument first( COUNTER_ARG, (yyvsp[(1) - (5)].int_value));
  Argument second( COUNTER_ARG, (yyvsp[(3) - (5)].int_value));
  Argument rhs( PARAMETER_ARG, (yyvsp[(5) - (5)].int_value));
  void* cs = createConstraint( first, second, (yyvsp[(4) - (5)].com_op), rhs);

  current_data->addPointer( INT_CS_T,  cs );
;}
    break;

  case 75:
#line 601 "src/uppaalparser.y"
    {
  Argument first( COUNTER_ARG, (yyvsp[(1) - (1)].int_value));
  Argument second( EMPTY_ARG, 0);
  Argument rhs( CONST_ARG, 0);
  void* cs = createConstraint( first, second,NE, rhs);

  current_data->addPointer( INT_CS_T,  cs );
;}
    break;

  case 76:
#line 611 "src/uppaalparser.y"
    {
  Argument first( COUNTER_ARG, (yyvsp[(2) - (2)].int_value));
  Argument second( EMPTY_ARG, 0);
  Argument rhs( CONST_ARG, 0);
  void* cs = createConstraint( first, second, EQ, rhs);
  current_data->addPointer( INT_CS_T, cs );  
;}
    break;

  case 77:
#line 620 "src/uppaalparser.y"
    {
  int pid=current_data->getFormalParameterId( (yyvsp[(1) - (1)].token_value)->symbol);
  Argument first( PARAMETER_ARG, pid);
  Argument second( EMPTY_ARG, 0);
  Argument rhs( CONST_ARG, 0);
  void* cs = createConstraint( first, second,NE, rhs);

  current_data->addPointer( INT_CS_T, cs);
  delete (yyvsp[(1) - (1)].token_value);
;}
    break;

  case 78:
#line 632 "src/uppaalparser.y"
    {
  int pid=current_data->getFormalParameterId( (yyvsp[(1) - (1)].token_value)->symbol);
  Argument first( REF_PARAMETER_ARG, pid);
  Argument second( EMPTY_ARG, 0);
  Argument rhs( CONST_ARG, 0);
  void* cs = createConstraint( first, second, NE, rhs);
  current_data->addPointer( INT_CS_T,  cs );
  delete (yyvsp[(1) - (1)].token_value);
;}
    break;

  case 79:
#line 644 "src/uppaalparser.y"
    {
  int pid=current_data->getFormalParameterId( (yyvsp[(2) - (2)].token_value)->symbol);

  Argument first( PARAMETER_ARG, pid);
  Argument second( EMPTY_ARG, 0);
  Argument rhs( CONST_ARG, 0);
  void* cs = createConstraint( first, second, EQ, rhs);
  current_data->addPointer( INT_CS_T, cs);
  delete (yyvsp[(2) - (2)].token_value);

;}
    break;

  case 80:
#line 657 "src/uppaalparser.y"
    {
  
  int pid=current_data->getFormalParameterId( (yyvsp[(2) - (2)].token_value)->symbol);

  Argument first( REF_PARAMETER_ARG, pid);
  Argument second( EMPTY_ARG, 0);
  Argument rhs( CONST_ARG, 0);
  void* cs = createConstraint( first, second, EQ, rhs);
  current_data->addPointer( INT_CS_T,  cs );
  delete (yyvsp[(2) - (2)].token_value);
;}
    break;

  case 81:
#line 670 "src/uppaalparser.y"
    {
  int pid=current_data->getFormalParameterId( (yyvsp[(1) - (2)].token_value)->symbol);
  pid=fromPidToChanId( pid);
  current_data->addValue(REF_CHAN_ACTION_T, -pid );
  assert( pid>0);
  delete (yyvsp[(1) - (2)].token_value);
  //TODO:   receive signal
;}
    break;

  case 82:
#line 680 "src/uppaalparser.y"
    {
  int pid=current_data->getFormalParameterId( (yyvsp[(1) - (2)].token_value)->symbol);
  pid=fromPidToChanId( pid);
  current_data->addValue(REF_CHAN_ACTION_T, pid );
  delete (yyvsp[(1) - (2)].token_value);
  
  //TODO:  send signal
;}
    break;

  case 85:
#line 697 "src/uppaalparser.y"
    {
  pair<int, int> *reset=new pair<int, int>((yyvsp[(1) - (3)].int_value), (yyvsp[(3) - (3)].int_value) );
  current_data->addPointer(RESET_T, reset );
;}
    break;

  case 86:
#line 703 "src/uppaalparser.y"
    {
  Argument lhs(COUNTER_ARG, (yyvsp[(1) - (3)].int_value) );
  Argument rhs( CONST_ARG, (yyvsp[(3) - (3)].int_value));
  CounterAction   *action=new CounterAction(lhs, ASSIGNMENT_ACTION, rhs);
  current_data->addPointer( INT_UPDATE_T, action);
;}
    break;

  case 87:
#line 711 "src/uppaalparser.y"
    {
  Argument lhs(COUNTER_ARG, (yyvsp[(1) - (3)].int_value) );
  Argument rhs( COUNTER_ARG, (yyvsp[(3) - (3)].int_value));
  CounterAction *action =new CounterAction(lhs, ASSIGNMENT_ACTION, rhs);
  current_data->addPointer( INT_UPDATE_T, action);
;}
    break;

  case 88:
#line 719 "src/uppaalparser.y"
    {
  Argument lhs( COUNTER_ARG, (yyvsp[(1) - (3)].int_value));
  Argument rhs( PARAMETER_ARG, (yyvsp[(3) - (3)].int_value));
  CounterAction *action =new CounterAction(lhs, ASSIGNMENT_ACTION, rhs);
  current_data->addPointer( INT_UPDATE_T, action);
;}
    break;

  case 89:
#line 727 "src/uppaalparser.y"
    {
  Argument lhs( COUNTER_ARG, (yyvsp[(1) - (3)].int_value));
  Argument rhs( CONST_ARG, (yyvsp[(3) - (3)].int_value));
  CounterAction   *action=new CounterAction(lhs, SELF_INC_ACTION, rhs);
  current_data->addPointer( INT_UPDATE_T, action);
;}
    break;

  case 90:
#line 735 "src/uppaalparser.y"
    {
  Argument lhs( COUNTER_ARG, (yyvsp[(1) - (3)].int_value));
  Argument rhs( COUNTER_ARG, (yyvsp[(3) - (3)].int_value));
  CounterAction *action =new CounterAction(lhs, SELF_INC_ACTION, rhs);
  current_data->addPointer( INT_UPDATE_T, action);
;}
    break;

  case 91:
#line 743 "src/uppaalparser.y"
    {
  Argument lhs( COUNTER_ARG, (yyvsp[(1) - (3)].int_value));
  Argument rhs( PARAMETER_ARG, (yyvsp[(3) - (3)].int_value));
  CounterAction *action =new CounterAction(lhs, SELF_INC_ACTION, rhs);
  current_data->addPointer( INT_UPDATE_T, action);
;}
    break;

  case 92:
#line 752 "src/uppaalparser.y"
    {
  Argument lhs( COUNTER_ARG, (yyvsp[(1) - (3)].int_value));
  Argument rhs( CONST_ARG, (yyvsp[(3) - (3)].int_value));
  CounterAction   *action=new CounterAction(lhs, SELF_DEC_ACTION, rhs);
  current_data->addPointer( INT_UPDATE_T, action);
;}
    break;

  case 93:
#line 760 "src/uppaalparser.y"
    {
  Argument lhs( COUNTER_ARG, (yyvsp[(1) - (3)].int_value));
  Argument rhs( COUNTER_ARG, (yyvsp[(3) - (3)].int_value));
  CounterAction *action =new CounterAction(lhs, SELF_DEC_ACTION, rhs);
  current_data->addPointer( INT_UPDATE_T, action);
;}
    break;

  case 94:
#line 768 "src/uppaalparser.y"
    {
  Argument lhs( COUNTER_ARG, (yyvsp[(1) - (3)].int_value));
  Argument rhs( PARAMETER_ARG, (yyvsp[(3) - (3)].int_value));
  CounterAction *action =new CounterAction(lhs, SELF_DEC_ACTION, rhs);
  current_data->addPointer( INT_UPDATE_T, action);
;}
    break;

  case 95:
#line 777 "src/uppaalparser.y"
    {

  TaDec * ta=new TaDec( );
  ta->name= (yyvsp[(1) - (7)].token_value)->symbol;
  ta->tmt_name=(yyvsp[(3) - (7)].token_value)->symbol;
  ta->real_param_list.insert(ta->real_param_list.end( ), (yyvsp[(5) - (7)].item_vec_pointer)->begin( ), (yyvsp[(5) - (7)].item_vec_pointer)->end( ) );
  current_data->addValue(AUTOMATA_T, (yyvsp[(1) - (7)].token_value)->symbol,  ta);
  
  delete (yyvsp[(1) - (7)].token_value);
  delete (yyvsp[(3) - (7)].token_value);
  delete (yyvsp[(5) - (7)].item_vec_pointer);
;}
    break;

  case 96:
#line 790 "src/uppaalparser.y"
    {
  int pid=current_data->getFormalParameterId( (yyvsp[(1) - (3)].token_value)->symbol);
  Argument lhs( PARAMETER_ARG, pid);
  Argument rhs( CONST_ARG, (yyvsp[(3) - (3)].int_value));
  CounterAction *action=new CounterAction( lhs,  ASSIGNMENT_ACTION, rhs);
  current_data->addPointer( INT_UPDATE_T, action);
  delete (yyvsp[(1) - (3)].token_value);
  
;}
    break;

  case 97:
#line 800 "src/uppaalparser.y"
    {
  
  int pid=current_data->getFormalParameterId( (yyvsp[(1) - (3)].token_value)->symbol);
  Argument lhs( REF_PARAMETER_ARG, pid );
  Argument rhs( CONST_ARG, (yyvsp[(3) - (3)].int_value));
  CounterAction *action=new CounterAction( lhs,  ASSIGNMENT_ACTION, rhs);
  current_data->addPointer( INT_UPDATE_T, action);
  delete (yyvsp[(1) - (3)].token_value);

;}
    break;

  case 98:
#line 815 "src/uppaalparser.y"
    {
  current_data->addValue(CHAN_ACTION_T, (yyvsp[(1) - (2)].int_value) );
;}
    break;

  case 99:
#line 820 "src/uppaalparser.y"
    {
  current_data->addValue(CHAN_ACTION_T, -(yyvsp[(1) - (2)].int_value) );
;}
    break;

  case 100:
#line 827 "src/uppaalparser.y"
    {
  (yyval.int_value)=(yyvsp[(1) - (1)].int_value);
;}
    break;

  case 101:
#line 831 "src/uppaalparser.y"
    {
  (yyval.int_value)=1;
;}
    break;

  case 102:
#line 835 "src/uppaalparser.y"
    {
  (yyval.int_value)=0;
;}
    break;

  case 103:
#line 842 "src/uppaalparser.y"
    {
  TYPE_T type=baseType((yyvsp[(1) - (3)].type_value) );
  switch( type){
    case INT_T:{
      for( auto v: *(yyvsp[(2) - (3)].str_vec_pointer)){
        current_data->setValue(INT_T, v); //All the variables  in system_data
      }
      break ;
    }
    case CLOCK_T:{
      for( auto v: *(yyvsp[(2) - (3)].str_vec_pointer)){
        current_data->setValue(CLOCK_T, v);  //clock variable only can declare in template
      }
      break ;
    }
    case CHAN_T: {
      for( auto v: *(yyvsp[(2) - (3)].str_vec_pointer)){
        current_data->setValue(CHAN_T, v, ONE2ONE_CH);
      }
      break ;
    }

    case URGENT_CHAN_T:{
      for( auto v: *(yyvsp[(2) - (3)].str_vec_pointer)){
       current_data->setValue(CHAN_T, v, URGENT_CH);
      }
      break ;
    }
    case BROADCAST_CHAN_T:{
      for( auto v: *(yyvsp[(2) - (3)].str_vec_pointer)){
        current_data->setValue(CHAN_T, v, BROADCAST_CH);
      }
      break ;
    }
    default:{
      assert( false);
    }

  }
  delete (yyvsp[(2) - (3)].str_vec_pointer);
;}
    break;

  case 104:
#line 884 "src/uppaalparser.y"
    {
  string name= (yyvsp[(2) - (6)].token_value)->symbol;
  TYPE_T type=baseType((yyvsp[(1) - (6)].type_value) );
  
  switch( type){
    case INT_T:{
      current_data->setValue( INT_T,  name);
      for( int i=0; i< (yyvsp[(4) - (6)].int_value); i++){
        string temp_str=arrayToVar(name, i);
        current_data->setValue( INT_T,  arrayToVar(temp_str, i));
      }
      break;
    }
    case CLOCK_T: {
      current_data->setValue( CLOCK_T, name );
      for( int i=0; i< (yyvsp[(4) - (6)].int_value); i++){
        current_data->setValue( CLOCK_T, arrayToVar(name, i ));
      }
      break;
    }     
    case CHAN_T:{
      current_data->setValue( CHAN_T, name, ONE2ONE_CH);
      for( int i=0; i< (yyvsp[(4) - (6)].int_value); i++){
        string temp_str=arrayToVar(name, i);
        current_data->setValue( CHAN_T, temp_str, ONE2ONE_CH);
      }
      break;
    }
    case URGENT_CHAN_T:{

      current_data->setValue( CHAN_T, name, URGENT_CH);
      for( int i=0; i< (yyvsp[(4) - (6)].int_value); i++){
        string temp_str=arrayToVar(name, i);
        current_data->setValue( CHAN_T, temp_str, URGENT_CH);
      }
      break;
    }
    case BROADCAST_CHAN_T: {
      current_data->setValue( CHAN_T,name, BROADCAST_CH);
      for( int i=0; i< (yyvsp[(4) - (6)].int_value); i++){
        current_data->setValue( CHAN_T, arrayToVar(name, i ), BROADCAST_CH);
      }
      break;
    }
    default:
      assert( false);
  }
;}
    break;

  case 105:
#line 934 "src/uppaalparser.y"
    {
  TYPE_T type=baseType((yyvsp[(1) - (5)].type_value) );
  
  switch(type){
    case INT_T:
      current_data->setValue(INT_T, (yyvsp[(2) - (5)].token_value)->symbol, (yyvsp[(4) - (5)].int_value));
      break;
      
    case CLOCK_T:
      current_data->setValue(CLOCK_T,  (yyvsp[(2) - (5)].token_value)->symbol,  (yyvsp[(4) - (5)].int_value));
      break;
    default:
      assert( false);
  }
  
;}
    break;

  case 106:
#line 952 "src/uppaalparser.y"
    {

  vector<int> temp;
  for( int i=(yyvsp[(4) - (9)].int_value); i<=(yyvsp[(6) - (9)].int_value); i++  ){
    temp.push_back( i);
  }
  current_data->addIntArray((yyvsp[(8) - (9)].token_value)->symbol,temp);
;}
    break;

  case 107:
#line 963 "src/uppaalparser.y"
    {
  for(auto e: *((yyvsp[(2) - (2)].para_element_pointer))){
    
    TYPE_T type=get_formal_paramter_type(e.type );
    current_data->addValue(type, e.name  );
    
    current_data->addValue(FORMAL_PARAMETER_T , e.name, new FormalParameterItem(e));
  }
  delete (yyvsp[(2) - (2)].para_element_pointer);
;}
    break;

  case 108:
#line 977 "src/uppaalparser.y"
    {
  SystemDec * sys=new SystemDec( );
  for( size_t i=0; i< (yyvsp[(2) - (3)].str_vec_pointer)->size( ); i++){
    string name=(*(yyvsp[(2) - (3)].str_vec_pointer))[ i];
    
    if(!current_data->hasPointer(AUTOMATA_T, name  )){
      TaDec* temp=new TaDec( );
      temp->has_parameter=false;
      temp->name= name;
      temp->tmt_name=name;
      sys->timed_automata_list.push_back(temp );
    }else{
      void* ta_dec=current_data->getPointer(AUTOMATA_T, name );
      
      sys->timed_automata_list.push_back((TaDec*)ta_dec );
    }
  }
  
  current_data->clear(TEMPLATE_T );
  
  current_data->addPointer(SYSTEM_T,sys );
  delete (yyvsp[(2) - (3)].str_vec_pointer);
;}
    break;


/* Line 1267 of yacc.c.  */
#line 2718 "src/uppaalparser.cpp"
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


#line 1001 "src/uppaalparser.y"


#include "io/uppaalscan.h"
   
/* extern int column; */

void yyerror(const char *s)
{
  printf("\n jjj error  %s\n",s);
  fflush(stdout);
  assert( false);
}
void yyerror(const string &s){


  cout<<"error in: "<<endl;
  fflush(stdout);
  assert( false);
  
}


namespace graphsat{
  void parseProblem( const string &str,   UppaalData* d){
    
    current_data=d;
    yyin=tmpfile();
    fputs( str.c_str(), yyin);
    rewind(yyin);
    yyparse();
    fclose(yyin);
  
  }
} 



