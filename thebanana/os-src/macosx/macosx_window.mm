#include "pch.h"
#include "window.h"
#include "application_layer.h"
#include "game.h"
#import <Cocoa/Cocoa.h>
#include "macosx_internal.h"
#import <backends/imgui_impl_osx.h>
#import "delegate.h"
#import "view.h"
@implementation BananaAppDelegate
- (void)applicationDidFinishLaunching:(NSNotification*)notification {
    // todo: init window
}

- (void)applicationWillTerminate:(NSNotification*)notification {
    // todo: clean up
}
@end
@implementation BananaWindowDelegate
- (instancetype)initWithWindow:(NSWindow*)initWindow {
    self = [super init];
    if (self != nil) {
        window = initWindow;
    }
    return self;
}
@end
@implementation BananaContentView
- (instancetype)initWithWindow:(NSWindow*)initWindow {
    self = [super init];
    if (self != nil) {
        window = initWindow;
    }
    return self;
}
- (BOOL)isOpaque
{
    return [window isOpaque];
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}
@end
namespace thebanana {
    namespace platform_specific {
        _window_t::~_window_t() {
            delete this->data;
        }
        void setup_window() {
            @autoreleasepool {
                [NSApplication sharedApplication];
                [NSApp setDelegate:[[BananaAppDelegate alloc] init]];
                [NSBundle loadNibNamed:@"MainMenu" owner:[NSApp delegate]];
                [NSApp finishLaunching];
            }
        }
        int window_loop(application_layer* app_layer) {
            bool running = true;
            while (running) {
                NSEvent* event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
                NSWindow* window = [event window];
                ImGui_ImplOSX_HandleEvent(event, [window contentView]);
                switch ([(NSEvent*)event type]) {
                    default:
                        [NSApp sendEvent:event];
                        break;
                }
                [event release];
                app_layer->gameloop();
            }
            return 0;
        }
        window_t create_window(const std::string& title, size_t width, size_t height, game* g) {
            @autoreleasepool {
                NSRect frame = NSMakeRect(0, 0, width, height);
                NSWindow* window = [[NSWindow alloc] initWithContentRect:frame styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:NO];
                BananaContentView* view = [[BananaContentView alloc] initWithWindow:window];
                [window setContentView:view];
                [window makeFirstResponder:view];
                [window setTitle:@(title.c_str())];
                [window setDelegate:[[BananaWindowDelegate alloc] initWithWindow:window]];
                [window setAcceptsMouseMovedEvents:YES];
                [window setRestorable:NO];
                [window setIsVisible:YES];
                _window_t* window_struct = new _window_t;
                window_struct->data = new window_data;
                window_struct->data->window = window;
                return (size_t)window_struct;
            }
        }
        NSWindow* get_window(window_t window) {
            _window_t* window_struct = (_window_t*)window;
            return window_struct->data->window;
        }
        size_t get_width(window_t window) {
            NSWindow* _window = get_window(window);
            NSView* view = [_window contentView];
            NSRect frame = [view frame];
            return frame.size.width;
        }
        size_t get_height(window_t window) {
            NSWindow* _window = get_window(window);
            NSView* view = [_window contentView];
            NSRect frame = [view frame];
            return frame.size.height;
        }
        void get_size(window_t window, size_t& width, size_t& height) {
            NSWindow* _window = get_window(window);
            NSView* view = [_window contentView];
            NSRect frame = [view frame];
            width = frame.size.width;
            height = frame.size.height;
        }
        void set_window_title(window_t window, const std::string& title) {
            NSWindow* _window = get_window(window);
            [_window setTitle:@(title.c_str())];
        }
        void destroy_window(window_t window) {
            NSWindow* _window = get_window(window);
            [_window orderOut:nil];
            BananaWindowDelegate* delegate = (BananaWindowDelegate*)[_window delegate];
            [_window setDelegate:nil];
            [delegate release];
            [[_window contentView] release];
            [_window close];
        }
        window_t get_focus() {
            return NULL;
        }
    }
}
