/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "syntax.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "quad.h"
#include "optimizer.h"  // Include the optimizer header

int yylex();
int line = 1;
int column = 1;
void yyerror(const char *s);

// Global symbol table
SymbolTable* symtab;
// Global quadruple list
QuadList* quad_list;
// Current type being processed in declarations
SymbolType currentType;

// External declarations
extern char* yytext;
extern int tempCount;  // From quad.c

// Function to check format specifier compatibility
int isFormatCompatible(const char* format, SymbolType type) {
    if (strcmp(format, "$") == 0) return type == TYPE_INTEGER;
    if (strcmp(format, "%") == 0) return type == TYPE_FLOAT;
    if (strcmp(format, "#") == 0) return type == TYPE_STRING;
    if (strcmp(format, "&") == 0) return type == TYPE_CHAR;
    return 0;
}

#line 105 "syntax.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "syntax.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_mc_ident = 3,                   /* mc_ident  */
  YYSYMBOL_mc_data = 4,                    /* mc_data  */
  YYSYMBOL_mc_endp = 5,                    /* mc_endp  */
  YYSYMBOL_mc_code = 6,                    /* mc_code  */
  YYSYMBOL_mc_comment = 7,                 /* mc_comment  */
  YYSYMBOL_mc_const = 8,                   /* mc_const  */
  YYSYMBOL_mc_vector = 9,                  /* mc_vector  */
  YYSYMBOL_mc_integ = 10,                  /* mc_integ  */
  YYSYMBOL_mc_float = 11,                  /* mc_float  */
  YYSYMBOL_mc_char = 12,                   /* mc_char  */
  YYSYMBOL_mc_string = 13,                 /* mc_string  */
  YYSYMBOL_mc_deuxp = 14,                  /* mc_deuxp  */
  YYSYMBOL_mc_pvg = 15,                    /* mc_pvg  */
  YYSYMBOL_mc_affectation = 16,            /* mc_affectation  */
  YYSYMBOL_mc_crocheto = 17,               /* mc_crocheto  */
  YYSYMBOL_mc_crochetf = 18,               /* mc_crochetf  */
  YYSYMBOL_mc_vg = 19,                     /* mc_vg  */
  YYSYMBOL_mc_separateur = 20,             /* mc_separateur  */
  YYSYMBOL_mc_paro = 21,                   /* mc_paro  */
  YYSYMBOL_mc_parf = 22,                   /* mc_parf  */
  YYSYMBOL_mc_plus = 23,                   /* mc_plus  */
  YYSYMBOL_mc_moins = 24,                  /* mc_moins  */
  YYSYMBOL_mc_mult = 25,                   /* mc_mult  */
  YYSYMBOL_mc_div = 26,                    /* mc_div  */
  YYSYMBOL_mc_read = 27,                   /* mc_read  */
  YYSYMBOL_mc_display = 28,                /* mc_display  */
  YYSYMBOL_mc_if = 29,                     /* mc_if  */
  YYSYMBOL_mc_else = 30,                   /* mc_else  */
  YYSYMBOL_mc_for = 31,                    /* mc_for  */
  YYSYMBOL_mc_sup = 32,                    /* mc_sup  */
  YYSYMBOL_mc_inf = 33,                    /* mc_inf  */
  YYSYMBOL_mc_supeg = 34,                  /* mc_supeg  */
  YYSYMBOL_mc_infeg = 35,                  /* mc_infeg  */
  YYSYMBOL_mc_egal = 36,                   /* mc_egal  */
  YYSYMBOL_mc_diff = 37,                   /* mc_diff  */
  YYSYMBOL_mc_adresse = 38,                /* mc_adresse  */
  YYSYMBOL_INT_CONST = 39,                 /* INT_CONST  */
  YYSYMBOL_mc_charactere = 40,             /* mc_charactere  */
  YYSYMBOL_mc_chaine = 41,                 /* mc_chaine  */
  YYSYMBOL_mc_cst = 42,                    /* mc_cst  */
  YYSYMBOL_mc_or = 43,                     /* mc_or  */
  YYSYMBOL_mc_and = 44,                    /* mc_and  */
  YYSYMBOL_mc_start = 45,                  /* mc_start  */
  YYSYMBOL_YYACCEPT = 46,                  /* $accept  */
  YYSYMBOL_program = 47,                   /* program  */
  YYSYMBOL_comment_list = 48,              /* comment_list  */
  YYSYMBOL_DATA_SECTION = 49,              /* DATA_SECTION  */
  YYSYMBOL_DECLARATIONS = 50,              /* DECLARATIONS  */
  YYSYMBOL_DEC = 51,                       /* DEC  */
  YYSYMBOL_VAR_TYPE = 52,                  /* VAR_TYPE  */
  YYSYMBOL_LISTE_VAR = 53,                 /* LISTE_VAR  */
  YYSYMBOL_DEC_CONST = 54,                 /* DEC_CONST  */
  YYSYMBOL_DEC_VECTOR = 55,                /* DEC_VECTOR  */
  YYSYMBOL_VALEURS = 56,                   /* VALEURS  */
  YYSYMBOL_CODE_SECTION = 57,              /* CODE_SECTION  */
  YYSYMBOL_INSTRUCTIONS = 58,              /* INSTRUCTIONS  */
  YYSYMBOL_INSTRUCTION = 59,               /* INSTRUCTION  */
  YYSYMBOL_INSTRUCTION_AFFECTATION = 60,   /* INSTRUCTION_AFFECTATION  */
  YYSYMBOL_EXPRESSION = 61,                /* EXPRESSION  */
  YYSYMBOL_INSTRUCTION_LIRE = 62,          /* INSTRUCTION_LIRE  */
  YYSYMBOL_INSTRUCTION_ECRIRE = 63,        /* INSTRUCTION_ECRIRE  */
  YYSYMBOL_INSTRUCTION_CONDITION = 64,     /* INSTRUCTION_CONDITION  */
  YYSYMBOL_STRUCTURE_CONDITIONELLE = 65,   /* STRUCTURE_CONDITIONELLE  */
  YYSYMBOL_CONDITION = 66,                 /* CONDITION  */
  YYSYMBOL_INSTRUCTION_BOUCLE = 67         /* INSTRUCTION_BOUCLE  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   177

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  57
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  136

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   300


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      45
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    67,    67,    75,    77,    81,    84,    86,    90,    91,
      92,    96,    97,    98,    99,   103,   108,   116,   149,   165,
     170,   175,   180,   188,   191,   193,   197,   198,   199,   200,
     201,   202,   206,   271,   307,   325,   343,   368,   378,   388,
     398,   408,   424,   440,   468,   496,   499,   548,   553,   558,
     563,   571,   590,   604,   608,   616,   636,   649
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "mc_ident", "mc_data",
  "mc_endp", "mc_code", "mc_comment", "mc_const", "mc_vector", "mc_integ",
  "mc_float", "mc_char", "mc_string", "mc_deuxp", "mc_pvg",
  "mc_affectation", "mc_crocheto", "mc_crochetf", "mc_vg", "mc_separateur",
  "mc_paro", "mc_parf", "mc_plus", "mc_moins", "mc_mult", "mc_div",
  "mc_read", "mc_display", "mc_if", "mc_else", "mc_for", "mc_sup",
  "mc_inf", "mc_supeg", "mc_infeg", "mc_egal", "mc_diff", "mc_adresse",
  "INT_CONST", "mc_charactere", "mc_chaine", "mc_cst", "mc_or", "mc_and",
  "mc_start", "$accept", "program", "comment_list", "DATA_SECTION",
  "DECLARATIONS", "DEC", "VAR_TYPE", "LISTE_VAR", "DEC_CONST",
  "DEC_VECTOR", "VALEURS", "CODE_SECTION", "INSTRUCTIONS", "INSTRUCTION",
  "INSTRUCTION_AFFECTATION", "EXPRESSION", "INSTRUCTION_LIRE",
  "INSTRUCTION_ECRIRE", "INSTRUCTION_CONDITION", "STRUCTURE_CONDITIONELLE",
  "CONDITION", "INSTRUCTION_BOUCLE", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-110)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -110,     2,    -2,  -110,  -110,  -110,    56,  -110,  -110,     5,
      48,  -110,  -110,  -110,   140,  -110,  -110,  -110,    44,    51,
    -110,  -110,  -110,  -110,    53,  -110,  -110,    49,  -110,    65,
      67,    76,    68,  -110,  -110,    70,    71,    72,    73,  -110,
    -110,  -110,  -110,  -110,  -110,    90,    78,    89,    95,    -3,
      80,    83,    -3,   122,   -32,    97,    76,  -110,  -110,    -3,
    -110,  -110,  -110,  -110,    -9,   123,   124,    79,   117,   126,
    -110,  -110,  -110,  -110,   131,   135,  -110,    64,  -110,    -3,
      -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      -3,   118,   152,   143,   119,  -110,   120,  -110,    -4,    -4,
    -110,  -110,     7,     7,     7,     7,   109,   109,    94,    94,
     157,   139,  -110,   148,   149,   142,   150,    -1,   163,    62,
     153,  -110,  -110,   155,    54,   145,   154,  -110,  -110,  -110,
     156,   165,   168,  -110,  -110,  -110
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     1,     3,     4,     0,     3,     6,     0,
       0,     3,    24,     2,     0,     7,     3,     5,     0,     0,
      11,    12,    13,    14,     0,     9,    10,     0,    25,     0,
       0,     0,     0,    23,    31,     0,     0,     0,     0,    26,
      27,    28,    29,    53,    30,     0,     0,    15,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     8,    46,     0,
      47,    48,    49,    50,     0,     0,     0,    56,     0,     0,
      21,    19,    20,    22,     0,     0,    16,     0,    32,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    17,     0,    45,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    44,    43,
       0,     0,    24,     0,     0,     0,     0,     3,     0,     0,
       0,    52,    54,     0,     0,     0,     0,    51,    24,     3,
       0,     3,     0,    18,    55,    57
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -110,  -110,    55,  -110,  -110,  -110,    57,   121,  -110,  -110,
    -110,  -110,  -109,    45,  -110,   -39,  -110,  -110,  -110,  -110,
    -110,  -110
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,   124,     7,    11,    15,    24,    48,    25,    26,
      74,    10,    16,    28,    39,    64,    40,    41,    42,    43,
      68,    44
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      58,     4,     3,   117,   122,     5,    78,    70,    71,    72,
      73,    12,     5,    67,    79,    80,    81,    82,    59,   131,
      77,    81,    82,    83,    84,    85,    86,    87,    88,   123,
      79,    80,    81,    82,    89,    90,    60,    61,    62,    63,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,    32,    13,    33,     2,     5,    32,    29,     6,
       8,     5,     9,     5,    34,    30,    14,    31,    45,    34,
      46,    27,    20,    21,    22,    23,    35,    36,    37,    47,
      38,    35,    36,    37,    49,    38,    97,    79,    80,    81,
      82,    50,    51,    52,    53,    55,    83,    84,    85,    86,
      87,    88,    79,    80,    81,    82,    54,    89,    90,    56,
      57,    83,    84,    85,    86,    87,    88,    79,    80,    81,
      82,    65,    89,    90,    66,    69,    83,    84,    85,    86,
      87,    88,    79,    80,    81,    82,    75,    91,    92,    93,
      94,    83,    84,    85,    86,    17,    95,     5,    18,    19,
      20,    21,    22,    23,    96,   111,   110,   112,   113,   114,
     115,   116,   118,   119,   120,   121,   125,   129,   127,   128,
     134,   133,   130,   135,   132,     0,   126,    76
};

static const yytype_int16 yycheck[] =
{
       3,     3,     0,   112,     5,     7,    15,    39,    40,    41,
      42,     6,     7,    52,    23,    24,    25,    26,    21,   128,
      59,    25,    26,    32,    33,    34,    35,    36,    37,    30,
      23,    24,    25,    26,    43,    44,    39,    40,    41,    42,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,     3,     5,     5,     0,     7,     3,    14,     4,
       4,     7,     7,     7,    15,    14,    11,    14,     3,    15,
       3,    16,    10,    11,    12,    13,    27,    28,    29,     3,
      31,    27,    28,    29,    16,    31,    22,    23,    24,    25,
      26,    21,    21,    21,    21,    17,    32,    33,    34,    35,
      36,    37,    23,    24,    25,    26,    16,    43,    44,    20,
      15,    32,    33,    34,    35,    36,    37,    23,    24,    25,
      26,    41,    43,    44,    41,     3,    32,    33,    34,    35,
      36,    37,    23,    24,    25,    26,    39,    14,    14,    22,
      14,    32,    33,    34,    35,     5,    15,     7,     8,     9,
      10,    11,    12,    13,    19,     3,    38,    14,    39,    39,
       3,    22,    14,    14,    22,    15,     3,    22,    15,    14,
       5,    15,    18,     5,   129,    -1,   119,    56
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    47,    48,     0,     3,     7,    48,    49,     4,    48,
      57,    50,     6,     5,    48,    51,    58,     5,     8,     9,
      10,    11,    12,    13,    52,    54,    55,    48,    59,    14,
      14,    14,     3,     5,    15,    27,    28,    29,    31,    60,
      62,    63,    64,    65,    67,     3,     3,     3,    53,    16,
      21,    21,    21,    21,    16,    17,    20,    15,     3,    21,
      39,    40,    41,    42,    61,    41,    41,    61,    66,     3,
      39,    40,    41,    42,    56,    39,    53,    61,    15,    23,
      24,    25,    26,    32,    33,    34,    35,    36,    37,    43,
      44,    14,    14,    22,    14,    15,    19,    22,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      38,     3,    14,    39,    39,     3,    22,    58,    14,    14,
      22,    15,     5,    30,    48,     3,    52,    15,    14,    22,
      18,    58,    59,    15,     5,     5
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    46,    47,    48,    48,    49,    50,    50,    51,    51,
      51,    52,    52,    52,    52,    53,    53,    54,    55,    56,
      56,    56,    56,    57,    58,    58,    59,    59,    59,    59,
      59,    59,    60,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    62,    63,    64,    65,    65,    66,    67
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     0,     2,     5,     0,     2,     5,     2,
       2,     1,     1,     1,     1,     1,     3,     6,    11,     1,
       1,     1,     1,     5,     0,     2,     2,     2,     2,     2,
       2,     2,     4,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     8,     7,     1,     7,    10,     1,    10
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
  case 2: /* program: comment_list mc_ident DATA_SECTION CODE_SECTION mc_endp  */
#line 67 "syntax.y"
                                                            {
        printf("Compilation successful!\n");
        printSymbolTable(symtab);
        printQuadList(quad_list);
        YYACCEPT;
    }
#line 1259 "syntax.c"
    break;

  case 11: /* VAR_TYPE: mc_integ  */
#line 96 "syntax.y"
             { currentType = TYPE_INTEGER; }
#line 1265 "syntax.c"
    break;

  case 12: /* VAR_TYPE: mc_float  */
#line 97 "syntax.y"
               { currentType = TYPE_FLOAT; }
#line 1271 "syntax.c"
    break;

  case 13: /* VAR_TYPE: mc_char  */
#line 98 "syntax.y"
              { currentType = TYPE_CHAR; }
#line 1277 "syntax.c"
    break;

  case 14: /* VAR_TYPE: mc_string  */
#line 99 "syntax.y"
                { currentType = TYPE_STRING; }
#line 1283 "syntax.c"
    break;

  case 15: /* LISTE_VAR: mc_ident  */
#line 103 "syntax.y"
             {
        if (!insertSymbol(symtab, (yyvsp[0].str_val), currentType, line, column)) {
            // Error already reported by insertSymbol
        }
    }
#line 1293 "syntax.c"
    break;

  case 16: /* LISTE_VAR: mc_ident mc_separateur LISTE_VAR  */
#line 108 "syntax.y"
                                       {
        if (!insertSymbol(symtab, (yyvsp[-2].str_val), currentType, line, column)) {
            // Error already reported by insertSymbol
        }
    }
#line 1303 "syntax.c"
    break;

  case 17: /* DEC_CONST: mc_const mc_deuxp mc_ident mc_affectation VALEURS mc_pvg  */
#line 116 "syntax.y"
                                                             {
        if (!insertSymbol(symtab, (yyvsp[-3].str_val), (yyvsp[-1].expr_val).type, line, column)) {
            // Error already reported by insertSymbol
        } else {
            // Mark as constant and initialized
            SymbolEntry* entry = lookupSymbol(symtab, (yyvsp[-3].str_val));
            if (entry) {
                entry->isConstant = 1;
                entry->isInitialized = 1;
                
                // Set value based on type
                switch (entry->type) {
                    case TYPE_INTEGER:
                        entry->value.intValue = (yyvsp[-1].expr_val).value.intValue;
                        break;
                    case TYPE_FLOAT:
                        entry->value.floatValue = (yyvsp[-1].expr_val).value.floatValue;
                        break;
                    case TYPE_CHAR:
                        entry->value.charValue = (yyvsp[-1].expr_val).value.charValue;
                        break;
                    case TYPE_STRING:
                        entry->value.stringValue = strdup((yyvsp[-1].expr_val).value.stringValue);
                        break;
                    default:
                        break;
                }
            }
        }
    }
#line 1338 "syntax.c"
    break;

  case 18: /* DEC_VECTOR: mc_vector mc_deuxp mc_ident mc_crocheto INT_CONST mc_vg INT_CONST mc_deuxp VAR_TYPE mc_crochetf mc_pvg  */
#line 149 "syntax.y"
                                                                                                           {
        if (!insertSymbol(symtab, (yyvsp[-8].str_val), TYPE_VECTOR, line, column)) {
            // Error already reported by insertSymbol
        } else {
            // Set vector dimensions and element type
            SymbolEntry* entry = lookupSymbol(symtab, (yyvsp[-8].str_val));
            if (entry) {
                entry->value.vectorInfo.rows = (yyvsp[-6].int_val);
                entry->value.vectorInfo.cols = (yyvsp[-4].int_val);
                entry->value.vectorInfo.elementType = currentType;
            }
        }
    }
#line 1356 "syntax.c"
    break;

  case 19: /* VALEURS: mc_charactere  */
#line 165 "syntax.y"
                  {
        (yyval.expr_val).type = TYPE_CHAR;
        (yyval.expr_val).value.charValue = (yyvsp[0].char_val);
        sprintf((yyval.expr_val).valueStr, "'%c'", (yyvsp[0].char_val));
    }
#line 1366 "syntax.c"
    break;

  case 20: /* VALEURS: mc_chaine  */
#line 170 "syntax.y"
                {
        (yyval.expr_val).type = TYPE_STRING;
        (yyval.expr_val).value.stringValue = (yyvsp[0].str_val);
        sprintf((yyval.expr_val).valueStr, "%s", (yyvsp[0].str_val));
    }
#line 1376 "syntax.c"
    break;

  case 21: /* VALEURS: INT_CONST  */
#line 175 "syntax.y"
                {
        (yyval.expr_val).type = TYPE_INTEGER;
        (yyval.expr_val).value.intValue = (yyvsp[0].int_val);
        sprintf((yyval.expr_val).valueStr, "%d", (yyvsp[0].int_val));
    }
#line 1386 "syntax.c"
    break;

  case 22: /* VALEURS: mc_cst  */
#line 180 "syntax.y"
             {
        (yyval.expr_val).type = TYPE_INTEGER;
        (yyval.expr_val).value.intValue = (yyvsp[0].int_val);
        sprintf((yyval.expr_val).valueStr, "%d", (yyvsp[0].int_val));
    }
#line 1396 "syntax.c"
    break;

  case 32: /* INSTRUCTION_AFFECTATION: mc_ident mc_affectation EXPRESSION mc_pvg  */
#line 206 "syntax.y"
                                              {
        SymbolEntry* entry = lookupSymbol(symtab, (yyvsp[-3].str_val));
        if (!entry) {
            fprintf(stderr, " ------------SEMANTIC Error: Undefined variable '%s' at line %d ------------\n", (yyvsp[-3].str_val), line);
        } else {
            entry->isInitialized = 1;
            
            // Check type compatibility
            if (!areTypesCompatible(entry->type, (yyvsp[-1].expr_val).type, '=')) {
                fprintf(stderr, "-------------SEMANTIC Error: Type mismatch in assignment to '%s' at line %d -------------\n", (yyvsp[-3].str_val), line);
            } else {
                // Set the value based on types
                switch (entry->type) {
                    case TYPE_INTEGER:
                        if ((yyvsp[-1].expr_val).type == TYPE_FLOAT) {
                            entry->value.intValue = (int)(yyvsp[-1].expr_val).value.floatValue;
                        } else if ((yyvsp[-1].expr_val).type == TYPE_CHAR) {
                            entry->value.intValue = (int)(yyvsp[-1].expr_val).value.charValue;
                        } else {
                            entry->value.intValue = (yyvsp[-1].expr_val).value.intValue;
                        }
                        // Generate quadruple for assignment
                        addQuad(quad_list, QUAD_ASSIGN, (yyvsp[-1].expr_val).valueStr, NULL, (yyvsp[-3].str_val));
                        break;
                    case TYPE_FLOAT:
                        if ((yyvsp[-1].expr_val).type == TYPE_INTEGER) {
                            entry->value.floatValue = (float)(yyvsp[-1].expr_val).value.intValue;
                        } else if ((yyvsp[-1].expr_val).type == TYPE_CHAR) {
                            entry->value.floatValue = (float)(yyvsp[-1].expr_val).value.charValue;
                        } else {
                            entry->value.floatValue = (yyvsp[-1].expr_val).value.floatValue;
                        }
                        // Generate quadruple for assignment
                        addQuad(quad_list, QUAD_ASSIGN, (yyvsp[-1].expr_val).valueStr, NULL, (yyvsp[-3].str_val));
                        break;
                    case TYPE_CHAR:
                        if ((yyvsp[-1].expr_val).type == TYPE_INTEGER) {
                            entry->value.charValue = (char)(yyvsp[-1].expr_val).value.intValue;
                        } else {
                            entry->value.charValue = (yyvsp[-1].expr_val).value.charValue;
                        }
                        // Generate quadruple for assignment
                        addQuad(quad_list, QUAD_ASSIGN, (yyvsp[-1].expr_val).valueStr, NULL, (yyvsp[-3].str_val));
                        break;
                    case TYPE_STRING:
                        if ((yyvsp[-1].expr_val).type == TYPE_STRING) {
                            if (entry->value.stringValue) {
                                free(entry->value.stringValue);
                            }
                            entry->value.stringValue = strdup((yyvsp[-1].expr_val).value.stringValue);
                        } else {
                            fprintf(stderr, "------------SEMANTIC Error: Cannot assign non-string to string at line %d ------------\n", line);
                        }
                        // Generate quadruple for assignment
                        addQuad(quad_list, QUAD_ASSIGN, (yyvsp[-1].expr_val).valueStr, NULL, (yyvsp[-3].str_val));
                        break;
                    default:
                        break;
                }
            }
        }
    }
#line 1463 "syntax.c"
    break;

  case 33: /* EXPRESSION: EXPRESSION mc_plus EXPRESSION  */
#line 271 "syntax.y"
                                  {
        if ((yyvsp[-2].expr_val).type == TYPE_STRING || (yyvsp[0].expr_val).type == TYPE_STRING) {
            // String concatenation
            (yyval.expr_val).type = TYPE_STRING;
            int len = strlen((yyvsp[-2].expr_val).value.stringValue) + strlen((yyvsp[0].expr_val).value.stringValue) + 1;
            (yyval.expr_val).value.stringValue = malloc(len);
            strcpy((yyval.expr_val).value.stringValue, (yyvsp[-2].expr_val).value.stringValue);
            strcat((yyval.expr_val).value.stringValue, (yyvsp[0].expr_val).value.stringValue);
            sprintf((yyval.expr_val).valueStr, "%s", (yyval.expr_val).value.stringValue);
            
            // Generate quadruple
            char* temp = genBinaryOp(quad_list, QUAD_ADD, (yyvsp[-2].expr_val).valueStr, (yyvsp[0].expr_val).valueStr);
            strcpy((yyval.expr_val).valueStr, temp);
            free(temp);
        } else if ((yyvsp[-2].expr_val).type == TYPE_FLOAT || (yyvsp[0].expr_val).type == TYPE_FLOAT) {
            // Float addition
            (yyval.expr_val).type = TYPE_FLOAT;
            (yyval.expr_val).value.floatValue = convertToFloat((yyvsp[-2].expr_val)) + convertToFloat((yyvsp[0].expr_val));
            sprintf((yyval.expr_val).valueStr, "%f", (yyval.expr_val).value.floatValue);
            
            // Generate quadruple
            char* temp = genBinaryOp(quad_list, QUAD_ADD, (yyvsp[-2].expr_val).valueStr, (yyvsp[0].expr_val).valueStr);
            strcpy((yyval.expr_val).valueStr, temp);
            free(temp);
        } else {
            // Integer addition
            (yyval.expr_val).type = TYPE_INTEGER;
            (yyval.expr_val).value.intValue = convertToInt((yyvsp[-2].expr_val)) + convertToInt((yyvsp[0].expr_val));
            sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
            
            // Generate quadruple
            char* temp = genBinaryOp(quad_list, QUAD_ADD, (yyvsp[-2].expr_val).valueStr, (yyvsp[0].expr_val).valueStr);
            strcpy((yyval.expr_val).valueStr, temp);
            free(temp);
        }
    }
#line 1504 "syntax.c"
    break;

  case 34: /* EXPRESSION: EXPRESSION mc_moins EXPRESSION  */
#line 307 "syntax.y"
                                     {
        if ((yyvsp[-2].expr_val).type == TYPE_FLOAT || (yyvsp[0].expr_val).type == TYPE_FLOAT) {
            // Float subtraction
            (yyval.expr_val).type = TYPE_FLOAT;
            (yyval.expr_val).value.floatValue = convertToFloat((yyvsp[-2].expr_val)) - convertToFloat((yyvsp[0].expr_val));
            sprintf((yyval.expr_val).valueStr, "%f", (yyval.expr_val).value.floatValue);
        } else {
            // Integer subtraction
            (yyval.expr_val).type = TYPE_INTEGER;
            (yyval.expr_val).value.intValue = convertToInt((yyvsp[-2].expr_val)) - convertToInt((yyvsp[0].expr_val));
            sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
        }
        
        // Generate quadruple
        char* temp = genBinaryOp(quad_list, QUAD_SUB, (yyvsp[-2].expr_val).valueStr, (yyvsp[0].expr_val).valueStr);
        strcpy((yyval.expr_val).valueStr, temp);
        free(temp);
    }
#line 1527 "syntax.c"
    break;

  case 35: /* EXPRESSION: EXPRESSION mc_mult EXPRESSION  */
#line 325 "syntax.y"
                                    {
        if ((yyvsp[-2].expr_val).type == TYPE_FLOAT || (yyvsp[0].expr_val).type == TYPE_FLOAT) {
            // Float multiplication
            (yyval.expr_val).type = TYPE_FLOAT;
            (yyval.expr_val).value.floatValue = convertToFloat((yyvsp[-2].expr_val)) * convertToFloat((yyvsp[0].expr_val));
            sprintf((yyval.expr_val).valueStr, "%f", (yyval.expr_val).value.floatValue);
        } else {
            // Integer multiplication
            (yyval.expr_val).type = TYPE_INTEGER;
            (yyval.expr_val).value.intValue = convertToInt((yyvsp[-2].expr_val)) * convertToInt((yyvsp[0].expr_val));
            sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
        }
        
        // Generate quadruple
        char* temp = genBinaryOp(quad_list, QUAD_MULT, (yyvsp[-2].expr_val).valueStr, (yyvsp[0].expr_val).valueStr);
        strcpy((yyval.expr_val).valueStr, temp);
        free(temp);
    }
#line 1550 "syntax.c"
    break;

  case 36: /* EXPRESSION: EXPRESSION mc_div EXPRESSION  */
#line 343 "syntax.y"
                                   {
        // Check for division by zero
        if (((yyvsp[0].expr_val).type == TYPE_INTEGER && (yyvsp[0].expr_val).value.intValue == 0) ||
            ((yyvsp[0].expr_val).type == TYPE_FLOAT && (yyvsp[0].expr_val).value.floatValue == 0.0)) {
            fprintf(stderr, "------------ SEMANTIC Error: Division by zero at line %d ------------\n", line);
            (yyval.expr_val).type = TYPE_INTEGER;
            (yyval.expr_val).value.intValue = 0;
            sprintf((yyval.expr_val).valueStr, "0");
        } else if ((yyvsp[-2].expr_val).type == TYPE_FLOAT || (yyvsp[0].expr_val).type == TYPE_FLOAT) {
            // Float division
            (yyval.expr_val).type = TYPE_FLOAT;
            (yyval.expr_val).value.floatValue = convertToFloat((yyvsp[-2].expr_val)) / convertToFloat((yyvsp[0].expr_val));
            sprintf((yyval.expr_val).valueStr, "%f", (yyval.expr_val).value.floatValue);
        } else {
            // Integer division
            (yyval.expr_val).type = TYPE_INTEGER;
            (yyval.expr_val).value.intValue = convertToInt((yyvsp[-2].expr_val)) / convertToInt((yyvsp[0].expr_val));
            sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
        }
        
        // Generate quadruple
        char* temp = genBinaryOp(quad_list, QUAD_DIV, (yyvsp[-2].expr_val).valueStr, (yyvsp[0].expr_val).valueStr);
        strcpy((yyval.expr_val).valueStr, temp);
        free(temp);
    }
#line 1580 "syntax.c"
    break;

  case 37: /* EXPRESSION: EXPRESSION mc_sup EXPRESSION  */
#line 368 "syntax.y"
                                   {
        (yyval.expr_val).type = TYPE_INTEGER;  // Boolean result (0 or 1)
        (yyval.expr_val).value.intValue = (convertToFloat((yyvsp[-2].expr_val)) > convertToFloat((yyvsp[0].expr_val))) ? 1 : 0;
        sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
        
        // Generate comparison quadruple
        char* temp = genBinaryOp(quad_list, QUAD_IF_GT, (yyvsp[-2].expr_val).valueStr, (yyvsp[0].expr_val).valueStr);
        strcpy((yyval.expr_val).valueStr, temp);
        free(temp);
    }
#line 1595 "syntax.c"
    break;

  case 38: /* EXPRESSION: EXPRESSION mc_inf EXPRESSION  */
#line 378 "syntax.y"
                                   {
        (yyval.expr_val).type = TYPE_INTEGER;  // Boolean result (0 or 1)
        (yyval.expr_val).value.intValue = (convertToFloat((yyvsp[-2].expr_val)) < convertToFloat((yyvsp[0].expr_val))) ? 1 : 0;
        sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
        
        // Generate comparison quadruple
        char* temp = genBinaryOp(quad_list, QUAD_IF_LT, (yyvsp[-2].expr_val).valueStr, (yyvsp[0].expr_val).valueStr);
        strcpy((yyval.expr_val).valueStr, temp);
        free(temp);
    }
#line 1610 "syntax.c"
    break;

  case 39: /* EXPRESSION: EXPRESSION mc_supeg EXPRESSION  */
#line 388 "syntax.y"
                                     {
        (yyval.expr_val).type = TYPE_INTEGER;  // Boolean result (0 or 1)
        (yyval.expr_val).value.intValue = (convertToFloat((yyvsp[-2].expr_val)) >= convertToFloat((yyvsp[0].expr_val))) ? 1 : 0;
        sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
        
        // Generate comparison quadruple
        char* temp = genBinaryOp(quad_list, QUAD_IF_GE, (yyvsp[-2].expr_val).valueStr, (yyvsp[0].expr_val).valueStr);
        strcpy((yyval.expr_val).valueStr, temp);
        free(temp);
    }
#line 1625 "syntax.c"
    break;

  case 40: /* EXPRESSION: EXPRESSION mc_infeg EXPRESSION  */
#line 398 "syntax.y"
                                     {
        (yyval.expr_val).type = TYPE_INTEGER;  // Boolean result (0 or 1)
        (yyval.expr_val).value.intValue = (convertToFloat((yyvsp[-2].expr_val)) <= convertToFloat((yyvsp[0].expr_val))) ? 1 : 0;
        sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
        
        // Generate comparison quadruple
        char* temp = genBinaryOp(quad_list, QUAD_IF_LE, (yyvsp[-2].expr_val).valueStr, (yyvsp[0].expr_val).valueStr);
        strcpy((yyval.expr_val).valueStr, temp);
        free(temp);
    }
#line 1640 "syntax.c"
    break;

  case 41: /* EXPRESSION: EXPRESSION mc_egal EXPRESSION  */
#line 408 "syntax.y"
                                    {
        (yyval.expr_val).type = TYPE_INTEGER;  // Boolean result (0 or 1)
        
        if ((yyvsp[-2].expr_val).type == TYPE_STRING && (yyvsp[0].expr_val).type == TYPE_STRING) {
            (yyval.expr_val).value.intValue = (strcmp((yyvsp[-2].expr_val).value.stringValue, (yyvsp[0].expr_val).value.stringValue) == 0) ? 1 : 0;
        } else {
            (yyval.expr_val).value.intValue = (convertToFloat((yyvsp[-2].expr_val)) == convertToFloat((yyvsp[0].expr_val))) ? 1 : 0;
        }
        
        sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
        
        // Generate comparison quadruple
        char* temp = genBinaryOp(quad_list, QUAD_IF_EQ, (yyvsp[-2].expr_val).valueStr, (yyvsp[0].expr_val).valueStr);
        strcpy((yyval.expr_val).valueStr, temp);
        free(temp);
    }
#line 1661 "syntax.c"
    break;

  case 42: /* EXPRESSION: EXPRESSION mc_diff EXPRESSION  */
#line 424 "syntax.y"
                                    {
        (yyval.expr_val).type = TYPE_INTEGER;  // Boolean result (0 or 1)
        
        if ((yyvsp[-2].expr_val).type == TYPE_STRING && (yyvsp[0].expr_val).type == TYPE_STRING) {
            (yyval.expr_val).value.intValue = (strcmp((yyvsp[-2].expr_val).value.stringValue, (yyvsp[0].expr_val).value.stringValue) != 0) ? 1 : 0;
        } else {
            (yyval.expr_val).value.intValue = (convertToFloat((yyvsp[-2].expr_val)) != convertToFloat((yyvsp[0].expr_val))) ? 1 : 0;
        }
        
        sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
        
        // Generate comparison quadruple
        char* temp = genBinaryOp(quad_list, QUAD_IF_NE, (yyvsp[-2].expr_val).valueStr, (yyvsp[0].expr_val).valueStr);
        strcpy((yyval.expr_val).valueStr, temp);
        free(temp);
    }
#line 1682 "syntax.c"
    break;

  case 43: /* EXPRESSION: EXPRESSION mc_and EXPRESSION  */
#line 440 "syntax.y"
                                   {
        (yyval.expr_val).type = TYPE_INTEGER;  // Boolean result (0 or 1)
        (yyval.expr_val).value.intValue = (convertToInt((yyvsp[-2].expr_val)) && convertToInt((yyvsp[0].expr_val))) ? 1 : 0;
        sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
        
        // Generate logical AND quadruple (using IF_NE with 0)
        char* temp1 = generateTemp();
        sprintf(temp1, "t%d", tempCount++);
        addQuad(quad_list, QUAD_ASSIGN, (yyvsp[-2].expr_val).valueStr, NULL, temp1);
        
        char* temp2 = generateTemp();
        sprintf(temp2, "t%d", tempCount++);
        addQuad(quad_list, QUAD_ASSIGN, (yyvsp[0].expr_val).valueStr, NULL, temp2);
        
        char* resultTemp = generateTemp();
        sprintf(resultTemp, "t%d", tempCount++);
        addQuad(quad_list, QUAD_ASSIGN, "0", NULL, resultTemp);
        
        int labelTrue = genLabel(quad_list);
        addQuad(quad_list, QUAD_IF_EQ, temp1, "0", resultTemp);
        addQuad(quad_list, QUAD_IF_EQ, temp2, "0", resultTemp);
        addQuad(quad_list, QUAD_ASSIGN, "1", NULL, resultTemp);
        
        strcpy((yyval.expr_val).valueStr, resultTemp);
        free(temp1);
        free(temp2);
        free(resultTemp);
    }
#line 1715 "syntax.c"
    break;

  case 44: /* EXPRESSION: EXPRESSION mc_or EXPRESSION  */
#line 468 "syntax.y"
                                  {
        (yyval.expr_val).type = TYPE_INTEGER;  // Boolean result (0 or 1)
        (yyval.expr_val).value.intValue = (convertToInt((yyvsp[-2].expr_val)) || convertToInt((yyvsp[0].expr_val))) ? 1 : 0;
        sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
        
        // Generate logical OR quadruple (using IF_NE with 0)
        char* temp1 = generateTemp();
        sprintf(temp1, "t%d", tempCount++);
        addQuad(quad_list, QUAD_ASSIGN, (yyvsp[-2].expr_val).valueStr, NULL, temp1);
        
        char* temp2 = generateTemp();
        sprintf(temp2, "t%d", tempCount++);
        addQuad(quad_list, QUAD_ASSIGN, (yyvsp[0].expr_val).valueStr, NULL, temp2);
        
        char* resultTemp = generateTemp();
        sprintf(resultTemp, "t%d", tempCount++);
        addQuad(quad_list, QUAD_ASSIGN, "1", NULL, resultTemp);
        
        int labelFalse = genLabel(quad_list);
        addQuad(quad_list, QUAD_IF_NE, temp1, "0", resultTemp);
        addQuad(quad_list, QUAD_IF_NE, temp2, "0", resultTemp);
        addQuad(quad_list, QUAD_ASSIGN, "0", NULL, resultTemp);
        
        strcpy((yyval.expr_val).valueStr, resultTemp);
        free(temp1);
        free(temp2);
        free(resultTemp);
    }
#line 1748 "syntax.c"
    break;

  case 45: /* EXPRESSION: mc_paro EXPRESSION mc_parf  */
#line 496 "syntax.y"
                                 {
        (yyval.expr_val) = (yyvsp[-1].expr_val);  // Just pass the expression value up
    }
#line 1756 "syntax.c"
    break;

  case 46: /* EXPRESSION: mc_ident  */
#line 499 "syntax.y"
               {
        SymbolEntry* entry = lookupSymbol(symtab, (yyvsp[0].str_val));
        if (!entry) {
            fprintf(stderr, "-------------SEMANTIC Error: Undefined variable '%s' at line %d -------------\n", (yyvsp[0].str_val), line);
            (yyval.expr_val).type = TYPE_INTEGER;
            (yyval.expr_val).value.intValue = 0;
            sprintf((yyval.expr_val).valueStr, "0");
        } else {
            entry->isUsed = 1;
            
            if (!entry->isInitialized) {
                fprintf(stderr, "Warning: Uninitialized variable '%s' used at line %d\n", (yyvsp[0].str_val), line);
            }
            
            // Set the expression value based on variable type
            (yyval.expr_val).type = entry->type;
            switch (entry->type) {
                case TYPE_INTEGER:
                    (yyval.expr_val).value.intValue = entry->value.intValue;
                    sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
                    break;
                case TYPE_FLOAT:
                    (yyval.expr_val).value.floatValue = entry->value.floatValue;
                    sprintf((yyval.expr_val).valueStr, "%f", (yyval.expr_val).value.floatValue);
                    break;
                case TYPE_CHAR:
                    (yyval.expr_val).value.charValue = entry->value.charValue;
                    sprintf((yyval.expr_val).valueStr, "'%c'", (yyval.expr_val).value.charValue);
                    break;
                case TYPE_STRING:
                    if (entry->value.stringValue) {
                        (yyval.expr_val).value.stringValue = strdup(entry->value.stringValue);
                        sprintf((yyval.expr_val).valueStr, "%s", (yyval.expr_val).value.stringValue);
                    } else {
                        (yyval.expr_val).value.stringValue = strdup("");
                        sprintf((yyval.expr_val).valueStr, "\"\"");
                    }
                    break;
                default:
                    (yyval.expr_val).type = TYPE_INTEGER;
                    (yyval.expr_val).value.intValue = 0;
                    sprintf((yyval.expr_val).valueStr, "0");
                    break;
            }
            
            // For symbol reference, just use the name
            strcpy((yyval.expr_val).valueStr, (yyvsp[0].str_val));
        }
    }
#line 1810 "syntax.c"
    break;

  case 47: /* EXPRESSION: INT_CONST  */
#line 548 "syntax.y"
                {
        (yyval.expr_val).type = TYPE_INTEGER;
        (yyval.expr_val).value.intValue = (yyvsp[0].int_val);
        sprintf((yyval.expr_val).valueStr, "%d", (yyvsp[0].int_val));
    }
#line 1820 "syntax.c"
    break;

  case 48: /* EXPRESSION: mc_charactere  */
#line 553 "syntax.y"
                    {
        (yyval.expr_val).type = TYPE_CHAR;
        (yyval.expr_val).value.charValue = (yyvsp[0].char_val);
        sprintf((yyval.expr_val).valueStr, "'%c'", (yyvsp[0].char_val));
    }
#line 1830 "syntax.c"
    break;

  case 49: /* EXPRESSION: mc_chaine  */
#line 558 "syntax.y"
                {
        (yyval.expr_val).type = TYPE_STRING;
        (yyval.expr_val).value.stringValue = (yyvsp[0].str_val);
        sprintf((yyval.expr_val).valueStr, "%s", (yyvsp[0].str_val));
    }
#line 1840 "syntax.c"
    break;

  case 50: /* EXPRESSION: mc_cst  */
#line 563 "syntax.y"
             {
        (yyval.expr_val).type = TYPE_INTEGER;
        (yyval.expr_val).value.intValue = (yyvsp[0].int_val);
        sprintf((yyval.expr_val).valueStr, "%d", (yyvsp[0].int_val));
    }
#line 1850 "syntax.c"
    break;

  case 51: /* INSTRUCTION_LIRE: mc_read mc_paro mc_chaine mc_deuxp mc_adresse mc_ident mc_parf mc_pvg  */
#line 571 "syntax.y"
                                                                          {
        SymbolEntry* entry = lookupSymbol(symtab, (yyvsp[-2].str_val));
        if (!entry) {
            fprintf(stderr, "--------------SEMANTIC Error: Undefined variable '%s' at line %d column %d--------------\n", (yyvsp[-2].str_val), line, column);
        } else {
            // Check format specifier compatibility
            if (!isFormatCompatible((yyvsp[-5].str_val), entry->type)) {
                fprintf(stderr, "--------------SEMANTIC Error: Incompatible variable type '%s' with format specifier '%s' at line %d column %d--------------\n", 
                        (yyvsp[-2].str_val), (yyvsp[-5].str_val), line, column);
            } else {
                entry->isInitialized = 1;
                // Generate quadruple for read operation
                genReadQuad(quad_list, (yyvsp[-5].str_val), (yyvsp[-2].str_val));
            }
        }
    }
#line 1871 "syntax.c"
    break;

  case 52: /* INSTRUCTION_ECRIRE: mc_display mc_paro mc_chaine mc_deuxp mc_ident mc_parf mc_pvg  */
#line 590 "syntax.y"
                                                                  {
        SymbolEntry* entry = lookupSymbol(symtab, (yyvsp[-2].str_val));
        if (!entry) {
            fprintf(stderr, "---------------SEMANTIC Error: Undefined variable '%s' at line %d T column %d---------------\n", (yyvsp[-2].str_val), line, column);
        } else {
            entry->isUsed = 1;
            
            // Generate quadruple for write operation
            genWriteQuad(quad_list, (yyvsp[-4].str_val), (yyvsp[-2].str_val));
        }
    }
#line 1887 "syntax.c"
    break;

  case 54: /* STRUCTURE_CONDITIONELLE: mc_if mc_paro CONDITION mc_parf mc_deuxp INSTRUCTIONS mc_endp  */
#line 608 "syntax.y"
                                                                  {
        // Single if statement (no else)
        int ifCondLabel = genLabel(quad_list);
        // Generate conditional jump
        if ((yyvsp[-4].expr_val).type == TYPE_INTEGER) {
            addQuad(quad_list, QUAD_IF_FALSE, (yyvsp[-4].expr_val).valueStr, NULL, "");
        }
    }
#line 1900 "syntax.c"
    break;

  case 55: /* STRUCTURE_CONDITIONELLE: mc_if mc_paro CONDITION mc_parf mc_deuxp INSTRUCTIONS mc_else mc_deuxp INSTRUCTIONS mc_endp  */
#line 616 "syntax.y"
                                                                                                  {
        // If-else statement
        int elseLabel = genLabel(quad_list);
        int endIfLabel = genLabel(quad_list);
        
        // Generate conditional jumps
        if ((yyvsp[-7].expr_val).type == TYPE_INTEGER) {
            // If condition is false, jump to else
            addQuad(quad_list, QUAD_IF_FALSE, (yyvsp[-7].expr_val).valueStr, NULL, "");
            // At end of if block, jump to end
            genGoto(quad_list, endIfLabel);
            // Place else label
            genLabel(quad_list);
            // Place end label after else block
            genLabel(quad_list);
        }
    }
#line 1922 "syntax.c"
    break;

  case 56: /* CONDITION: EXPRESSION  */
#line 636 "syntax.y"
               {
        (yyval.expr_val) = (yyvsp[0].expr_val);  // Just pass the expression value up
        
        // Ensure condition is treated as a boolean
        if ((yyval.expr_val).type != TYPE_INTEGER) {
            (yyval.expr_val).type = TYPE_INTEGER;
            (yyval.expr_val).value.intValue = ((yyval.expr_val).value.intValue != 0) ? 1 : 0;
            sprintf((yyval.expr_val).valueStr, "%d", (yyval.expr_val).value.intValue);
        }
    }
#line 1937 "syntax.c"
    break;

  case 57: /* INSTRUCTION_BOUCLE: mc_for mc_paro mc_ident mc_deuxp INT_CONST mc_deuxp mc_ident mc_parf INSTRUCTION mc_endp  */
#line 649 "syntax.y"
                                                                                             {
        // For loop 
        // First identifier is the loop counter
        SymbolEntry* counterEntry = lookupSymbol(symtab, (yyvsp[-7].str_val));
        if (!counterEntry) {
            fprintf(stderr, " ------------SEMANTIC Error: Undefined variable '%s' at line %d COLUMN %d ------------\n", (yyvsp[-7].str_val), line, column);
        } else {
            counterEntry->isInitialized = 1;
            counterEntry->isUsed = 1;
            
            // Set counter to initial value
            addQuad(quad_list, QUAD_ASSIGN, "0", NULL, (yyvsp[-7].str_val));
            
            // Second constant is the limit
            char limitStr[20];
            sprintf(limitStr, "%d", (yyvsp[-5].int_val));
            
            // Third identifier is the condition variable
            SymbolEntry* condEntry = lookupSymbol(symtab, (yyvsp[-3].str_val));
            if (!condEntry) {
                fprintf(stderr, " ------------SEMANTIC Error: Undefined variable '%s' at line %d COLUMN %d ------------\n", (yyvsp[-3].str_val), line, column);
            } else {
                condEntry->isUsed = 1;
                
                // Start of loop label
                int startLabel = genLabel(quad_list);
                
                // Condition: counter < limit
                char* tempResult = generateTemp();
                addQuad(quad_list, QUAD_IF_LT, (yyvsp[-7].str_val), limitStr, tempResult);
                
                // If condition is false, exit the loop
                int endLabel = genLabel(quad_list);
                addQuad(quad_list, QUAD_IF_FALSE, tempResult, NULL, "");
                
                // Body of the loop
                // (INSTRUCTION is already parsed)
                
                // Increment the counter
                char* tempInc = generateTemp();
                addQuad(quad_list, QUAD_ADD, (yyvsp[-7].str_val), "1", tempInc);
                addQuad(quad_list, QUAD_ASSIGN, tempInc, NULL, (yyvsp[-7].str_val));
                
                // Jump back to start
                genGoto(quad_list, startLabel);
                
                // End of loop label
                genLabel(quad_list);
                
                free(tempResult);
                free(tempInc);
            }
        }
    }
#line 1996 "syntax.c"
    break;


#line 2000 "syntax.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 705 "syntax.y"


void yyerror(const char *s) {
    fprintf(stderr, "SYNTAX Error: %s at line %d column %d\n", s, line, column);
}

int main() {
    printf("Starting compilation...\n");
    
    // Initialize symbol table
    symtab = createSymbolTable();
    
    // Initialize quadruple list
    quad_list = createQuadList();
    
    int result = yyparse();
    if (result == 0) {
        printf("Compilation successful!\n");
        
        // Print original quadruples
        printf("\n--- Original Quadruples ---\n");
        printQuadList(quad_list);
        
        // Optimize the code
        printf("\n--- Optimizing Code ---\n");
        optimize_code(quad_list);
        
        // Print optimized quadruples
        print_optimized_code(quad_list);
    }
    
    // Free memory
    freeSymbolTable(symtab);
    freeQuadList(quad_list);
    
    return result;
}

int yywrap() {
    return 1;
}
