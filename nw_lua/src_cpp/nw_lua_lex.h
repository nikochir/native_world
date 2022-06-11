/*
** $Id: lua_lex.h $
** Lexical Analyzer
** See Copyright Notice in lua_core.h
*/

#ifndef lua_lex_h
#define lua_lex_h

#include <limits.h>

#include "nw_lua_obj.h"
#include "nw_lua_zbuf.h"


/*
** Single-char tokens (terminal symbols) are represented by their own
** numeric code. Other tokens start at the following value.
*/
#define FIRST_RESERVED	(UCHAR_MAX + 1)


#if !defined(NW_LUA_ENV)
#define NW_LUA_ENV		"_ENV"
#endif


/*
* WARNING: if you change the order of this enumeration,
* grep "ORDER RESERVED"
*/
enum RESERVED {
  /* terminal symbols denoted by reserved words */
  TK_AND = FIRST_RESERVED, TK_BREAK,
  TK_DO, TK_ELSE, TK_ELSEIF, TK_END, TK_FALSE, TK_FOR, TK_FUNCTION,
  TK_GOTO, TK_IF, TK_IN, TK_LOCAL, TK_NIL, TK_NOT, TK_OR, TK_REPEAT,
  TK_RETURN, TK_THEN, TK_TRUE, TK_UNTIL, TK_WHILE,
  /* other terminal symbols */
  TK_IDIV, TK_CONCAT, TK_DOTS, TK_EQ, TK_GE, TK_LE, TK_NE,
  TK_SHL, TK_SHR,
  TK_DBCOLON, TK_EOS,
  TK_FLT, TK_INT, TK_NAME, TK_STRING
};

/* number of reserved words */
#define NUM_RESERVED	(cast_int(TK_WHILE-FIRST_RESERVED + 1))


typedef union {
  lua_Number r;
  lua_Integer i;
  TString *ts;
} SemInfo;  /* semantics information */


typedef struct Token {
  int token;
  SemInfo seminfo;
} Token;


/* state of the lexer plus state of the parser when shared by all
   functions */
typedef struct LexState {
  int current;  /* current character (charint) */
  int linenumber;  /* input line counter */
  int lastline;  /* line of last token 'consumed' */
  Token t;  /* current token */
  Token lookahead;  /* look ahead token */
  struct FuncState *fs;  /* current function (parser) */
  struct lua_State *L;
  ZIO *z;  /* input stream */
  Mbuffer *buff;  /* buffer for tokens */
  Table *h;  /* to avoid collection/reuse strings */
  struct Dyndata *dyd;  /* dynamic structures used by the parser */
  TString *source;  /* current source name */
  TString *envn;  /* environment variable name */
} LexState;


NW_LUAI_FUNC void luaX_init (lua_State *L);
NW_LUAI_FUNC void luaX_setinput (lua_State *L, LexState *ls, ZIO *z,
                              TString *source, int firstchar);
NW_LUAI_FUNC TString *luaX_newstring (LexState *ls, const char *str, size_t l);
NW_LUAI_FUNC void luaX_next (LexState *ls);
NW_LUAI_FUNC int luaX_lookahead (LexState *ls);
NW_LUAI_FUNC l_noret luaX_syntaxerror (LexState *ls, const char *s);
NW_LUAI_FUNC const char *luaX_token2str (LexState *ls, int token);


#endif
