#pragma once
#include <misc/vector.h>
#include <misc/color.h>
#include "font/font.h"
#include "sprite/sprite.h"
#include "textures/textures.h"

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

namespace renderer {
	inline IDirect3DDevice9* device;
	inline IDirect3DStateBlock9* state_block;
	inline IDirect3DVertexDeclaration9* vert_dec;
	inline IDirect3DVertexShader9* vert_shader;

	inline bool initialized;
	inline std::vector<font*> font_arr;
	inline float alpha_multiplier = 1.f;

	namespace fonts {
		inline font main("Tahoma", 22, 500, true);
		inline font esp_name("Tahoma", 14, 500, true);
		inline font weapon_name("Tahoma", 12, 500, true);

		inline font menu_title("Segoe UI Semilight", 26, 600, true);
		inline font menu_title_bold("Segoe UI Bold", 26, 900, true);
		inline font tab("Product Sans", 22, 550, true);
		inline font subtab("Segoe UI Semibold", 20, 600, true);
		inline font checkbox("Segoe UI Semilight", 20, 500, true);
		inline font group_element("Segoe UI Semilight", 20, 500, true);
		inline font group_title("Segoe UI Bold", 18, 500, true);
		inline font group_bar("Segoe UI Semibold", 16, 600, true);
	}

	namespace textures {
		inline c_sprite* onion_pic = new c_sprite( );
	}

	void init(IDirect3DDevice9* p_device);
	void release();
	void restore(IDirect3DDevice9* p_device);
	void set_render_states();

	void set_alpha_mutliplier(float multiplier);
	float get_alpha_mutliplier();

	void set_viewport(D3DVIEWPORT9 vp);
	D3DVIEWPORT9 get_viewport();

	mango::Vec2i get_screen_size();

	void begin();
	void end();

	// drawing functions
	void rect(mango::Vec2i pos, const mango::Vec2i& size, const mango::rgba8& color);
	void filled_rect(const mango::Vec2i& pos, const mango::Vec2i& size, const mango::rgba8& color);
	void gradient(const mango::Vec2i& pos, const mango::Vec2i& size, const mango::rgba8& color1, const mango::rgba8& color2, bool horizontal = true);
	void line(const mango::Vec2i& pos, const mango::Vec2i& pos2, const mango::rgba8& color);
	void circle(const mango::Vec2i& pos, const int& radius, const int& percent, const mango::rgba8& color);
}