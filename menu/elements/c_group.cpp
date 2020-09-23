#include "../../renderer/renderer.h"
#include "../../sdk/input/input.h"
#include "c_group.h"
#include "../c_menu.h"

#include <misc/logger.h>

mango::Vec2i c_group::calc_pos_size() {
	this->m_size = { 233, (this->m_parent_menu->m_anim_y_pos + this->m_parent_menu->m_size[1]) - this->m_pos[1] - 25 }; // hardcoded for 3
	return { this->m_size[0] + 35, 0 };
}

bool c_group::update() {
	return true;
}

void c_group::draw() {
	this->m_body_pos = { this->m_pos[0] + 2, this->m_pos[1] + 30 };
	this->m_body_size = { this->m_size[0] - (2 * 2), this->m_size[1] - 30 - 37 };

	this->m_title_pos = this->m_pos;
	this->m_title_size = { this->m_size[0], 30 };

	this->m_bottom_bar_pos = { this->m_body_pos[0], this->m_body_pos[1] + this->m_body_size[1] };
	this->m_bottom_bar_size = { this->m_body_size[0], 37 };

	// body
	renderer::filled_rect(this->m_body_pos, this->m_body_size, mango::rgba8{ 50, 50, 56 });

	// drop shadow for bottom bar
	renderer::filled_rect({ this->m_bottom_bar_pos[0] - 7, this->m_bottom_bar_pos[1] - 8 }, { this->m_bottom_bar_size[0] + 14, this->m_bottom_bar_size[1] + 16 }, mango::rgba8{ 0, 0, 0, 5 });
	renderer::filled_rect({ this->m_bottom_bar_pos[0] - 5, this->m_bottom_bar_pos[1] - 6 }, { this->m_bottom_bar_size[0] + 10, this->m_bottom_bar_size[1] + 12 }, mango::rgba8{ 0, 0, 0, 10 });
	renderer::filled_rect({ this->m_bottom_bar_pos[0] - 3, this->m_bottom_bar_pos[1] - 4 }, { this->m_bottom_bar_size[0] + 6, this->m_bottom_bar_size[1] + 8 }, mango::rgba8{ 0, 0, 0, 15 });
	renderer::filled_rect({ this->m_bottom_bar_pos[0] - 1, this->m_bottom_bar_pos[1] - 2 }, { this->m_bottom_bar_size[0] + 2, this->m_bottom_bar_size[1] + 4 }, mango::rgba8{ 0, 0, 0, 20 });

	// bottom bar
	renderer::filled_rect({ this->m_bottom_bar_pos[0], this->m_bottom_bar_pos[1] + 7 }, { this->m_bottom_bar_size[0], this->m_bottom_bar_size[1] - 7 }, mango::rgba8{ 50, 50, 56 });

	// bottom gradient bar
	auto original_viewport = renderer::get_viewport();
	renderer::set_viewport({ DWORD(this->m_bottom_bar_pos[0]), DWORD(this->m_bottom_bar_pos[1]), DWORD(this->m_bottom_bar_size[0]), DWORD(7), 0.f, 1.f });
	renderer::gradient({ this->m_parent_menu->m_pos[0] + 15, this->m_bottom_bar_pos[1] }, { this->m_parent_menu->m_size[0] - 30, 7 }, this->m_parent_menu->m_style.secondary(200), this->m_parent_menu->m_style.darker_secondary(200));
	renderer::set_viewport(original_viewport);

	// bottom bar text stuff
	{
		static const std::array<std::string, 4> options = { "IMPORT", "UNDO", "REDO", "EXPORT" };
		const auto text_portion_size = int(this->m_bottom_bar_size[0] / options.size());
		auto offset = 0;

		for (size_t i = 0; i < options.size(); i++) {
			const auto option = options[i];
			
			//renderer::filled_rect({ this->m_bottom_bar_pos[0] + offset, this->m_bottom_bar_pos[1] }, { text_portion_size, 37 }, i % 2 ? mango::rgba8{ 255, 255, 255 } : mango::rgba8{ 255, 0, 0 });
			renderer::fonts::group_bar.draw(option, { this->m_bottom_bar_pos[0] + offset + text_portion_size / 2, this->m_bottom_bar_pos[1] + 3 + this->m_bottom_bar_size[1] / 2 }, { 165, 165, 165 }, true, true);

			offset += text_portion_size;
		}
	}

	this->draw_elements({ this->m_body_pos[0] + 5, this->m_body_pos[1] + 10 });

	// drop shadow for title
	renderer::filled_rect({ this->m_pos[0] - 3, this->m_pos[1] }, { this->m_size[0] + 8, 32 }, mango::rgba8{ 0, 0, 0, 5 });
	renderer::filled_rect({ this->m_pos[0] - 3, this->m_pos[1] + 2 }, { this->m_size[0] + 8, 32 }, mango::rgba8{ 0, 0, 0, 10 });
	renderer::filled_rect({ this->m_pos[0] - 3, this->m_pos[1] + 4 }, { this->m_size[0] + 8, 32 }, mango::rgba8{ 0, 0, 0, 15 });
	renderer::filled_rect({ this->m_pos[0] - 3, this->m_pos[1] + 6 }, { this->m_size[0] + 8, 32 }, mango::rgba8{ 0, 0, 0, 20 });

	// clip the gradient and draw the title bar
	original_viewport = renderer::get_viewport();
	renderer::set_viewport({ DWORD(this->m_title_pos[0]), DWORD(this->m_title_pos[1]), DWORD(this->m_title_size[0]), DWORD(this->m_title_size[1]), 0.f, 1.f });
	renderer::gradient({ this->m_parent_menu->m_pos[0] + 15, this->m_pos[1] }, { this->m_parent_menu->m_size[0] - 30, 30 }, this->m_parent_menu->m_style.secondary, this->m_parent_menu->m_style.darker_secondary);
	renderer::set_viewport(original_viewport);

	// title text
	renderer::fonts::group_title.draw(this->m_name, { this->m_title_pos[0] + this->m_title_size[0] / 2, this->m_title_pos[1] + this->m_title_size[1] / 2 }, mango::rgba8{ 190, 190, 190 }, true, true);
}