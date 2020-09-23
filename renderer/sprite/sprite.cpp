#include "./sprite.h"
#include "../renderer.h"
#define M_PI 3.14159265358979323846 

void c_sprite::release() {
	this->m_texture->Release();
	delete this->m_texture;
	this->m_sprite->Release();
	delete this->m_sprite;
}

void c_sprite::draw(mango::Vec2i pos, int alpha, float rotation, float scale) {
	m_sprite->Begin(D3DXSPRITE_SORT_TEXTURE | D3DXSPRITE_ALPHABLEND);

	D3DXVECTOR3 position;

	position.x = float(pos[0]);
	position.y = float(pos[1]);
	position.z = 0.01f;

	D3DXVECTOR3 scaling(scale, scale, scale);
	D3DXVECTOR3 spriteCentre(this->m_width / 2.f, this->m_height / 2.f, 0);
	D3DXMATRIX matrix;

	D3DXQUATERNION rotation_quaternion;
	D3DXQuaternionIdentity(&rotation_quaternion);

	D3DXVECTOR3 rotation_axis = { 0, 0, 1 };

	D3DXQuaternionRotationAxis(&rotation_quaternion, &rotation_axis, FLOAT((rotation * M_PI) / 180.f));
	D3DXMatrixTransformation(&matrix, NULL, 0, &scaling, &spriteCentre, &rotation_quaternion, &position);

	m_sprite->SetTransform(&matrix);

	m_sprite->Draw(m_texture, NULL, NULL, NULL, D3DCOLOR_RGBA(255, 255, 255, uint8_t(alpha * renderer::alpha_multiplier)));

	m_sprite->End();
}