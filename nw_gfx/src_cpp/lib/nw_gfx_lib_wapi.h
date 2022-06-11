#ifndef NW_GFX_LIB_WINDOW_API_H
#define NW_GFX_LIB_WINDOW_API_H
#include "nw_gfx_core.hpp"
#if (defined NW_GAPI)
#	if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	// context
	typedef context_handle(__stdcall* pfn_gfx_new_context)    (device_handle device);
	typedef v1s(__stdcall* pfn_gfx_del_context)    (context_handle context);
	typedef context_handle(__stdcall* pfn_gfx_get_context)    (v1nil);
	typedef v1s(__stdcall* pfn_gfx_set_context)    (device_handle device, context_handle context);
	// device
	typedef device_handle(__stdcall* pfn_gfx_get_device)     (v1nil);
	// other
	typedef ptr_t(__stdcall* pfn_gfx_get_proc)       (cstr_t name);
	// extensions
	typedef cstr_t(__stdcall* pfn_gfx_ext_get_str)   (v1nil);
	typedef v1s(__stdcall* pfn_gfx_ext_get_vsync)    (v1nil);
	typedef v1nil(__stdcall* pfn_gfx_ext_set_vsync)  (v1s swap_interval);
}
namespace NW
{
	// context
	extern pfn_gfx_new_context gfx_new_context;
	extern pfn_gfx_del_context gfx_del_context;
	extern pfn_gfx_get_context gfx_get_context;
	extern pfn_gfx_set_context gfx_set_context;
	// device
	extern pfn_gfx_get_device    gfx_get_device;
	// other
	extern pfn_gfx_get_proc      gfx_get_proc_address;
	// extensions
	extern pfn_gfx_ext_get_str   gfx_ext_get_str;
	extern pfn_gfx_ext_get_vsync gfx_ext_get_vsync;
	extern pfn_gfx_ext_set_vsync gfx_ext_set_vsync;
}
// context
#	define wglCreateContext       gfx_new_context
#	define wglDeleteContext       gfx_del_context
#	define wglGetCurrentContext   gfx_get_context
#	define wglMakeContextCurrent  gfx_set_context
// device
#	define wglGetCurrentDC        gfx_get_device
// other
#	define wglGetProcAddress      gfx_get_proc_address
// extensions
#	define wglGetExtensionsStringEXT gfx_ext_get_str
#	define wglSwapIntervalEXT        gfx_ext_set_vsync
#	define wglGetSwapIntervalEXT     gfx_ext_get_vsync
#	endif	// GAPI_OGL
#endif	// NW_GAPI
#endif	// NW_GFX_LIB_WINDOW_API_H