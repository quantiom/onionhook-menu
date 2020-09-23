#include "../renderer.h"
#include "font.h"

void font::draw_internal(const std::string& text, const mango::Vec2i& pos, const mango::rgba8& color, int flags) {
	RECT rect;
	SetRect(&rect, pos[0], pos[1], pos[0], pos[1]);
	p_font->DrawTextA(NULL, text.c_str(), -1, &rect, ((flags & font_centered) ? DT_CENTER : DT_LEFT) | DT_NOCLIP, color.get_argb(renderer::alpha_multiplier));
}