#include <misc/logger.h>
#include "renderer.h"

void renderer::set_render_states() {
	device->SetVertexShader(nullptr);
	device->SetPixelShader(nullptr);
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetRenderState(D3DRS_ZENABLE, false);

	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	device->SetSamplerState(0ul, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	device->SetSamplerState(0ul, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	device->SetSamplerState(0ul, D3DSAMP_ADDRESSW, D3DTADDRESS_MIRROR);
	device->SetSamplerState(0ul, D3DSAMP_SRGBTEXTURE, 0ul);
}

D3DVIEWPORT9 renderer::get_viewport() {
	D3DVIEWPORT9 viewport{}; device->GetViewport(&viewport);
	return viewport;
}

void renderer::set_alpha_mutliplier(float multiplier) {
	alpha_multiplier = multiplier;
}

float renderer::get_alpha_mutliplier() {
	return alpha_multiplier;
}

void renderer::init(IDirect3DDevice9* p_device) {
	device = p_device;

	font_arr.push_back(&fonts::main);
	font_arr.push_back(&fonts::esp_name);
	font_arr.push_back(&fonts::weapon_name);

	font_arr.push_back(&fonts::menu_title);
	font_arr.push_back(&fonts::menu_title_bold);
	font_arr.push_back(&fonts::tab);
	font_arr.push_back(&fonts::subtab);
	font_arr.push_back(&fonts::checkbox);
	font_arr.push_back(&fonts::group_title);
	font_arr.push_back(&fonts::group_bar);
	font_arr.push_back(&fonts::group_element);

	for (auto font : font_arr) {
		font->setup(device);
		font->init();
	}

	textures::onion_pic->setup(p_device, onion_picture, sizeof(onion_picture), 37, 55);

	initialized = true;
}

void renderer::release() {
	for (auto font : font_arr) {
		font->release();
	}

	//textures::onion_pic->release();
}

void renderer::restore(IDirect3DDevice9* p_device) {
	device = p_device;

	for ( auto font : font_arr ) {
		font->setup( device );
		font->init( );
	}
	
	textures::onion_pic->setup(p_device, onion_picture, sizeof(onion_picture), 37, 55);
}

void renderer::set_viewport(D3DVIEWPORT9 vp) {
	device->SetViewport(&vp);
}

mango::Vec2i renderer::get_screen_size() {
	D3DDEVICE_CREATION_PARAMETERS cparams;
	RECT rect;

	device->GetCreationParameters(&cparams);
	GetWindowRect(cparams.hFocusWindow, &rect);

	return { int(rect.right), int(rect.bottom) };
}

struct vertice {
	float x, y, z, rhw;
	DWORD c;
};

void renderer::rect(mango::Vec2i pos, const mango::Vec2i& size, const mango::rgba8& color) {
	pos[0] -= 1;
	pos[1] -= 1;

	vertice verts[5] = {
		{ float(pos[0]), float(pos[1]), 0.01f, 0.01f, color.get_argb(alpha_multiplier) },
		{ float(pos[0] + size[0]), float(pos[1]), 0.01f, 0.01f, color.get_argb(alpha_multiplier) },
		{ float(pos[0] + size[0]), float(pos[1] + size[1]), 0.01f, 0.01f, color.get_argb(alpha_multiplier) },
		{ float(pos[0]), float(pos[1] + size[1]), 0.01f, 0.01f, color.get_argb(alpha_multiplier) },
		{ float(pos[0]), float(pos[1]), 0.01f, 0.01f, color.get_argb(alpha_multiplier) }
	};

	device->SetTexture(0, nullptr);
	device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &verts, 20);
}

void renderer::filled_rect(const mango::Vec2i& pos, const mango::Vec2i& size, const mango::rgba8& color) {
	vertice verts[4] = {
		{ float(pos[0]), float(pos[1]), 0.01f, 0.01f, color.get_argb(alpha_multiplier) },
		{ float(pos[0] + size[0]), float(pos[1]), 0.01f, 0.01f, color.get_argb(alpha_multiplier) },
		{ float(pos[0]), float(pos[1] + size[1]), 0.01f, 0.01f, color.get_argb(alpha_multiplier) },
		{ float(pos[0] + size[0]), float(pos[1] + size[1]), 0.01f, 0.01f, color.get_argb(alpha_multiplier) }
	};

	device->SetTexture(0, nullptr);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &verts, 20);
}

void renderer::gradient(const mango::Vec2i& pos, const mango::Vec2i& size, const mango::rgba8& color1, const mango::rgba8& color2, bool horizontal) {
	if (!horizontal) {
		vertice verts[4] = {
			{ float(pos[0]), float(pos[1] + size[1]), 0.01f, 0.01f, color1.get_argb(alpha_multiplier) },
			{ float(pos[0]), float(pos[1]), 0.01f, 0.01f, color1.get_argb(alpha_multiplier) },
			{ float(pos[0] + size[0]), float(pos[1] + size[1]), 0.01f, 0.01f, color2.get_argb(alpha_multiplier) },
			{ float(pos[0] + size[0]), float(pos[0]), 0.01f, 0.01f, color2.get_argb(alpha_multiplier) }
		};

		device->SetTexture(0, nullptr);
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &verts, 20);
	}
	else {
		vertice verts[4] = {
			{ float(pos[0]), float(pos[1]), 0.01f, 0.01f, color1.get_argb(alpha_multiplier) },
			{ float(pos[0] + size[0]), float(pos[1]), 0.01f, 0.01f, color2.get_argb(alpha_multiplier) },
			{ float(pos[0]), float(pos[1] + size[1]), 0.01f, 0.01f, color1.get_argb(alpha_multiplier) },
			{ float(pos[0] + size[0]), float(pos[1] + size[1]), 0.01f, 0.01f, color2.get_argb(alpha_multiplier) }
		};

		device->SetTexture(0, nullptr);
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &verts, 20);
	}
}

void renderer::line(const mango::Vec2i& pos, const mango::Vec2i& pos2, const mango::rgba8& color) {
	vertice verts[2] = {
		{ float(pos[0]), float(pos[1]), 0.01f, 0.01f, color.get_argb(alpha_multiplier) },
		{ float(pos2[0]), float(pos2[1]), 0.01f, 0.01f, color.get_argb(alpha_multiplier) }
	};

	device->SetTexture(0, nullptr);
	device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &verts, 20);
}

void renderer::circle(const mango::Vec2i& pos, const int& radius, const int& percent, const mango::rgba8& color) {
	vertice* vertex = new vertice[percent + 1];

	const float pi = D3DX_PI * ((float)percent / 100.f);

	for (int i = 0; i <= percent; i++)
		vertex[i] = { pos[0] + radius * cos(pi * (i / (percent / 2.0f))), pos[1] + radius * sin(pi * (i / (percent / 2.0f))), 0.0f, 1.0f, color.get_argb(alpha_multiplier) };

	device->SetTexture(0, nullptr);
	device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, percent, vertex, 20);
}

void renderer::begin() {
	device->CreateStateBlock(D3DSBT_PIXELSTATE, &state_block);

	device->GetVertexDeclaration(&vert_dec);
	device->GetVertexShader(&vert_shader);

	set_render_states();
}

void renderer::end() {
	state_block->Apply();
	state_block->Release();

	device->SetVertexDeclaration(vert_dec);
	device->SetVertexShader(vert_shader);
}