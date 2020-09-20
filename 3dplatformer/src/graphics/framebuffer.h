#pragma once
#include "graphics_types.h"
class framebuffer {
public:
	struct attachment_map {
		int color_index = -1;
		int depth_index = -1;
	};
	struct attachment {
		enum type {
			color,
			depth,
		};
		type type;
		void* value;
	};
	framebuffer(void* data);
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void set_size(int width, int height) = 0;
	virtual void resize() = 0;
	virtual std::vector<attachment> get_attachments() = 0;
	virtual ~framebuffer();
	attachment_map get_attachment_map() const;
	static framebuffer* create(void* data, graphics_api api = graphics_api::none);
protected:
	attachment_map m_attachment_map;
};
class dummy_framebuffer : public framebuffer {
public:
	dummy_framebuffer(void* data);
	virtual void bind() override;
	virtual void unbind() override;
	virtual void set_size(int width, int height) override;
	virtual void resize() override;
	virtual std::vector<attachment> get_attachments() override;
};