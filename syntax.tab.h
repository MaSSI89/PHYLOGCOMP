/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SYNTAX_TAB_H_INCLUDED
# define YY_YY_SYNTAX_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    mc_ident = 258,                /* mc_ident  */
    mc_data = 259,                 /* mc_data  */
    mc_endp = 260,                 /* mc_endp  */
    mc_code = 261,                 /* mc_code  */
    mc_comment = 262,              /* mc_comment  */
    mc_const = 263,                /* mc_const  */
    mc_vector = 264,               /* mc_vector  */
    mc_integ = 265,                /* mc_integ  */
    mc_float = 266,                /* mc_float  */
    mc_char = 267,                 /* mc_char  */
    mc_string = 268,               /* mc_string  */
    mc_deuxp = 269,                /* mc_deuxp  */
    mc_pvg = 270,                  /* mc_pvg  */
    mc_affectation = 271,          /* mc_affectation  */
    mc_crocheto = 272,             /* mc_crocheto  */
    mc_crochetf = 273,             /* mc_crochetf  */
    mc_vg = 274,                   /* mc_vg  */
    mc_separateur = 275,           /* mc_separateur  */
    mc_paro = 276,                 /* mc_paro  */
    mc_parf = 277,                 /* mc_parf  */
    mc_plus = 278,                 /* mc_plus  */
    mc_moins = 279,                /* mc_moins  */
    mc_mult = 280,                 /* mc_mult  */
    mc_div = 281,                  /* mc_div  */
    mc_read = 282,                 /* mc_read  */
    mc_display = 283,              /* mc_display  */
    mc_if = 284,                   /* mc_if  */
    mc_else = 285,                 /* mc_else  */
    mc_for = 286,                  /* mc_for  */
    mc_sup = 287,                  /* mc_sup  */
    mc_inf = 288,                  /* mc_inf  */
    mc_supeg = 289,                /* mc_supeg  */
    mc_infeg = 290,                /* mc_infeg  */
    mc_egal = 291,                 /* mc_egal  */
    mc_diff = 292,                 /* mc_diff  */
    mc_adresse = 293,              /* mc_adresse  */
    INT_CONST = 294,               /* INT_CONST  */
    mc_charactere = 295,           /* mc_charactere  */
    mc_chaine = 296,               /* mc_chaine  */
    mc_cst = 297,                  /* mc_cst  */
    mc_or = 298,                   /* mc_or  */
    mc_and = 299,                  /* mc_and  */
    mc_start = 300                 /* mc_start  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 43 "syntax.y"

    int int_val;
    char char_val;
    char* str_val;
    struct ExprValue expr_val;

#line 116 "syntax.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SYNTAX_TAB_H_INCLUDED  */
