#pragma once
#include <iostream>
#include <misc/vector.h>

class c_menu;
class c_container;

enum class c_element_types {
	menu,
	group,
	checkbox,
	button,
	slider,
	combobox,
	subtab,
	tab
};

class c_base_element {
public:
	c_base_element() = default;

	c_base_element(const std::string& name, const std::string& info) : m_name(name), m_info(info) {};

	std::string m_name;
	std::string m_info;

public:
	mango::Vec2i m_pos;
	mango::Vec2i m_size;
	mango::Vec2i m_start_pos;

public:
	c_container* m_parent;
	c_menu* m_parent_menu;

	bool m_hovered = false;
	bool m_viewable = true;

	virtual bool update() { return true; };
	virtual void draw() {};
	virtual mango::Vec2i calc_pos_size() { return { 0, 0 }; };
	virtual c_element_types get_type() const { return c_element_types::menu; };
};