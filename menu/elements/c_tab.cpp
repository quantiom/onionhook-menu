#include "../../renderer/renderer.h"
#include "../../sdk/input/input.h"
#include "../../globals.h"

#include "../c_menu.h"
#include "./c_tab.h"
#include "./c_subtab.h"

bool c_tab::update() {
	if (input::in_bounds(this->m_pos, this->m_size)) {
		this->m_hovered = true;

		if (input::get_key_state(VK_LBUTTON) == keystate::pressed && !this->m_active) {
			for (auto& element : this->m_parent_menu->m_elements) {
				if (element->get_type() != c_element_types::tab) continue;
				std::reinterpret_pointer_cast<c_tab>(element)->m_active = false;
				this->m_activated_time = globals::get_timestamp();
			}

			this->m_active = true;
		}
	} else this->m_hovered = false;

	return true;
}

void c_tab::draw() {
	const auto text_size = renderer::fonts::tab.get_text_size(this->m_name);
	renderer::fonts::tab.draw(this->m_name, this->m_pos, this->m_active ? mango::rgba8{ 255, 255, 255, 255 } : (this->m_hovered ? mango::rgba8{ 220, 220, 220 } : mango::rgba8{ 180, 180, 180 }), false);

	if (this->m_active) {
		// bar that goes under the tab text
		float timer = min((globals::get_timestamp() - this->m_activated_time) * 2, 1.f);
		renderer::filled_rect({ int(m_pos[0] + (text_size[0] / 2) - (timer * (text_size[0] - (text_size[0] / 4))) / 2), int(m_pos[1] + text_size[1] + 3) }, { int(timer * (text_size[0] - (text_size[0] / 4))), 3, }, this->m_parent_menu->m_style.bar);
	
		std::vector<std::shared_ptr<c_subtab>> subtabs;

		// just incase u are retarded and add elements other than subtabs
		for (const auto& e : this->m_elements) if (e->get_type() == c_element_types::subtab) subtabs.push_back(std::reinterpret_pointer_cast<c_subtab>(e));

		// no dividing by 0
		if (subtabs.empty()) {
			// no subtabs
			renderer::filled_rect({ this->m_parent_menu->m_pos[0], this->m_parent_menu->m_anim_y_pos + 60 }, { this->m_parent_menu->m_size[0], 45 }, this->m_parent_menu->m_style.main);
			this->draw_elements({ this->m_parent_menu->m_pos[0] + 15, this->m_parent_menu->m_anim_y_pos + 60 + 15 });
			return;
		}

		auto tab_size = this->m_parent_menu->m_size[0] / subtabs.size();
		auto last_tab_offset = 0;

		for (size_t i = 0; i < subtabs.size(); i++) {
			auto& subtab = subtabs[i];

			subtab->m_parent_menu = this->m_parent_menu;
			subtab->m_parent = this;

			if (i == subtabs.size() - 1) tab_size += this->m_parent_menu->m_size[0] - last_tab_offset - tab_size;

			subtab->m_pos = { this->m_parent_menu->m_pos[0] + last_tab_offset, this->m_parent_menu->m_anim_y_pos + 60 };
			subtab->m_tab_size = tab_size;

			subtab->update();
			subtab->draw();

			last_tab_offset += tab_size;
		}

		subtabs.clear();
	}
}