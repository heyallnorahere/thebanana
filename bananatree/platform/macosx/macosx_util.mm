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
        std::string open_dialog(const char* filter, int* index) {
            thebanana::window window_ = thebanana::g_game->get_window();
            NSWindow* window = thebanana::platform_specific::get_window(window_.m);
            NSOpenPanel* panel = [NSOpenPanel openPanel];
            [panel setCanChooseFiles:YES];
            [panel setCanChooseDirectories:NO];
            [panel setResolvesAliases:YES];
            [panel setAllowsMultipleSelection:NO];
            [panel setAccessoryViewDisclosed:NO];
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
