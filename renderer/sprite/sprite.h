#pragma once
#include <misc/vector.h>
#include <d3d9.h>
#include <d3dx9.h>

class c_sprite {
private:
	LPDIRECT3DTEXTURE9 m_texture;
	bool m_began;
	int m_width, m_height;
public:
	~c_sprite() {
		this->release();
	}

	LPD3DXSPRITE m_sprite;
	
	template <typename t>
	void setup(IDirect3DDevice9* device, t texture_bytes, int size, int width, int height) {
		this->m_width = width;
		this->m_height = height;
		D3DXCreateTextureFromFileInMemoryEx(device, (void*)texture_bytes, size, width, height, 0, D3DPOOL_DEFAULT, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_RGBA(0, 0, 0, 255), NULL, NULL, &m_texture);
		D3DXCreateSprite(device, &m_sprite);
	}

	void release();
	void draw(mango::Vec2i pos, int alpha = 255, float rotation = 0.f, float scale = 1.f);
};