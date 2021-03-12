#include <thebanana.h>
#include "viewport_panel.h"
#ifdef BANANA_WINDOWS
#include <gl/GL.h>
#elif defined BANANA_LINUX
#include <GL/gl.h>
#endif
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include <ImGuizmo.h>
#include "../imgui_layer.h"
#include "scene_hierarchy_panel.h"
namespace bananatree {
	viewport_panel::viewport_panel() {
		this->m_gizmo_operation = gizmo_operation::translate;
		this->m_transformation_mode = transformation_mode::local;
		thebanana::graphics::framebuffer::specification spec;
		spec.width = 800;
		spec.height = 600;
		spec.buffers = thebanana::graphics::framebuffer::specification::color | thebanana::graphics::framebuffer::specification::depth;
		this->m_framebuffer = std::unique_ptr<thebanana::graphics::framebuffer>(thebanana::graphics::framebuffer::create(&spec));
		this->m_keyboard_index = std::string::npos;
		thebanana::input_manager* input_manager = thebanana::g_game->get_input_manager();
		for (size_t i = 0; i < input_manager->get_num_devices(); i++) {
			if (input_manager->get_device_type(i) == thebanana::input_manager::device_type::keyboard) {
				this->m_keyboard_index = i;
				break;
			}
		}
#ifdef BANANA_WINDOWS
		assert(this->m_keyboard_index != std::string::npos);
#endif
	}
	void viewport_panel::render() {
		ImGui::Begin("Viewport", &this->m_open);
		ImVec2 content_region = ImGui::GetContentRegionAvail();
		this->m_framebuffer->bind();
		if (content_region.x > 0.f && content_region.y > 0.f) {
			glViewport(0, 0, static_cast<int>(content_region.x), static_cast<int>(content_region.y));
			thebanana::g_game->set_aspect_ratio(content_region.x / content_region.y);
			thebanana::g_game->set_custom_window_size(glm::vec2(content_region.x, content_region.y));
		}
		thebanana::g_game->render();
		this->m_framebuffer->unbind();
		auto att = this->m_framebuffer->get_attachments();
		void* texture = att[this->m_framebuffer->get_attachment_map().color_index].value;
		ImGui::Image(texture, content_region, ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
		thebanana::gameobject* main_camera = thebanana::g_game->get_scene()->find_main_camera();
		thebanana::gameobject* selected_object = this->m_imgui_layer->find_panel<scene_hierarchy_panel>()->get_selected_object();
		if (main_camera && selected_object) {
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, content_region.x, content_region.y);
			auto& camera = main_camera->get_component<thebanana::camera_component>();
			glm::mat4 projection = camera.calculate_projection();
			glm::mat4 view = camera.calculate_view();
			glm::mat4 transform = selected_object->get_absolute_transform();
			bool snap = thebanana::g_game->get_input_manager()->get_key(thebanana::key_ctrl).held;
			float snap_value = 0.5f;
			if (this->m_gizmo_operation == gizmo_operation::rotate) {
				snap_value = 45.f;
			}
			float snap_values[3] = { snap_value, snap_value, snap_value };
			if (this->m_gizmo_operation != gizmo_operation::none) {
				glm::mat4 delta;
				ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), (ImGuizmo::OPERATION)this->m_gizmo_operation, (ImGuizmo::MODE)this->m_transformation_mode, glm::value_ptr(transform), glm::value_ptr(delta), snap ? snap_values : NULL);
				if (ImGuizmo::IsUsing()) {
					auto& object_transform = selected_object->get_transform();
					glm::vec3 translation, rotation, scale;
					glm::mat4 transform_matrix(1.f);
					if (!selected_object->get_parent()) {
						transform_matrix = transform;
					} else {
						transform_matrix = object_transform.get_matrix() * (transform / selected_object->get_parent()->get_absolute_transform().get_matrix());
					}
					thebanana::transform::decompose_matrix(transform_matrix, translation, rotation, scale);
					object_transform.translation() = translation;
					object_transform.rotation() += rotation;
					object_transform.scale() = scale;
				}
			}
		}
		ImGui::End();
	}
	std::string viewport_panel::get_menu_text() {
		return "Viewport";
	}
	void viewport_panel::set_imgui_layer(imgui_layer* il) {
		this->m_imgui_layer = il;
	}
	void viewport_panel::set_gizmo_operation(gizmo_operation op) {
		this->m_gizmo_operation = op;
	}
	viewport_panel::gizmo_operation viewport_panel::get_gizmo_operation() {
		return this->m_gizmo_operation;
	}
	void viewport_panel::set_transformation_mode(transformation_mode mode) {
		this->m_transformation_mode = mode;
	}
	viewport_panel::transformation_mode viewport_panel::get_transformation_mode() {
		return this->m_transformation_mode;
	}
}