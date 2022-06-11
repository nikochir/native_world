#ifndef NW_LUA_CONFIG_H
#define NW_LUA_CONFIG_H
// standard C libraries;
#include <limits.h>
#include <stddef.h>
// general configuration file for lua
// some definitions here can be changed externally, through the compiler (e.g., with '-D' options);
// those are protected by '#if !defined' guards;
// however, several other definitions should be changed directly here
// either because they affect the lua api or because they are seldom changed;
// (by making the changes here, you ensure that all software connected to lua,
// such as C libraries, will be compiled with the same configuration)
// search for "@@" to find all configurable definitions;
//
// system configuration: macros to adapt (if needed) lua to some
// particular platform, for instance restricting it to C89.
//
// @@ NW_LUA_USE_C89 controls the use of non-ISO-C89 features;
// define it if you want lua to avoid the use of a few C99 features or windows-specific features on windows;
// #define NW_LUA_USE_C89
// by default, lua on windows use (some) specific windows features;
#if !defined(NW_LUA_USE_C89) && defined(_WIN32) && !defined(_WIN32_WCE)
#	define NW_LUA_USE_WINDOWS // enable goodies for regular windows;
#endif
// windows platform config
#if defined(NW_LUA_USE_WINDOWS)
#	define NW_LUA_DL_DLL	// enable support for dynamic library linking
#	define NW_LUA_USE_C89	// broadly, windows is C89
#endif
// linux platform config
#if defined(NW_LUA_USE_LINUX)
#	define NW_LUA_USE_POSIX
#	define NW_LUA_USE_DLOPEN		// needs an extra library: -ldl;
#endif
// macosx platform config
#if defined(NW_LUA_USE_MACOSX)
#	define NW_LUA_USE_POSIX
#	define NW_LUA_USE_DLOPEN		// MacOS does not need -ldl;
#endif
// @@ NW_LUAI_IS32INT is true if 'int' has (at least) 32 bits.
#define NW_LUAI_IS32INT	((UINT_MAX >> 30) >= 3)
// configuration for number types.
// @@ NW_LUA_32BITS enables Lua with 32-bit integers and 32-bit floats.
 #define NW_LUA_32BITS
// @@ NW_LUA_C89_NUMBERS ensures that lua uses the largest types available for
// C89 ('long' and 'double'); windows always has '__int64', so it does
// not need to use this case.
#if defined(NW_LUA_USE_C89) && !defined(NW_LUA_USE_WINDOWS)
#	define NW_LUA_C89_NUMBERS
#endif
// @@ NW_LUA_INT_TYPE defines the type for lua integers;
// @@ NW_LUA_FLOAT_TYPE defines the type for lua floats;
// lua should work fine with any mix of these options supported by your C compiler;
// the usual configurations are:
// default: 64-bit integers and 'double';
// for restricted platforms: 32-bit integers and 'float';
// for C compilers not compliant with C99: 'long'/'double' (may not have support for 'long long');
//
// predefined options for NW_LUA_INT_TYPE
#define NW_LUA_INT_INT		1
#define NW_LUA_INT_LONG		2
#define NW_LUA_INT_LONGLONG	3
// predefined options for NW_LUA_FLOAT_TYPE
#define NW_LUA_FLOAT_FLOAT		1
#define NW_LUA_FLOAT_DOUBLE		2
#define NW_LUA_FLOAT_LONGDOUBLE	3
// int and float
#if defined(NW_LUA_32BITS) // 32-bit integers and 'float'
#	if NW_LUAI_IS32INT  // use 'int' if big enough
#		define NW_LUA_INT_TYPE	NW_LUA_INT_INT
#	else  // otherwise use long
#		define NW_LUA_INT_TYPE	NW_LUA_INT_LONG
#	endif
#		define NW_LUA_FLOAT_TYPE	NW_LUA_FLOAT_FLOAT
#elif defined(NW_LUA_C89_NUMBERS)
// largest types available for C89 ('long' and 'double')
#	define NW_LUA_INT_TYPE	NW_LUA_INT_LONG
#	define NW_LUA_FLOAT_TYPE	NW_LUA_FLOAT_DOUBLE
#endif
// default configuration for 64-bit Lua ('long long' and 'double')
#if !defined(NW_LUA_INT_TYPE)
#	define NW_LUA_INT_TYPE	NW_LUA_INT_LONGLONG
#endif
#if !defined(NW_LUA_FLOAT_TYPE)
#	define NW_LUA_FLOAT_TYPE	NW_LUA_FLOAT_DOUBLE
#endif

// --==<configuration_for_paths>==--
// NW_LUAPATH_SEP is the character that separates templates in a path;
// NW_LUAPATH_MARK is the string that marks the substitution points in a template;
// NW_LUAEXEC_DIR in a Windows path is replaced by the executable's directory;
#define NW_LUA_PATH_SEP            ";"
#define NW_LUA_PATH_MARK           "?"
#define NW_LUA_EXEC_DIR            "!"
// @@ NW_LUA_PATH_DEFAULT is the default path that lua uses to look for lua libraries;
// @@ NW_LUA_CPATH_DEFAULT is the default path that lua uses to look for C libraries.
// CHANGE them if your machine has a non-conventional directory
// hierarchy or if you want to install your libraries in
// non-conventional directories.
#define NW_LUA_VDIR	NW_LUA_VERSION_MAJOR "." NW_LUA_VERSION_MINOR
#if defined(_WIN32)
// in windows, any exclamation mark ('!') in the path is replaced by the
// path of the directory of the executable file of the current process.
#	define NW_LUA_LDIR	"!\\lua\\"
#	define NW_LUA_CDIR	"!\\"
#	define NW_LUA_SHRDIR	"!\\..\\share\\lua\\" NW_LUA_VDIR "\\"
#	if !defined(NW_LUA_PATH_DEFAULT)
#	define NW_LUA_PATH_DEFAULT  \
		NW_LUA_LDIR"?.lua;"  NW_LUA_LDIR"?\\init.lua;" \
		NW_LUA_CDIR"?.lua;"  NW_LUA_CDIR"?\\init.lua;" \
		NW_LUA_SHRDIR"?.lua;" NW_LUA_SHRDIR"?\\init.lua;" \
		".\\?.lua;" ".\\?\\init.lua"
#	endif
#	if !defined(NW_LUA_CPATH_DEFAULT)
#	define NW_LUA_CPATH_DEFAULT \
		NW_LUA_CDIR"?.dll;" \
		NW_LUA_CDIR"..\\lib\\lua\\" NW_LUA_VDIR "\\?.dll;" \
		NW_LUA_CDIR"loadall.dll;" ".\\?.dll"
#	endif
#else
#	define NW_LUA_ROOT	"/usr/local/"
#	define NW_LUA_LDIR	NW_LUA_ROOT "share/lua/" NW_LUA_VDIR "/"
#	define NW_LUA_CDIR	NW_LUA_ROOT "lib/lua/" NW_LUA_VDIR "/"
#	if !defined(NW_LUA_PATH_DEFAULT)
#	define NW_LUA_PATH_DEFAULT  \
		NW_LUA_LDIR"?.lua;"  NW_LUA_LDIR"?/init.lua;" \
		NW_LUA_CDIR"?.lua;"  NW_LUA_CDIR"?/init.lua;" \
		"./?.lua;" "./?/init.lua"
#	endif
#	if !defined(NW_LUA_CPATH_DEFAULT)
#	define NW_LUA_CPATH_DEFAULT \
		NW_LUA_CDIR"?.so;" NW_LUA_CDIR"loadall.so;" "./?.so"
#	endif
#endif
// @@ NW_LUA_DIRSEP is the directory separator (for submodules);
// change it if your machine does not use "/" as the directory separator and is not windows;
// (on windows lua automatically uses "\");
#if !defined(NW_LUA_DIRSEP)
#	if defined(_WIN32)
#		define NW_LUA_DIRSEP	"\\"
#	else
#		define NW_LUA_DIRSEP	"/"
#	endif
#endif
// marks for exported symbols in the C code;
// @@ NW_LUA_API is a mark for all core api functions;
// @@ NW_LUA_API is a mark for all auxiliary library functions;
// @@ NW_LUAMOD_API is a mark for all standard library opening functions;
// change them if you need to define those functions in some special way;
// for instance, if you want to create one windows dll with the core and
// the libraries, you may want to use the following definition;
// (define NW_LUA_BUILD_AS_DLL to get it);
#if defined(NW_LUA_BUILD_DLL)
#	if defined(NW_LUA_CORE) || defined(NW_LUA_LIB)
#		define NW_LUA_API __declspec(dlua_lexport)
#	else
	#	define NW_LUA_API __declspec(dllimport)
#	endif
#else
#	define NW_LUALIB_API		extern
#endif
#define NW_LUA_API		NW_LUALIB_API
#define NW_LUAMOD_API	NW_LUA_API
// @@ NW_LUAI_FUNC is a mark for all extern functions that are not to be exported to outside modules;
// @@ NW_LUAI_DDEF and NW_LUAI_DDEC are marks for all extern (const) variables,
// none of which to be exported to outside modules (NW_LUAI_DDEF for
// definitions and NW_LUAI_DDEC for declarations);
// CHANGE them if you need to mark them in some special way. Elf/gcc
// (versions 3.2 and later) mark them as "hidden" to optimize access
// when Lua is compiled as a shared library. Not all elf targets support
// this attribute. Unfortunately, gcc does not offer a way to check
// whether the target offers that support, and those without support
// give a warning about it. To avoid these warnings, change to the
// default definition.
#if defined(__GNUC__) && ((__GNUC__*100 + __GNUC_MINOR__) >= 302) && defined(__ELF__) 
#	define NW_LUAI_FUNC	__attribute__((visibility("internal"))) extern
#else 
#	define NW_LUAI_FUNC	extern
#endif

#define NW_LUAI_DDEC(dec)	NW_LUAI_FUNC dec
#define NW_LUAI_DDEF	 // empty 
// --==</configuration_for_paths>==--

// --==<compatibility_with_previous_versions>==---
// @@ NW_LUA_COMPAT_5_3 controls other macros for compatibility with lua 5.3;
// you can define it to get all options, or change specific options to fit your specific needs;
#if defined(NW_LUA_COMPAT_5_3)
// @@ NW_LUA_COMPAT_MATHLIB controls the presence of several deprecated
// functions in the mathematical library.
// (These functions were already officially removed in 5.3;
// nevertheless they are still available here.)
#	define NW_LUA_COMPAT_MATHLIB
// @@ NW_LUA_COMPAT_APIINTCASTS controls the presence of macros for
// manipulating other integer types (lua_pushunsigned, lua_tounsigned,
// luaL_checkint, luaL_checklong, etc.)
// (These macros were also officially removed in 5.3, but they are still
// available here.)
#	define NW_LUA_COMPAT_APIINTCASTS
// @@ NW_LUA_COMPAT_LT_LE controls the emulation of the '__le' metamethod
// using '__lt'.
#	define NW_LUA_COMPAT_LT_LE
// @@ the following macros supply trivial compatibility for some changes in the api;
// the macros themselves document how to change your code to avoid using them;
// (once more, these macros were officially removed in 5.3, but they are still available here);
#	define lua_strlen(L,i)		lua_rawlen(L, (i))
#	define lua_objlen(L,i)		lua_rawlen(L, (i))
#	define lua_equal(L,idx1,idx2)		lua_compare(L,(idx1),(idx2),NW_LUA_OPEQ)
#	define lua_lessthan(L,idx1,idx2)	lua_compare(L,(idx1),(idx2),NW_LUA_OPLT)
#endif
// --==</compatibility_with_previous_versions>==---

// --==<configuration_for_numbers>==--
// change these definitions if no predefined NW_LUA_FLOAT_* / NW_LUA_INT_*
// satisfy your needs;
// @@ NW_LUA_NUMBER is the floating-point type used by lua;
// @@ NW_LUAI_UACNUMBER is the result of a 'default argument promotion'
// @@ over a floating number;
// @@ l_floatatt(x) corrects float attribute 'x' to the proper float type
// by prefixing it with one of FLT/DBL/LDBL;
// @@ NW_LUA_NUMBER_FRMLEN is the length modifier for writing floats;
// @@ NW_LUA_NUMBER_FMT is the format for writing floats;
// @@ lua_number2str converts a float to a string;
// @@ l_mathop allows the addition of an 'l' or 'f' to all math operations;
// @@ l_floor takes the floor of a float;
// @@ lua_str2number converts a decimal numeral to a number;
// the following definitions are good for most cases here 
#define l_floor(x)		(l_mathop(floor)(x))
#define lua_number2str(s,sz,n) l_sprintf((s), sz, NW_LUA_NUMBER_FMT, (NW_LUAI_UACNUMBER)(n))
// @@ lua_numbertointeger converts a float number with an integral value
// to an integer, or returns 0 if float is not within the range of
// a lua_Integer.  (The range comparisons are tricky because of
// rounding. The tests here assume a two-complement representation,
// where MININTEGER always has an exact representation as a float;
// MAXINTEGER may not have one, and therefore its conversion to float
// may have an ill-defined value);
#define lua_numbertointeger(n,p) \
  ((n) >= (NW_LUA_NUMBER)(NW_LUA_MININTEGER) && \
   (n) < -(NW_LUA_NUMBER)(NW_LUA_MININTEGER) && \
      (*(p) = (NW_LUA_INTEGER)(n), 1))
// now the variable definitions
#if NW_LUA_FLOAT_TYPE == NW_LUA_FLOAT_FLOAT
#	define NW_LUA_NUMBER	float
#	define l_floatatt(n)		(FLT_##n)
#	define NW_LUAI_UACNUMBER	double
#	define NW_LUA_NUMBER_FRMLEN	""
#	define NW_LUA_NUMBER_FMT		"%.7g"
#	define l_mathop(op)		op##f
#	define lua_str2number(s,p)	strtof((s), (p))
#elif NW_LUA_FLOAT_TYPE == NW_LUA_FLOAT_LONGDOUBLE
#	define NW_LUA_NUMBER	long double
#	define l_floatatt(n)		(LDBL_##n)
#	define NW_LUAI_UACNUMBER	long double
#	define NW_LUA_NUMBER_FRMLEN	"L"
#	define NW_LUA_NUMBER_FMT		"%.19Lg"
#	define l_mathop(op)		op##l
#	define lua_str2number(s,p)	strtold((s), (p))
#elif NW_LUA_FLOAT_TYPE == NW_LUA_FLOAT_DOUBLE
#	define NW_LUA_NUMBER		double
#	define l_floatatt(n)	(DBL_##n)
#	define NW_LUAI_UACNUMBER	double
#	define NW_LUA_NUMBER_FRMLEN	""
#	define NW_LUA_NUMBER_FMT		"%.14g"
#	define l_mathop(op)		op
#	define lua_str2number(s,p)	strtod((s), (p))
#else
#	error "numeric float type not defined"
#endif
// @@ NW_LUA_INTEGER is the integer type used by lua;
// @@ NW_LUA_UNSIGNED is the unsigned version of NW_LUA_INTEGER;
// @@ NW_LUA_UINT is the result of a "default argument promotion" over a NW_LUA_INTEGER;
// @@ NW_LUA_INTEGER_FRMLEN is the length modifier for reading/writing integers;
// @@ NW_LUA_INTEGER_FMT is the format for writing integers;
// @@ NW_LUA_MAXINTEGER is the maximum value for a NW_LUA_INTEGER;
// @@ NW_LUA_MININTEGER is the minimum value for a NW_LUA_INTEGER;
// @@ NW_LUA_MAXUNSIGNED is the maximum value for a NW_LUA_UNSIGNED;
// @@ NW_LUA_UNSIGNEDBITS is the number of bits in a NW_LUA_UNSIGNED;
// @@ lua_integer2str converts an integer to a string;
// the following definitions are good for most cases here
#define NW_LUA_INTEGER_FMT		"%" NW_LUA_INTEGER_FRMLEN "d"
#define NW_LUA_UINT				unsigned NW_LUA_INTEGER
#define lua_integer2str(s,sz,n) l_sprintf((s), sz, NW_LUA_INTEGER_FMT, (NW_LUA_UINT)(n))
// use NW_LUA_UINT here to avoid problems with promotions
// (which can turn a comparison between unsigneds into a signed comparison)
#define NW_LUA_UNSIGNED		unsigned
#define NW_LUA_UNSIGNEDBITS	(sizeof(NW_LUA_UNSIGNED) * CHAR_BIT)
// now the variable definitions
#if NW_LUA_INT_TYPE == NW_LUA_INT_INT
#	define NW_LUA_INTEGER			int
#	define NW_LUA_INTEGER_FRMLEN	""
#	define NW_LUA_MAXINTEGER		INT_MAX
#	define NW_LUA_MININTEGER		INT_MIN
#	define NW_LUA_MAXUNSIGNED		UINT_MAX
#elif NW_LUA_INT_TYPE == NW_LUA_INT_LONG
#	define NW_LUA_INTEGER		long
#	define NW_LUA_INTEGER_FRMLEN	"l"
#	define NW_LUA_MAXINTEGER		LONG_MAX
#	define NW_LUA_MININTEGER		LONG_MIN
#	define NW_LUA_MAXUNSIGNED		ULONG_MAX
#elif NW_LUA_INT_TYPE == NW_LUA_INT_LONGLONG // use presence of macro LLONG_MAX as proxy for C99 compliance;
#	if defined(LLONG_MAX)	// use iso C99 stuff 
#		define NW_LUA_INTEGER			long long
#		define NW_LUA_INTEGER_FRMLEN	"ll"
#		define NW_LUA_MAXINTEGER		LLONG_MAX
#		define NW_LUA_MININTEGER		LLONG_MIN
#		define NW_LUA_MAXUNSIGNED		ULLONG_MAX
#	elif defined(NW_LUA_USE_WINDOWS) 
 // in windows, can use specific windows types;
#		define NW_LUA_INTEGER		__int64
#		define NW_LUA_INTEGER_FRMLEN	"I64"
#		define NW_LUA_MAXINTEGER		_I64_MAX
#		define NW_LUA_MININTEGER		_I64_MIN
#		define NW_LUA_MAXUNSIGNED		_UI64_MAX
#	else
#		error "Compiler does not support 'long long'. Use option '-DNW_LUA_32BITS' \
	or '-DNW_LUA_C89_NUMBERS' (see file 'lua_config.h' for details)"
#	endif
#else
#	error "numeric integer type not defined"
#endif
// dependencies with C99 and other C details
// @@ l_sprintf is equivalent to 'snprintf' or 'sprintf' in C89.
// (all uses in lua have only one format item);
#if !defined(NW_LUA_USE_C89)
#	define l_sprintf(s,sz,f,i)	snprintf(s,sz,f,i)
#else
#	define l_sprintf(s,sz,f,i)	((void)(sz), sprintf(s,f,i))
#endif
// @@ lua_strx2number converts a hexadecimal numeral to a number;
// in C99, 'strtod' does that conversion;
// otherwise, you can leave 'lua_strx2number' undefined and lua will provide its own implementation;
#if !defined(NW_LUA_USE_C89)
#	define lua_strx2number(s,p)		lua_str2number(s,p)
#endif
// @@ lua_pointer2str converts a pointer to a readable string in a non-specified way;
#define lua_pointer2str(buff,sz,p)	l_sprintf(buff,sz,"%p",p)
// @@ lua_number2strx converts a float to a hexadecimal numeral;
// in C99, 'sprintf' (with format specifiers '%a'/'%A') does that;
// otherwise, you can leave 'lua_number2strx' undefined and lua will provide its own implementation;
#if !defined(NW_LUA_USE_C89)
#	define lua_number2strx(L,b,sz,f,n)	((void)L, l_sprintf(b,sz,f,(NW_LUAI_UACNUMBER)(n)))
#endif
// 'strtof' and 'opf' variants for math functions are not valid in C89;
// otherwise, the macro 'HUGE_VALF' is a good proxy for testing the availability of these variants;
// ('math.h' is already included in all files that use these macros);
#if defined(NW_LUA_USE_C89) || (defined(HUGE_VAL) && !defined(HUGE_VALF))
#	undef l_mathop   // variants not available 
#	undef lua_str2number
#	define l_mathop(op)		(lua_Number)op   // no variant 
#	define lua_str2number(s,p)	((lua_Number)strtod((s), (p)))
#endif
// @@ NW_LUA_KCONTEXT is the type of the context ('ctx') for continuation functions;
// it must be a numerical type;
// lua will use 'intptr_t' if available, otherwise it will use 'ptrdiff_t'
// (the nearest thing to 'intptr_t' in C89);
#define NW_LUA_KCONTEXT	ptrdiff_t
#if !defined(NW_LUA_USE_C89) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#	include <stdint.h>
#	if defined(INTPTR_MAX)   // even in C99 this type is optional 
#		undef NW_LUA_KCONTEXT
#		define NW_LUA_KCONTEXT	intptr_t
#	endif
#endif
// @@ lua_getlocaledecpoint gets the locale "radix character" (decimal point);
// change that if you do not want to use C locales;
// (code using this macro must include the header 'locale.h'.)
#if !defined(lua_getlocaledecpoint)
#	define lua_getlocaledecpoint()		(localeconv()->decimal_point[0])
#endif
// --==</configuration_for_numbers>==--

// --==<language_variations>==--
// @@ NW_LUA_NOCVTN2S/NW_LUA_NOCVTS2N control how lua performs some convertions;
// define NW_LUA_NOCVTN2S to turn off automatic convertion from numbers to strings;
// #define NW_LUA_NOCVTN2S
// #define NW_LUA_NOCVTS2N
// @@ NW_LUA_USE_APICHECK turns on several consistency checks on the C api;
// define it as a help when debugging C code;
#if defined(NW_LUA_USE_APICHECK)
#	include <assert.h>
#	define luai_apicheck(l,e)	assert(e)
#endif
// macros that affect the API and must be stable
// (that is, must be the same when you compile lua and when you compile code that links to lua);
// @@ NW_LUAI_MAXSTACK limits the size of the Lua stack.
// CHANGE it if you need a different limit. This limit is arbitrary;
// its only purpose is to stop Lua from consuming unlimited stack
// space (and to reserve some numbers for pseudo-indices).
// (It must fit into max(size_t)/32.)
#if NW_LUAI_IS32INT
#	define NW_LUAI_MAXSTACK		1000000
#else
#	define NW_LUAI_MAXSTACK		15000
#endif
// @@ NW_LUA_EXTRASPACE defines the size of a raw memory area
// associated with a lua state with very fast access;
// change it if you need a different size;
#define NW_LUA_EXTRASPACE		(sizeof(void *))
// @@ NW_LUA_IDSIZE gives the maximum size for the description of the source of a function in debug information;
// change it if you want a different size.
#define NW_LUA_IDSIZE	60
// @@ NW_LUAL_BUFFERSIZE is the buffer size used by the lauxlib buffer system;
#define NW_LUAL_BUFFERSIZE   ((int)(16 * sizeof(void*) * sizeof(lua_Number)))
// @@ NW_LUAI_MAXALIGN defines fields that, when used in a union, ensure
// maximum alignment for the other items in that union;
#define NW_LUAI_MAXALIGN  lua_Number n; double u; void *s; lua_Integer i; long l
// --==</language_variations>==--

// --==<local_configuration>==--
// you can use this space to add your redefinitions
// without modifying the main part of the file.
// --==</local_configuration>==--

#endif	// NW_LUACONFIG_H
