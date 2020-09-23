#pragma once
#include "../c_container.h"

class c_group : public c_container {
public:
	c_group(const std::string& name, const std::string& info = "No information available.") : c_container(name, info) {}

	mango::Vec2i calc_pos_size();
	bool update();
	void draw();

	c_element_types get_type() const { return c_element_types::group; };

	mango::Vec2i m_body_pos;
	mango::Vec2i m_body_size;

private:
	mango::Vec2i m_title_pos;
	mango::Vec2i m_title_size;

	mango::Vec2i m_bottom_bar_pos;
	mango::Vec2i m_bottom_bar_size;
};