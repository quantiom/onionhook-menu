#pragma once
#include <iostream>
#include <misc/color.h>

#include "./c_container.h"

struct c_menu_style {
	mango::rgba8 main, secondary, darker_secondary, bar;
};

class c_menu : public c_container {
public:
	c_menu() = default;

	c_menu(const std::string& bold_name, const std::string& non_bold_name, const mango::Vec2i& pos) {
		this->setup(bold_name, non_bold_name, pos);
	}

	void setup(const std::string& bold_name, const std::string& non_bold_name, const mango::Vec2i& pos) {
		this->m_name = bold_name + non_bold_name;
		this->m_bold_name = bold_name;
		this->m_non_bold_name = non_bold_name;
		this->m_pos = pos;
		this->m_size = { 800, 600 };
		this->m_style = c_menu_style({ mango::rgba8{ 32, 32, 38, 255 }, mango::rgba8{ 60, 60, 158, 255 }, mango::rgba8{ 0, 0, 0, 0 }, mango::rgba8{ 0, 0, 0, 0 } });
	}

	void update_style() {
		this->m_style.bar = mango::rgba8{
			uint8_t(this->m_style.secondary[0] + (255 - this->m_style.secondary[0]) * 0.05f),
			uint8_t(this->m_style.secondary[1] + (255 - this->m_style.secondary[1]) * 0.05f),
			uint8_t(this->m_style.secondary[2] + (255 - this->m_style.secondary[2]) * 0.05f)
		};

		this->m_style.darker_secondary = mango::rgba8{
			uint8_t(this->m_style.secondary[0] * 0.5f),
			uint8_t(this->m_style.secondary[1] * 0.5f),
			uint8_t(this->m_style.secondary[2] * 0.5f)
		};
	}

	bool update();
	void draw();

	c_element_types get_type() const { return c_element_types::menu; };
	c_menu_style m_style;

	bool m_open;
	int m_anim_y_pos;

private:
	float alpha_multiplier;
	float m_timer = 0.f;

	bool m_dragging;

	std::string m_bold_name;
	std::string m_non_bold_name;
};