#include "nw_app_pch.hpp"
#include "nw_app_wnd.h"
#if (defined NW_WAPI)
#	if (NW_WAPI & NW_WAPI_WIN)
namespace NW
{
	app_wnd::app_wnd() :
		a_mem_user(),
		m_handle{ NW_NULL },
		m_class{ NW_NULL },
		m_msg{ NW_NULL },
		m_title(NW_DEFAULT_STR),
		m_ver_str(NW_DEFAULT_STR),
		m_viewp(NW_NULL),
		m_ratio(NW_NULL),
		m_opacity(NW_NULL),
		m_is_enabled(NW_NULL),
		m_is_hovered(NW_NULL),
		m_is_focused(NW_NULL),
		m_event_proc([](iop_event_t&)->v1nil { return; })
	{
	}
	app_wnd::~app_wnd()
	{
		if (m_handle != NULL) { ::DestroyWindow(m_handle); m_handle = { NW_NULL }; }
		if (m_class.lpfnWndProc != NULL) { ::UnregisterClass(m_class.lpszClassName, m_class.hInstance); m_class = { NW_NULL }; }
	}
	// --setters
	v1nil app_wnd::set_size_xy(cv2u size_xy) {
		m_viewp[2] = size_xy[0];
		m_viewp[3] = size_xy[1];
		//SetWindowPos(m_handle, NULL, 0, 0, GetSizeW(), GetSizeH(), SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
		PostMessage(m_handle, WM_PAINT, NW_NULL, NW_NULL);
	}
	v1nil app_wnd::set_coord_xy(cv2u coord_xy) {
		m_viewp[0] = coord_xy[0];
		m_viewp[1] = coord_xy[1];
		//SetWindowPos(m_handle, NULL, GetCoordX(), GetCoordY(), 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
		PostMessage(m_handle, WM_PAINT, NW_NULL, NW_NULL);
	}
	v1nil app_wnd::set_viewp_xywh(cv4u viewport) {
		m_viewp[0] = viewport[0];
		m_viewp[1] = viewport[1];
		m_viewp[2] = viewport[2];
		m_viewp[3] = viewport[3];
	}
	v1nil app_wnd::set_title(cstr_t window_title) {
		m_title = window_title;
		::SetWindowTextA(m_handle, window_title);
	}
	v1nil app_wnd::set_opacity(v1f opacity) {
		opacity = opacity > 1.0f ? 1.0f : opacity < 0.1f ? 0.1f : opacity;
		m_opacity = opacity;
		DWORD style_flags = ::GetWindowLongW(m_handle, GWL_EXSTYLE);
		if (opacity < 1.0f) {	// get window style and add "layered attribute" to it
			style_flags |= WS_EX_LAYERED;
			::SetWindowLongW(m_handle, GWL_EXSTYLE, style_flags);
			::SetLayeredWindowAttributes(m_handle, NW_NULL, static_cast<byte_t>(255.0f * opacity), LWA_ALPHA);
		}
		else {  // get rid of transparency
			style_flags &= ~WS_EX_LAYERED;	// "&~0b0010" == get all bits except this one
			::SetWindowLongW(m_handle, GWL_EXSTYLE, style_flags);
		}
	}
	v1nil app_wnd::set_enabled(v1bit is_enabled) {
		if (m_is_enabled == is_enabled) { return; }
		m_is_enabled = is_enabled;
		if (is_enabled) { ::EnableWindow(m_handle, NW_TRUTH); }
		else { ::EnableWindow(m_handle, NW_FALSE); }
	}
	v1nil app_wnd::set_focused(v1bit is_focused) {
		if (m_is_focused == is_focused) { return; }
		m_is_focused = is_focused;
		if (is_focused) { ::SetFocus(m_handle); }
		else { ::SetFocus(NW_NULL); }
	}
	v1nil app_wnd::set_hovered(v1bit is_hovered) {
		if (m_is_hovered == is_hovered) { return; }
		if (is_hovered) { }
	}
	v1nil app_wnd::set_icon(const gfx_img& img) {
	}
	v1nil app_wnd::set_callback(const event_callback& event_proc) {
		m_event_proc = event_proc;
	}
	// --==<core_methods>==--
	v1nil app_wnd::update()
	{
		// if the message queue is empty - thread can be blocked by GetMessage()
		// if there is 0 - we've got a quit; can be also -1
		if (m_msg.message != WM_QUIT) {
			// if there is false - we don't have a message
			if (::PeekMessage(&m_msg, m_handle, 0u, 0u, PM_REMOVE)) {
				::TranslateMessage(&m_msg);	// make WM_CHAR/WM_SYSCHAR messages
				::DispatchMessage(&m_msg);	// send all the messages into window procedure
			}
		}
	}
	v1bit app_wnd::remake()
	{
		NW_CHECK(NW_TRUTH, "", return NW_FALSE);

		static TCHAR class_name[256];
		OemToChar(get_title(), class_name);
		m_class.lpszClassName = class_name;
		// register a window class to create a window;
		m_class.lpszMenuName = NW_NULL;
		m_class.lpfnWndProc = NW_NULL;
		// GetModuleHandle(NULL) is the current application;
		m_class.hInstance = GetModuleHandle(NW_NULL);
		m_class.hIcon = LoadIcon(m_class.hInstance, MAKEINTRESOURCE(NW_APP_ICON1));
		m_class.hIconSm = LoadIcon(m_class.hInstance, MAKEINTRESOURCE(NW_APP_ICON1));
		m_class.hCursor = LoadCursor(m_class.hInstance, MAKEINTRESOURCE(NW_APP_CURSOR0));
		m_class.hbrBackground = NW_NULL;
		m_class.cbClsExtra = NW_NULL;
		m_class.cbWndExtra = NW_NULL;
		m_class.cbSize = sizeof(WNDCLASSEX);
		// everything is left is:
		// ->set up window class procedure;
		// ->set up window class style;
		// ->create window handle;
		return NW_TRUTH;
	}

	// --==</core_methods>==--
	// --==<impl_methods>==--
	LRESULT WINAPI app_wnd::event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg != WM_NCCREATE) { return ::DefWindowProc(hwnd, msg, wparam, lparam); }

		CREATESTRUCT* crtst = reinterpret_cast<CREATESTRUCT*>(lparam);
		app_wnd* nw_app_window = reinterpret_cast<app_wnd*>(crtst->lpCreateParams);

		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nw_app_window));
		::SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(app_wnd::event_proc_static));

		return app_wnd::event_proc_static(hwnd, msg, wparam, lparam);
	}
	LRESULT WINAPI app_wnd::event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		return reinterpret_cast<app_wnd*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA))->event_proc(hwnd, msg, wparam, lparam);
	}
	LRESULT inline WINAPI app_wnd::event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		switch (msg) {
		// events_of_application
		case WM_CREATE: {
			return 0l;
			break;
		}
		case WM_DESTROY: {
			return 0l;
			break;
		}
		case WM_QUIT: {
			return 0l;
			break;
		}
		case WM_PAINT: {
			return 0l;
			break;
		}
		case WM_ERASEBKGND: {
			return 0l;
			break;
		}
		// events_of_keyboard
		// events_of_mouse
		// events_of_window
		case WM_SIZE: {
			set_size_xy({ LOWORD(lparam), HIWORD(lparam) });
			event_t wnd_evt = event_t(NW_EVTYPE_WINDOW_SIZED, get_size_x(), get_size_y());
			m_event_proc(wnd_evt);
			PostMessage(m_handle, WM_PAINT, 0, 0);
			return 0l;
			break;
		}
		case WM_MOVE: {
			set_coord_xy(v2u{ LOWORD(lparam), HIWORD(lparam) });
			event_t wnd_evt = event_t(NW_EVTYPE_WINDOW_MOVED, get_coord_x(), get_coord_y());
			m_event_proc(wnd_evt);
			PostMessage(m_handle, WM_PAINT, NW_NULL, NW_NULL);
			return 0l;
			break;
		}
		case WM_SETFOCUS: {		// wparam is the last window was focused, lParam is not used
			event_t wnd_evt(NW_EVTYPE_WINDOW_FOCUS, NW_TRUTH);
			m_event_proc(wnd_evt);
			set_focused(NW_TRUTH);
			return 0l;
			break;
		}
		case WM_CLOSE: {
			event_t wnd_evt(NW_EVTYPE_WINDOW_CLOSE, NW_TRUTH);
			m_event_proc(wnd_evt);
			::PostQuitMessage(NW_NULL);
			return 0l;
			break;
		}
		case WM_KILLFOCUS: {	// wparam is the next window will be focused, lParam is not used
			set_focused(NW_FALSE);
			event_t wnd_evt = event_t(NW_EVTYPE_WINDOW_FOCUS, NW_FALSE);
			m_event_proc(wnd_evt);
			return 0l;
			break;
		}
		default: break;
		}
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	// --==</impl_methods>==--
}
#	endif	// WAPI_WIN
#endif	// NW_WAPI