#pragma once
#include "../c_base_element.h"

class c_checkbox : public c_base_element {
public:
	c_checkbox(const std::string& name, bool& value, const std::string& info = "No information available.")
		: c_base_element(name, info), m_value(value) {};

	bool update();
	void draw();
	mango::Vec2i calc_pos_size();

	c_element_types get_type() const { return c_element_types::checkbox; };
	bool& m_value;

private:
	float m_timer = 0.f;
	uint8_t m_alpha;
};