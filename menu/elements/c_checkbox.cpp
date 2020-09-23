#include "../../renderer/renderer.h"
#include "../../sdk/input/input.h"
#include "../../sdk/easing/easing.h"
#include "../../globals.h"
#include "../c_menu.h"
#include "../c_container.h"
#include "./c_checkbox.h"
#include "./c_group.h"

mango::Vec2i c_checkbox::calc_pos_size() {
	const auto text_size = renderer::fonts::checkbox.get_text_size(this->m_name);

	switch (this->m_parent->get_type()) {
		case c_element_types::tab:
		case c_element_types::subtab:
			this->m_size = { 20 + 10 + text_size[0], 20 };
			return { 30 + text_size[0] + 25, 0 }; // horizontal
			break;
		case c_element_types::group:
			this->m_size = { 18 + 10 + text_size[0], 18 };
			return { 0, 18 + 15 }; // vertical
			break;
	}

	return { 0, 30 }; // default
}

bool c_checkbox::update() {
	if (this->m_value)
		this->m_timer = min(this->m_timer + (globals::frame_time * 2.f), 1.f);
	else
		this->m_timer = max(this->m_timer - (globals::frame_time * 2.f), 0.f);

	this->m_alpha = uint8_t(easing::blend_in_out(m_timer) * 255);

	const auto text_size = renderer::fonts::checkbox.get_text_size(this->m_name);

	switch (this->m_parent->get_type()) {
		case c_element_types::tab:
		case c_element_types::subtab:
			if (input::in_bounds(this->m_pos, { 20 + 10 + text_size[0], 20 })) {
				this->m_hovered = true;

				if (input::get_key_state(VK_LBUTTON) == keystate::pressed) this->m_value = !this->m_value;
			} else this->m_hovered = false;
			break;
	}
	
	return true;
}

void c_checkbox::draw() {
	if (!this->m_viewable) return;

	const auto text_size = renderer::fonts::checkbox.get_text_size(this->m_name);

	switch (this->m_parent->get_type()) {
		case c_element_types::tab:
		case c_element_types::subtab:
			renderer::filled_rect(this->m_pos, { 20, 20 }, mango::rgba8{ 50, 50, 50 });
			renderer::fonts::checkbox.draw(this->m_name, { this->m_pos[0] + 20 + 10, this->m_pos[1] }, mango::rgba8{ 235, 235, 235 }, false);
			
			renderer::filled_rect(this->m_pos, { 20, 20 }, this->m_parent_menu->m_style.secondary(m_alpha));
			break;
		case c_element_types::group:
			renderer::fonts::group_element.draw(this->m_name, this->m_pos, mango::rgba8{ 220, 220, 220 }, false);
			renderer::filled_rect({ ((c_group*)this->m_parent)->m_body_pos[0] + ((c_group*)this->m_parent)->m_body_size[0] - 5 - 20, this->m_pos[1] }, { 20, 20 }, mango::rgba8{ 35, 35, 35 });
			//renderer::filled_rect(this->m_pos, { 20, 20 }, this->m_parent_menu->m_style.secondary(m_alpha));
			break;
	}
}