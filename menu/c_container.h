#pragma once
#include "./c_base_element.h"
#include <misc/logger.h>

class c_container : public c_base_element {
public:
	c_container() = default;

	c_container(const std::string& name, const std::string& info) : c_base_element(name, info) {}

	void draw_elements(mango::Vec2i pos) {
		if (this->m_elements.empty()) return;

		const auto start_pos = pos;

		std::vector<std::shared_ptr<c_base_element>> groups;
		std::vector<std::shared_ptr<c_base_element>> non_groups;
		
		for (const auto& element : this->m_elements) {
			if (element->get_type() == c_element_types::group) groups.push_back(element);
			else non_groups.push_back(element);
		}

		// do groups last
		for (const auto& element : non_groups) {
			element->m_parent_menu = this->m_parent_menu;
			element->m_parent = this;

			element->m_pos = pos;

			const auto offset = element->calc_pos_size();

			pos[0] += offset[0]; 
			pos[1] += offset[1];
		}
		
		for (const auto& element : non_groups) if (!element->update()) break;

		for (int i = non_groups.size() - 1; i >= 0; i--) {
			auto element = non_groups[i];
			element->draw();
		}

		if (groups.empty()) return;

		mango::Vec2i group_pos = { start_pos[0], pos[1] + 45 };

		// groups
		for (const auto& group : groups) {
			group->m_parent_menu = this->m_parent_menu;
			group->m_parent = this;

			group->m_pos = group_pos;

			const auto offset = group->calc_pos_size();
			group_pos[0] += offset[0]; group_pos[1] += offset[1];
		}

		for (const auto& group : groups) if (!group->update()) break;

		for (int i = groups.size() - 1; i >= 0; i--) {
			auto group = groups[i];
			group->draw();
		}
	}

	template <typename Type, typename ...Args>
	std::shared_ptr<Type> add_element(Args&&... args) {
		auto element = std::make_shared<Type>(std::forward<Args>(args)...);
		this->m_elements.push_back(element);
		return element;
	}

	std::vector<std::shared_ptr<c_base_element>> m_elements;
};