#pragma once
namespace bananatree {
	class editor_layer : public thebanana::application_layer {
	public:
		virtual void init() override;
		virtual void gameloop() override;
		virtual std::string window_title() override;
	};
}