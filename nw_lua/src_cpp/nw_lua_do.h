#ifndef NW_LUA_DO_H
#define NW_LUA_DO_H

#include "nw_lua_obj.h"
#include "nw_lua_stt.h"
#include "nw_lua_zbuf.h"

// ,acro to check stack size and grow stack if needed.
// parameters 'pre'/'pos' allow the macro to preserve a pointer into the
// stack across reallocations, doing the work only when needed;
// it also allows the running of one GC step when the stack is reallocated
// 'condmovestack' is used in heavy tests to force a stack reallocation at every check
#define luaD_checkstackaux(L,n,pre,pos)  \
	if (L->stack_last - L->top <= (n)) \
	  { pre; luaD_growstack(L, n, 1); pos; } \
        else { condmovestack(L,pre,pos); }

// in general, 'pre'/'pos' are empty (nothing to save)
#define luaD_checkstack(L,n)	luaD_checkstackaux(L,n,(void)0,(void)0)

#define savestack(L,p)		((char *)(p) - (char *)L->stack)
#define restorestack(L,n)	((StkId)((char *)L->stack + (n)))

// macro to check stack size, preserving 'p'
#define checkstackGCp(L,n,p)  \
  luaD_checkstackaux(L, n, \
    ptrdiff_t t__ = savestack(L, p);  /* save 'p' */ \
    luaC_checkGC(L),  /* stack grow uses memory */ \
    p = restorestack(L, t__))  /* 'pos' part: restore 'p' */

// macro to check stack size and GC
#define checkstackGC(L,fsize)  \
	luaD_checkstackaux(L, (fsize), luaC_checkGC(L), (void)0)

// type of protected functions, to be ran by 'runprotected'
typedef void (*Pfunc) (lua_State *L, void *ud);

NW_LUAI_FUNC void luaD_seterrorobj (lua_State *L, int errcode, StkId oldtop);
NW_LUAI_FUNC int luaD_protectedparser (lua_State *L, ZIO *z, const char *name,
                                                  const char *mode);
NW_LUAI_FUNC void luaD_hook (lua_State *L, int event, int line,
                                        int fTransfer, int nTransfer);
NW_LUAI_FUNC void luaD_hookcall (lua_State *L, CallInfo *ci);
NW_LUAI_FUNC void luaD_pretailcall (lua_State *L, CallInfo *ci, StkId func, int n);
NW_LUAI_FUNC CallInfo *luaD_precall (lua_State *L, StkId func, int nResults);
NW_LUAI_FUNC void luaD_call (lua_State *L, StkId func, int nResults);
NW_LUAI_FUNC void luaD_callnoyield (lua_State *L, StkId func, int nResults);
NW_LUAI_FUNC void luaD_tryfuncTM (lua_State *L, StkId func);
NW_LUAI_FUNC int luaD_pcall (lua_State *L, Pfunc func, void *u,
                                        ptrdiff_t oldtop, ptrdiff_t ef);
NW_LUAI_FUNC void luaD_poscall (lua_State *L, CallInfo *ci, int nres);
NW_LUAI_FUNC int luaD_reallocstack (lua_State *L, int newsize, int raiseerror);
NW_LUAI_FUNC int luaD_growstack (lua_State *L, int n, int raiseerror);
NW_LUAI_FUNC void luaD_shrinkstack (lua_State *L);
NW_LUAI_FUNC void luaD_inctop (lua_State *L);

NW_LUAI_FUNC l_noret luaD_throw (lua_State *L, int errcode);
NW_LUAI_FUNC int luaD_rawrunprotected (lua_State *L, Pfunc f, void *ud);

#endif  // NW_LUADO_H
