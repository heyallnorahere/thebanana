#include <thebanana.h>
#include "util.h"
#include <Cocoa/Cocoa.h>
namespace thebanana {
    namespace platform_specific {
        BANANA_API extern NSWindow* get_window(window_t window);
        BANANA_API extern bool omit_app_layer_update;
        BANANA_API extern int window_loop(application_layer* app_layer);
    }
}
bool success, done;
std::string path;
namespace bananatree {
    namespace util {
        static NSArray<NSString*>* parse_filter(const char* filter) {
            size_t pos = 0;
            std::vector<std::string> temp;
            auto find = [](size_t& pos, const char* filter, std::vector<std::string>& temp) -> size_t {
                size_t offset = pos + 1;
                std::string str = std::string(filter + offset);
                for (size_t i = offset; ; i++) {
                    if (filter[i] == '\0') {
                        offset = i;
                        break;
                    }
                }
                if (str.length() == 0 || filter[offset + 1] == '\0') {
                    offset = std::string::npos;
                }
                temp.push_back(str);
                return offset;
            };
            while ((pos = find(pos, filter, temp)) != std::string::npos) { /* wait*/ }
            std::vector<NSString*> strings;
            for (size_t i = 1; i < (temp.size() - (temp.size() % 2)); i += 2) {
                std::string filetype = temp[i];
                std::vector<std::string> subtypes;
                std::string extension;
                for (char c : filetype) {
                    if (c == ',') {
                        subtypes.push_back(extension);
                        extension.clear();
                    } else {
                        extension.append(std::string(&c, 1));
                    }
                }
                subtypes.push_back(extension);
                for (std::string type : subtypes) {
                    size_t pos = type.find('.');
                    if (pos != std::string::npos) {
                        type = type.substr(pos + 1);
                    }
                    strings.push_back(@(type.c_str()));
                }
            }
            NSArray<NSString*>* array = [NSArray arrayWithObjects:strings.data() count:strings.size()];
            return array;
        }
        std::string open_dialog(const char* filter, int* index) {
            thebanana::window window_ = thebanana::g_game->get_window();
            NSWindow* window = thebanana::platform_specific::get_window(window_.m);
            NSOpenPanel* panel = [NSOpenPanel openPanel];
            [panel setCanChooseFiles:YES];
            [panel setCanChooseDirectories:NO];
            [panel setResolvesAliases:YES];
            [panel setAllowsMultipleSelection:NO];
            [panel setAccessoryViewDisclosed:NO];
            NSArray<NSString*>* filetypes = parse_filter(filter);
            [panel setAllowedFileTypes:filetypes];
            [filetypes release];
            done = false;
            [panel beginSheetModalForWindow:window completionHandler:^(NSInteger result) {
                success = (result == NSModalResponseOK);
                if (success) {
                    NSArray* urls = [panel URLs];
                    NSURL* url = [urls objectAtIndex:0];
                    path = [[url path] UTF8String];
                }
                done = true;
            }];
            thebanana::platform_specific::omit_app_layer_update = true;
            while (!done) {
                // wait, but poll events
                thebanana::platform_specific::window_loop(NULL);
            }
            thebanana::platform_specific::omit_app_layer_update = false;
            [panel release];
            if (success) {
                return path;
            } else {
                return "";
            }
        }
        std::string save_dialog(const char* filter, int* index) {
            thebanana::window window_ = thebanana::g_game->get_window();
            NSWindow* window = thebanana::platform_specific::get_window(window_.m);
            NSSavePanel* panel = [NSSavePanel savePanel];
            [panel setCanCreateDirectories:YES];
            NSArray<NSString*>* filetypes = parse_filter(filter);
            [panel setRequiredFileType:[filetypes objectAtIndex:0]];
            [filetypes release];
            done = false;
            [panel beginSheetModalForWindow:window completionHandler:^(NSInteger result) {
                success = (result == NSModalResponseOK);
                if (success) {
                    path = [[[panel URL] path] UTF8String];
                }
                done = true;
            }];
            thebanana::platform_specific::omit_app_layer_update = true;
            while (!done) {
                // wait, but poll events
                thebanana::platform_specific::window_loop(NULL);
            }
            thebanana::platform_specific::omit_app_layer_update = false;
            if (success) {
                return path;
            } else {
                return "";
            }
        }
        
        std::string make_temp_path(const std::string& prefix) {
            return "";
        }
    }
}
