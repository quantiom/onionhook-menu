#include <misc/logger.h>

#include "../renderer/renderer.h"
#include "../sdk/input/input.h"
#include "../sdk/easing/easing.h"
#include "../sdk/interfaces/interfaces.h"
#include "../globals.h"
#include "c_menu.h"

bool c_menu::update() {
	if (input::get_key_state(VK_INSERT) == keystate::pressed) this->m_open = !this->m_open;

	// easing timer
	if (this->m_open)
		this->m_timer = float(min(this->m_timer + (globals::frame_time * 1.5f), 1.f));
	else
		this->m_timer = float(max(this->m_timer - (globals::frame_time * 2.f), 0.f));

	// fading and moving up/down
	this->alpha_multiplier = float(easing::blend_in_out(this->m_timer));
	this->m_anim_y_pos = int(easing::out_quint(this->m_timer / 2.f) * this->m_pos[1]);

	if (input::get_key_state(VK_LBUTTON) == keystate::released) this->m_dragging = false;

	if (!this->m_open) return true;

	// dragging (the 60 = the title bar)
	if (input::get_key_state(VK_LBUTTON) == keystate::pressed && input::in_bounds({ this->m_pos[0], this->m_anim_y_pos }, { this->m_size[0], 60 }))
		this->m_dragging = true;

	// no vector operators or nothing like dat
	if (this->m_dragging) this->m_pos = { this->m_pos[0] + input::get_mouse_offset()[0], this->m_pos[1] + input::get_mouse_offset()[1] };

	return true;
}

void c_menu::draw() {
	// check if open
	if (!this->m_open && !this->alpha_multiplier) return;

	// update the style (basically just updates the bar color)
	this->update_style();

	// save old alpha multiplier, and set it to the fading one
	auto original_alpha_multiplier = renderer::get_alpha_mutliplier();
	renderer::set_alpha_mutliplier(this->alpha_multiplier);

	// title bar = 60px
	// tab bar = 45px
	// = y + 105px
	renderer::filled_rect({ this->m_pos[0], this->m_anim_y_pos + 105 }, { this->m_size[0], this->m_size[1] - 105 }, this->m_style.main);
	
	// gradient bar with title and main tabs
	renderer::gradient({ this->m_pos[0], this->m_anim_y_pos }, { this->m_size[0], 60 }, this->m_style.main, this->m_style.secondary);
	
	// onion picture / clip it
	const auto original_viewport = renderer::get_viewport();
	renderer::set_viewport({ DWORD(this->m_pos[0]), DWORD(this->m_anim_y_pos), DWORD(this->m_size[0]), 60, 0.f, 1.f });
	renderer::textures::onion_pic->draw({ this->m_pos[0] - 15, this->m_anim_y_pos - 10 }, 65, -20.f, 1.5f);
	renderer::set_viewport(original_viewport);

	// title
	const auto title_text_size = renderer::fonts::menu_title.get_text_size(this->m_name);
	const auto bold_title_size = renderer::fonts::menu_title_bold.get_text_size(this->m_bold_name);
	
	renderer::fonts::menu_title_bold.draw(this->m_bold_name, { this->m_pos[0] + 40, this->m_anim_y_pos + 15 }, mango::rgba8{ 255, 255, 255 }, false);
	renderer::fonts::menu_title.draw(this->m_non_bold_name, { this->m_pos[0] + 40 + bold_title_size[0], m_anim_y_pos + 15 }, mango::rgba8{ 255, 255, 255 }, false);

	// line under title			
	renderer::filled_rect({ this->m_pos[0] + 40 + (title_text_size[0] / 8), this->m_anim_y_pos + 15 + title_text_size[1] + 3 }, { title_text_size[0] - ((title_text_size[0] / 8) * 2), 3}, this->m_style.bar(uint8_t(abs(sin(globals::get_timestamp() * 2)) * 255)));
	//renderer::filled_rect({ this->m_pos[0] + 20 + (title_text_size[0] / 8), this->m_anim_y_pos + 17 + title_text_size[1] + 2 }, { title_text_size[0] - (title_text_size[0] / 8) * 2, 3 }, this->m_style.bar);

	// draw tabs
	const auto tab_padding = 10;
	mango::Vec2i start_tab_pos = { this->m_pos[0] + this->m_size[0] - 20, this->m_anim_y_pos + 20 };
	auto offset_from_last = 0;

	// assuming only tabs were added to the menu, pretty bad
	for (auto& tab : this->m_elements) {
		mango::Vec2i tab_text_size = renderer::fonts::tab.get_text_size(tab->m_name);

		// set parents
		tab->m_parent_menu = this;
		tab->m_parent = this;

		// set position and size
		tab->m_pos = { start_tab_pos[0] - (tab_padding * 2) - tab_text_size[0] - offset_from_last, start_tab_pos[1] };
		tab->m_size = { tab_text_size[0], tab_text_size[1] };

		// update and draw
		tab->update();
		tab->draw();

		// add to the offset
		offset_from_last += (tab_padding * 2) + tab_text_size[0];
	}

	// restore alpha multiplier
	renderer::set_alpha_mutliplier(original_alpha_multiplier);
}