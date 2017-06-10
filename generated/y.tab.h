/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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

#ifndef YY_YY_GENERATED_Y_TAB_H_INCLUDED
# define YY_YY_GENERATED_Y_TAB_H_INCLUDED
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
    NUMBER = 258,
    WORD = 259,
    COMMENT = 260,
    CMP = 261,
    DECIMAL = 262,
    NAME = 263,
    SCIENTIFICVAL = 264,
    EOL = 265,
    ATAN = 266,
    ATAN2 = 267,
    COS = 268,
    SIN = 269,
    SQRT = 270,
    SIGN = 271,
    FUN = 272,
    IF = 273,
    FOR = 274,
    WHILE = 275,
    END = 276
  };
#endif
/* Tokens.  */
#define NUMBER 258
#define WORD 259
#define COMMENT 260
#define CMP 261
#define DECIMAL 262
#define NAME 263
#define SCIENTIFICVAL 264
#define EOL 265
#define ATAN 266
#define ATAN2 267
#define COS 268
#define SIN 269
#define SQRT 270
#define SIGN 271
#define FUN 272
#define IF 273
#define FOR 274
#define WHILE 275
#define END 276

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 30 "yacc/mcc.y" /* yacc.c:1909  */

	int ival; // integer values for constants
	double dval; // double values for constants
	char *string; // string values for constants
	char *name;	 // name of function or variable
	TreeNode * tree;

#line 104 "generated/y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GENERATED_Y_TAB_H_INCLUDED  */
