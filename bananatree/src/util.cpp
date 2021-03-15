#include <thebanana.h>
#include "util.h"
namespace bananatree {
    namespace util {
        bool file_exists(const std::string& path) {
			FILE* f = fopen(path.c_str(), "r");
			if (f) {
				fclose(f);
				return true;
			}
			else return false;
		}
    }
}