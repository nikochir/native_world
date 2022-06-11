#include "nw_app_pch.hpp"
#include "nw_app_wnd_core.h"
#if (defined NW_WAPI)
#	include "core/nw_app_engine.h"
#	include "nw_app_wnd_sub.h"
#	if (NW_WAPI & NW_WAPI_WIN)
#		include <shellapi.h>
namespace NW
{
	app_wnd_core::app_wnd_core() :
		app_wnd()
	{
	}
	app_wnd_core::~app_wnd_core()
	{
	}
	// --setters
	// --==<core_methods>==--
	v1nil app_wnd_core::update()
	{
		app_wnd::update();
	}
	v1bit app_wnd_core::remake()
	{
		NW_CHECK(app_wnd::remake(), "remake error!", return NW_FALSE);

		// register a window class to create a window; ModuleHandle is the current application;
		m_class.lpfnWndProc = app_wnd_core::event_proc_init;
		m_class.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
		NW_CHECK(::RegisterClassEx(&m_class), "failed class register", return NW_FALSE);
		// WS_OVERLAPPEDWINDOW is the same as:
		// (WS_OVERLAPPED | WS_CAPTION | WS_SYS_MENU | WS_THICKFRAME | WS_MINIMIZE_BOX | WS_MAXIMIZE_BOX);
		// WS_TILEDWINDOW is the same as:
		// (WS_TILED | WS_CAPTION | WS_SYS_MENU | WS_THICKFRAME | WS_MINIMIZE_BOX | WS_MAXIMIZE_BOX);
		DWORD style = NW_NULL;
		// control styles;
		style |= WS_CAPTION;		// title bar;
		style |= WS_SYSMENU;		// must be used with WS_CAPTION;
		style |= WS_MINIMIZEBOX;	// button sizing;
		style |= WS_MAXIMIZEBOX;	// button sizing;
		style |= WS_SIZEBOX;		// arrow-border sizing;
		//style |= WS_HSCROLL;		// horizontal scrollbar;
		//style |= WS_VSCROLL;		// vertical scrollbar;
		// view styles;
		style |= WS_OVERLAPPED;		// title bar and border; same as WS_TILED;
		style |= WS_TILED;			// title bar and border; same as WS_OVERLAPPED;
		style |= WS_BORDER;			// thin-line border;
		style |= WS_THICKFRAME;		// ;
		// initial styles;
		style |= WS_VISIBLE;		// initialy visible
		// style |= WS_DISABLED;	// initialy disabled;
		// style |= WS_ICONIC;		// initialy minimized;
		style |= WS_MAXIMIZE;		// initialy maximized;
		// extended styles
		// WS_EX_OVERLAPPEDWINDOW is the same as
		// (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE);
		// WS_EX_PALETTEWINDOW is the same as
		// (WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
		DWORD style_ex = NW_NULL;
		style_ex |= WS_EX_APPWINDOW;	// force top window onto the taskbar whe it is visible;
		style_ex |= WS_EX_ACCEPTFILES;	// drag-drop files;
		// style_ex |= WS_EX_CONTEXTHELP;	// add an icon which makes WM_HELP message and changes the cursor to question mark;
		// style_ex |= WS_EX_LAYERED;		// cannot be used with CS_OWNDC or CS_CLASSDC;
		style_ex |= WS_EX_LEFT;			// generic left-aligned properties;
		//style_ex |= WS_EX_RIGHT;		// generic right-aligned properties;
		// style_ex |= WS_EX_TOOLWINDOW;	// the window becomes a floating toolbar;
		// style_ex |= WS_EX_TRANSPARENT;	// the window should not be draw until siblings beneath it;
		style_ex |= WS_EX_WINDOWEDGE;	// the window has a border with a raised edge
		// adjust sizes;
		RECT wnd_rect = { 100, 100, 100 + static_cast<LONG>(get_size_x()), 100 + static_cast<LONG>(get_size_y()) };
		::AdjustWindowRect(&wnd_rect, style, FALSE);
		// create the window itself;
		m_handle = CreateWindowEx(
			style_ex,
			m_class.lpszClassName, m_class.lpszClassName,
			style,
			wnd_rect.left, wnd_rect.top,
			wnd_rect.right - wnd_rect.left, wnd_rect.bottom - wnd_rect.top,
			NULL, NULL,
			m_class.hInstance,
			this
		);
		NW_CHECK(m_handle, "no handle!", return NW_FALSE);
		// do some stuff with the window;
		::ShowWindow(m_handle, SW_SHOWDEFAULT);
		::UpdateWindow(m_handle);
		// register mouse raw input device;
		RAWINPUTDEVICE raw_dvc;
		raw_dvc.usUsagePage = 0x01; // mouse page bit;
		raw_dvc.usUsage = 0x02;     // mouse usage bit;
		raw_dvc.dwFlags = NW_NULL;  // additional param;
		raw_dvc.hwndTarget = NW_NULL; // we don't really need a target;
		NW_CHECK(::RegisterRawInputDevices(&raw_dvc, 1u, sizeof(raw_dvc)), "failed raw register!", return NW_FALSE);
		
		return NW_TRUTH;
	}
	// --==</core_methods>==--
	// --==<impl_methods>==--
	LRESULT WINAPI app_wnd_core::event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg != WM_NCCREATE) { return ::DefWindowProc(hwnd, msg, wparam, lparam); }

		CREATESTRUCT* crtst = reinterpret_cast<CREATESTRUCT*>(lparam);
		app_wnd* nw_app_window = reinterpret_cast<app_wnd*>(crtst->lpCreateParams);

		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nw_app_window));
		::SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(app_wnd_core::event_proc_static));

		return app_wnd_core::event_proc_static(hwnd, msg, wparam, lparam);
	}
	LRESULT WINAPI app_wnd_core::event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (gui_wapi_event_proc(hwnd, msg, wparam, lparam) == TRUE) { return 0l; }
		return reinterpret_cast<app_wnd_core*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA))->event_proc(hwnd, msg, wparam, lparam);
	}
	inline LRESULT WINAPI app_wnd_core::event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		switch (msg) {
		// events_of_application
		{
		case WM_CREATE: { return 0l; break; }
		case WM_DESTROY: { return 0l; break; }
		case WM_QUIT: { return 0l; break; }
		case WM_PAINT: { return 0l; break; }
		case WM_ERASEBKGND: { return 0l; break; }
		case WM_DROPFILES: {	// wparam is HDROP; lparam is not used;
			TCHAR str_buf[MAX_PATH]{ 0 };
			HDROP drop_info = reinterpret_cast<HDROP>(wparam);
			UINT drop_result = DragQueryFile(drop_info, 0u, str_buf, MAX_PATH); // we can retrieve the file path;
			NW_CHECK(drop_result, "invalid result!", return NW_NULL);
			DragFinish(drop_info);	// free that resources that windows has just taken to help us;
			DWORD attribs = GetFileAttributes(str_buf);
			if (attribs & FILE_ATTRIBUTE_ARCHIVE) {
				std::cout << "a file or directory that is an archieve file or directory;" << std::endl;
				std::cout << "usually used to mark files for backup or removal;" << std::endl;
			}
			if (attribs & FILE_ATTRIBUTE_COMPRESSED) {
				std::cout << "all file data is compressed;" << std::endl;
			}
			if (attribs & FILE_ATTRIBUTE_DEVICE) {
				std::cout << "the value is reserved for system use;" << std::endl;
			}
			if (attribs & FILE_ATTRIBUTE_DIRECTORY) {
				std::cout << "the file identifies a directory;" << std::endl;
			}
			if (attribs & FILE_ATTRIBUTE_EA) {
				std::cout << "the value is reserved for system use;" << std::endl;
			}
			if (attribs & FILE_ATTRIBUTE_VIRTUAL) {
				std::cout << "the value is reserved for system use;" << std::endl;
			}
			if (attribs & FILE_ATTRIBUTE_NORMAL) {
				std::cout << "a file does not have other attributes set;" << std::endl;
			}
			m_event_proc(iop_event_app_t(NW_EVTYPE_APPLIC_DROPF, str_buf));
			return 0l;
			break;
		}
		}
		// events_of_input
		{
		case WM_INPUT: {
			UINT in_data_size = 0u;
			if (::GetRawInputData(	//  fill the number of bytes variable for the data;
				reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, NULL, &in_data_size, sizeof(RAWINPUTHEADER)) == -1) {
				break;
			}
			// error with NULL returns -1, but we just bail that;
			m_raw_buf.resize(in_data_size);
			if (::GetRawInputData(	// fill data buffer in this time;
				reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, &m_raw_buf[0], &in_data_size, sizeof(RAWINPUTHEADER)) != in_data_size) {
				break;
			}
			// error with reading returns number of the read bytes, bail it again;
			// raw input data processing;
			const RAWINPUT& raw_in = reinterpret_cast<const RAWINPUT&>(m_raw_buf[0]);
			// we are interested in the header and data;
			if (raw_in.header.dwType == RIM_TYPEMOUSE) {
				if (raw_in.data.mouse.lLastX != NW_NULL || raw_in.data.mouse.lLastY != NW_NULL) {
					m_event_proc(iop_cursor_t::event_t(NW_EVTYPE_CURSOR_MOVED, raw_in.data.mouse.lLastX, raw_in.data.mouse.lLastY));
				}
			}
			break;
		}
		}
		// events_of_mouse
		{
		case WM_MOUSEMOVE: {
			POINTS coord = MAKEPOINTS(lparam);
			m_event_proc(iop_cursor_t::event_t(NW_EVTYPE_CURSOR_COORD, coord.x, coord.y));
			return 0l;
			break;
		}
		case WM_MOUSEHWHEEL: {
			m_event_proc(iop_cursor_t::event_t(NW_EVTYPE_CURSOR_SCROL, GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA, NW_NULL));
			return 0l;
			break;
		}
		case WM_MOUSEWHEEL: {
			m_event_proc(iop_cursor_t::event_t(NW_EVTYPE_CURSOR_SCROL, NW_NULL, GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA));
			return 0l;
			break;
		}
		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
		{
			iop_cursor_t::code_t msb_code = NW_NULL;
			if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { msb_code = NW_CURCODE_LEFT; }
			if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { msb_code = NW_CURCODE_RIGT; }
			if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { msb_code = NW_CURCODE_MIDL; }
			if (::GetCapture() == NW_NULL) { ::SetCapture(m_handle); }
			m_event_proc(iop_cursor_t::event_t(NW_EVTYPE_CURSOR_PRESS, msb_code));
			return 0l;
			break;
		}
		}
		{
		case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP: {
			iop_cursor_t::code_t msb_code = NW_NULL;
			if (msg == WM_LBUTTONUP) { msb_code = NW_CURCODE_LEFT; }
			if (msg == WM_RBUTTONUP) { msb_code = NW_CURCODE_RIGT; }
			if (msg == WM_MBUTTONUP) { msb_code = NW_CURCODE_MIDL; }
			m_event_proc(iop_cursor_t::event_t(NW_EVTYPE_CURSOR_RAISE, msb_code));
			if (::GetCapture() == m_handle) { ::ReleaseCapture(); }
			return 0l;
			break;
		}
		// events_of_keyboard
		case WM_KEYDOWN: case WM_SYSKEYDOWN: {
			m_event_proc(iop_keybod_t::event_t(NW_EVTYPE_KEYBOD_PRESS, wparam));
			return 0l;
			break;
		}
		case WM_KEYUP: case WM_SYSKEYUP: {
			m_event_proc(iop_keybod_t::event_t(NW_EVTYPE_KEYBOD_RAISE, wparam));
			return 0l;
			break;
		}
		case WM_CHAR: case WM_SYSCHAR: {
			m_event_proc(iop_keybod_t::event_t(NW_EVTYPE_KEYBOD_CHART, wparam));
			return 0l;
			break;
		}
		}
		// events_of_window
		{
		case WM_SIZE: {
			set_size_xy({ LOWORD(lparam), HIWORD(lparam) });
			m_event_proc(event_t(NW_EVTYPE_WINDOW_SIZED, get_size_x(), get_size_y()));
			return 0l;
			break;
		}
		case WM_MOVE: {
			set_coord_xy({ LOWORD(lparam), HIWORD(lparam) });
			m_event_proc(event_t(NW_EVTYPE_WINDOW_MOVED, get_coord_x(), get_coord_y()));
			return 0l;
			break;
		}
		case WM_SETFOCUS: {		// wparam is the last window was focused, lParam is not used
			set_focused(NW_TRUTH);
			m_event_proc(event_t(NW_EVTYPE_WINDOW_FOCUS, NW_TRUTH));
			return 0l;
			break;
		}
		case WM_KILLFOCUS: {	// wparam is the next window will be focused, lParam is not used
			set_focused(NW_FALSE);
			m_event_proc(event_t(NW_EVTYPE_WINDOW_FOCUS, NW_FALSE));
			return 0l;
			break;
		}
		case WM_CLOSE: {
			m_event_proc(event_t(NW_EVTYPE_WINDOW_CLOSE, NW_TRUTH));
			::PostQuitMessage(NW_NULL);
			return 0l;
			break;
		}
		case WM_ACTIVATE: {
			m_event_proc(event_t(NW_EVTYPE_WINDOW_ACTIV, NW_TRUTH));
			return 0l;
			break;
		}
		}
		}
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	// --==</impl_methods>==--
}
#	endif	// WAPI_WIN
#endif // NW_WAPI