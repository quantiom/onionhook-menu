#include "../../renderer/renderer.h"
#include "../../sdk/input/input.h"
#include <misc/logger.h>
#include "../c_menu.h"
#include "c_subtab.h"

bool c_subtab::update() {
	const auto text_size = renderer::fonts::subtab.get_text_size(this->m_name);

	this->m_hovered = input::in_bounds({ this->m_pos[0] + (m_tab_size / 2) - (text_size[0] / 2), this->m_pos[1] }, { text_size[0], 45 });

	if (input::get_key_state(VK_LBUTTON) == keystate::pressed && input::in_bounds({ this->m_pos[0], this->m_pos[1] }, { m_tab_size, 45 })) {
		std::vector<std::shared_ptr<c_subtab>> subtabs;

		for (const auto& element : this->m_parent->m_elements) if (element->get_type() == c_element_types::subtab) subtabs.push_back(std::reinterpret_pointer_cast<c_subtab>(element));
		for (const auto& subtab : subtabs) subtab->m_active = false;

		this->m_active = true;

		subtabs.clear();
	}

	return true;
}

void c_subtab::draw() {
	if (this->m_active) {
		renderer::gradient({ this->m_parent_menu->m_pos[0], this->m_parent_menu->m_anim_y_pos + 60 + 41 }, { this->m_parent_menu->m_size[0], 4 }, this->m_parent_menu->m_style.secondary, this->m_parent_menu->m_style.darker_secondary);
		
		const auto original_viewport = renderer::get_viewport();
		
		renderer::set_viewport({ DWORD(this->m_pos[0]), DWORD(this->m_pos[1]), DWORD(this->m_tab_size), 41, 0.f, 1.f });
		renderer::gradient({ this->m_parent_menu->m_pos[0], this->m_parent_menu->m_anim_y_pos + 60 }, { this->m_parent_menu->m_size[0], 41 }, this->m_parent_menu->m_style.secondary, this->m_parent_menu->m_style.darker_secondary);

		renderer::set_viewport(original_viewport);
	} else

	// 41 because the bar is 4 px
	renderer::filled_rect(this->m_pos, { this->m_tab_size, 41 }, { uint8_t(this->m_parent_menu->m_style.main[0] * 0.9f), uint8_t(this->m_parent_menu->m_style.main[1] * 0.9f), uint8_t(this->m_parent_menu->m_style.main[2] * 0.9f) });
	
	// text
	const auto text_size = renderer::fonts::subtab.get_text_size(this->m_name);
	renderer::fonts::subtab.draw(this->m_name, { this->m_pos[0] + (m_tab_size / 2), this->m_pos[1] + (text_size[1] / 2) }, this->m_active ? mango::rgba8{ 255, 255, 255 } : (this->m_hovered ? mango::rgba8{ 210, 210, 210 } : mango::rgba8{ 120, 120, 120 }));

	if (this->m_active && !this->m_elements.empty()) {
		this->draw_elements({ this->m_parent_menu->m_pos[0] + 15, this->m_parent_menu->m_anim_y_pos + 60 + 45 + 15 });
	}
}