#pragma once
namespace bananatree {
	class panel {
	public:
		panel();
		virtual void update();
		virtual void render() = 0;
		virtual std::string get_menu_text() = 0;
		bool& is_open();
		void show();
		void hide();
		void toggle();
		virtual ~panel();
	protected:
		bool m_open;
	};
}