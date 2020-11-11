#include <thebanana.h>
#include "panel.h"
namespace bananatree {
	panel::panel() {
		this->show();
	}
	void panel::update() { }
	bool& panel::is_open() {
		return this->m_open;
	}
	void panel::show() {
		this->m_open = true;
	}
	void panel::hide() {
		this->m_open = false;
	}
	void panel::toggle() {
		this->m_open = !this->m_open;
	}
	panel::~panel() { }
}