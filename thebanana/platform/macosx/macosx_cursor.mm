#include "pch.h"
#include "cursor.h"
namespace thebanana {
    namespace platform_specific {
        void set_cursor(cursor_t c) {
            // todo: implement
        }
        cursor_t load_cursor(const char* resource) {
            return 0;
        }
        const char* make_cursor_location(cursor_types type) {
            return NULL;
        }
    }
}
