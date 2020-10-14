#pragma once
#include "framebuffer.h"
namespace thebanana {
	class opengl_framebuffer : public framebuffer {
	public:
		struct desc {
			static constexpr int color = 0b1, depth = 0b10;
			int width, height, buffers;
		};
		opengl_framebuffer(desc* d);
		virtual void bind() override;
		virtual void unbind() override;
		virtual void set_size(int width, int height) override;
		virtual void reload() override;
		virtual std::vector<attachment> get_attachments() override;
		virtual ~opengl_framebuffer() override;
	private:
		desc m_description;
		std::vector<attachment> m_attachments;
		unsigned int m_id;
		void setup();
		void clean_up();
	};
}