#ifndef NW_LUA_PREFIX_H
#define NW_LUA_PREFIX_H
// definitions for Lua code that must come before any other header file
// allows POSIX/XSI stuff
#if !defined(NW_LUA_USE_C89)
#	if !defined(_XOPEN_SOURCE)
#		define _XOPEN_SOURCE           600
#	elif _XOPEN_SOURCE == 0
#		undef _XOPEN_SOURCE  /* use -D_XOPEN_SOURCE=0 to undefine it */
#	endif
// allows manipulation of large files in gcc and some other compilers
#	if !defined(NW_LUA_32BITS) && !defined(_FILE_OFFSET_BITS)
#		define _LARGEFILE_SOURCE       1
#		define _FILE_OFFSET_BITS       64
#	endif
#endif
// windows stuff
#if defined(_WIN32)
#	if !defined(_CRT_SECURE_NO_WARNINGS)
#		define _CRT_SECURE_NO_WARNINGS  // avoid warnings about ISO C functions
#	endif
#endif
#endif	// NW_LUAPREFIX_H
