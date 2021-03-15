#include "pch.h"
#include "cursor.h"
namespace thebanana {
	namespace platform_specific {
		void set_cursor(cursor_t c) {
			SetCursor(c);
		}
		cursor_t load_cursor(const char* resource) {
			if (!resource) {
				return NULL;
			}
			return LoadCursorA(NULL, resource);
		}
		const char* make_cursor_location(cursor_types type) {
			return MAKEINTRESOURCEA(type);
		}
	}
}