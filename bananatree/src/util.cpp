#include <thebanana.h>
#include "util.h"
#include <commdlg.h>
namespace bananatree {
	namespace util {
		void* load_file_from_resource(int resource, const std::string& type, size_t& size) {
			HRSRC resource_ = FindResourceA(NULL, MAKEINTRESOURCEA(resource), type.c_str());
			if (!resource_) return NULL;
			HGLOBAL global = LoadResource(NULL, resource_);
			if (!global) return NULL;
			void* data = LockResource(global);
			if (!data) return NULL;
			size = (size_t)SizeofResource(NULL, resource_);
			if (!size) return NULL;
			return data;
		}
		std::string open_dialog(const char* filter, int* index) {
			OPENFILENAMEA ofn;
			char file[256];
			memset(file, 0, sizeof(file));
			memset(&ofn, 0, sizeof(OPENFILENAMEA));
			ofn.lStructSize = sizeof(OPENFILENAMEA);
			ofn.hwndOwner = thebanana::g_game->get_window();
			ofn.lpstrFile = file;
			ofn.nMaxFile = sizeof(file);
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
			if (GetOpenFileNameA(&ofn)) {
				if (index) {
					*index = static_cast<int>(ofn.nFilterIndex);
				}
				return ofn.lpstrFile;
			}
			return "";
		}
		std::string save_dialog(const char* filter, int* index) {
			OPENFILENAMEA ofn;
			char file[256];
			memset(file, 0, sizeof(file));
			memset(&ofn, 0, sizeof(OPENFILENAMEA));
			ofn.lStructSize = sizeof(OPENFILENAMEA);
			ofn.hwndOwner = thebanana::g_game->get_window();
			ofn.lpstrFile = file;
			ofn.nMaxFile = sizeof(file);
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
			ofn.lpstrDefExt = strchr(filter, '\0') + 1;
			if (GetSaveFileNameA(&ofn)) {
				if (index) {
					*index = static_cast<int>(ofn.nFilterIndex);
				}
				return ofn.lpstrFile;
			}
			return "";
		}
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