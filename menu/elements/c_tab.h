#pragma once
#include "../c_container.h"

class c_tab : public c_container {
public:
	c_tab(const std::string& name, const std::string& info = "No information available.") : c_container(name, info) {}

	bool update();
	void draw();

	c_element_types get_type() const { return c_element_types::tab; };
	bool m_active = false;

private:
	float m_activated_time = 0.f;
};