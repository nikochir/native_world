#include "nw_cmd_pch.hpp"
#include "nw_cmd_gui.h"
#if (defined NW_WAPI)
#include "core/nw_cmd_engine.h"
namespace NW
{
	cmd_widget::cmd_widget(cmd_engine& engine) :
		m_engine(&engine),
		m_name("cwidget"),
		coord{ 0, 0 }, size{ 10, 5 }, padding{ 1, 1, 1, 1 },
		pxl_default{ PXT_SOLID, COLOR_BG_4 | COLOR_FG_8 }, pxl_disable{ PXT_SOLID, COLOR_BG_1 | COLOR_FG_4 },
		pxl_focus{ PXT_SOLID, COLOR_BG_8 | COLOR_FG_12 }, pxl_action{ PXT_SOLID, COLOR_BG_8 | COLOR_FG_12 },
		pxl_draw{ pxl_default },
		m_state(WGS_DEFAULT),
		m_action{ []()->v1u { return WGS_DEFAULT; } }
	{
		m_name = m_name + "_" + std::to_string(get_rand(0u, 2u << 8u));
	}
	cmd_widget::~cmd_widget() { }
	// --setters
	void cmd_widget::set_name(cstr_t name) { m_name = name; }
	void cmd_widget::set_action(const action& act) { m_action = act; }
}

namespace NW
{
	cmd_menu_item::cmd_menu_item(cmd_engine& engine) :
		cmd_widget(engine) { }
	cmd_menu_item::~cmd_menu_item() { }

	// --setters
	// --core_methods
	void cmd_menu_item::on_draw()
	{
		v4s draw_rect;
		draw_rect[0] = coord[0] + padding[0];
		draw_rect[1] = coord[1] + padding[1];
		draw_rect[2] = coord[0] - padding[2] + size[0];
		draw_rect[3] = coord[1] - padding[3] + size[1];
		m_engine->draw_rect(draw_rect[0], draw_rect[1],
			draw_rect[2], draw_rect[3], pxl_draw);
		m_engine->draw_bytes(
			draw_rect[0], draw_rect[1], draw_rect[2], draw_rect[3],
			static_cast<cmd_colors>(pxl_draw.Attributes & 0xf0), &m_name[0], strlen(&m_name[0])
		);
	}
	void cmd_menu_item::on_state(cmd_widget_states state)
	{
		if (m_state == WGS_DISABLE && state != WGS_ENABLE) { return; }
		switch (state) {
		case WGS_ENABLE:
			pxl_draw = pxl_default;
			break;
		case WGS_DISABLE:
			pxl_draw = pxl_disable;
			break;
		case WGS_ACT:
			pxl_draw = pxl_action;
			m_action();
			break;
		case WGS_DEACT:
			pxl_draw = pxl_default;
			break;
		case WGS_FOCUS:
			pxl_draw = pxl_focus;
			break;
		case WGS_DEFOCUS:
			pxl_draw = pxl_default;
			break;
		default: break;
		}
		m_state = state;
	}
}
#endif	// NW_WAPI