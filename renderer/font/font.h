#pragma once
#include <d3d9.h>
#include <d3dx9core.h>
#include <string>

enum font_flags {
	font_centered = (1 << 1),
	font_dropshadow = (1 << 2),
	font_outline = (1 << 3),
};

class font {
private:
	void draw_internal(const std::string& text, const mango::Vec2i& pos, const mango::rgba8& color, int flags);

	std::string name;
	int height;
	unsigned int weight;
	bool anti_aliased = false;

public:
	font(std::string name, int height, unsigned int weight, bool anti_aliased) {
		this->name = name;
		this->height = height;
		this->weight = weight;
		this->anti_aliased = anti_aliased;
	}
	font(std::string name, int height, unsigned int weight) {
		this->name = name;
		this->height = height;
		this->weight = weight;
	}

	IDirect3DDevice9* device;
	LPD3DXFONT p_font;

	void setup(IDirect3DDevice9* p_device) {
		this->device = p_device;
	}

	void init() {
		D3DXCreateFontA(device, height, 0, weight, 0, 0, ANSI_CHARSET, OUT_RASTER_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH, name.data(), &p_font);
	}

	void release() {
		p_font->Release();
	}

	void draw(const std::string& text, mango::Vec2i pos, mango::rgba8 color, bool center_width = true, bool center_height = false, int flags = 0) {
		if (flags & font_dropshadow) {
			draw_internal(text, { pos[0] + 1, pos[1] + 1 }, mango::rgba8{ 0, 0, 0, color[3] }, flags);
		}

		if (flags & font_outline) {
			for (int x = -1; x <= 1; x++) for (int y = -1; y <= 1; y++) draw_internal(text, mango::Vec2i(pos[0] + x, pos[1] + y), mango::rgba8{ 0, 0, 0, color[3] }, flags);
		}

		if (center_width || center_height) {
			auto size = this->get_text_size(text);

			if (center_width) pos[0] -= size[0] / 2;
			if (center_height) pos[1] -= size[1] / 2;
		}

		draw_internal(text, pos, color, flags);
	}

	mango::Vec2i get_text_size(const std::string& text) {
		RECT rect;
		p_font->DrawTextA(0, text.c_str(), text.size(), &rect, DT_CALCRECT, 0);
		return mango::Vec2i{ int(rect.right - rect.left), int(rect.bottom - rect.top) };
	}
};