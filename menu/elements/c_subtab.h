#pragma once
#include "../c_container.h"

class c_subtab : public c_container {
public:
	c_subtab(const std::string& name, const std::string& info = "No information available.") : c_container(name, info) {}

	bool update();
	void draw();

	c_element_types get_type() const { return c_element_types::subtab; };
	bool m_active = false;

	int m_tab_size;
};