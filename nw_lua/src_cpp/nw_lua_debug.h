#ifndef NW_LUA_DEBUG_H
#define NW_LUA_DEBUG_H

#include "nw_lua_stt.h"

#define pcRel(pc, p)	(cast_int((pc) - (p)->code) - 1)

/* Active Lua function (given call info) */
#define ci_func(ci)		(clLvalue(s2v((ci)->func)))


#define resethookcount(L)	(L->hookcount = L->basehookcount)

// mark for entries in 'lineinfo' array that has absolute information in
// 'abslineinfo' array
#define ABSLINEINFO	(-0x80)

NW_LUAI_FUNC int luaG_getfuncline (const Proto *f, int pc);
NW_LUAI_FUNC const char *luaG_findlocal (lua_State *L, CallInfo *ci, int n,
                                                    StkId *pos);
NW_LUAI_FUNC l_noret luaG_typeerror (lua_State *L, const TValue *o,
                                                const char *opname);
NW_LUAI_FUNC l_noret luaG_forerror (lua_State *L, const TValue *o,
                                               const char *what);
NW_LUAI_FUNC l_noret luaG_concaterror (lua_State *L, const TValue *p1,
                                                  const TValue *p2);
NW_LUAI_FUNC l_noret luaG_opinterror (lua_State *L, const TValue *p1,
                                                 const TValue *p2,
                                                 const char *msg);
NW_LUAI_FUNC l_noret luaG_tointerror (lua_State *L, const TValue *p1,
                                                 const TValue *p2);
NW_LUAI_FUNC l_noret luaG_ordererror (lua_State *L, const TValue *p1,
                                                 const TValue *p2);
NW_LUAI_FUNC l_noret luaG_runerror (lua_State *L, const char *fmt, ...);
NW_LUAI_FUNC const char *luaG_addinfo (lua_State *L, const char *msg,
                                                  TString *src, int line);
NW_LUAI_FUNC l_noret luaG_errormsg (lua_State *L);
NW_LUAI_FUNC int luaG_traceexec (lua_State *L, const Instruction *pc);

#endif  // NW_LUADEBUG_H
