#include "pch.h"
#include "window.h"
#include "application_layer.h"
#include "game.h"
#import <Cocoa/Cocoa.h>
#include "macosx_internal.h"
#import <backends/imgui_impl_osx.h>
#import "delegate.h"
#import "view.h"
#include "platform_misc.h"
#include <memory>
void create_menu_bar();
@implementation BananaAppDelegate
- (void)applicationWillFinishLaunching:(NSNotification *)notification {
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    if ([[NSBundle mainBundle] pathForResource:@"MainMenu" ofType:@"nib"]) {
        [[NSBundle mainBundle] loadNibNamed:@"MainMenu" owner:NSApp topLevelObjects:NULL];
    } else {
        create_menu_bar();
    }
}
@end
bool running = true;
@implementation BananaWindowDelegate
- (instancetype)initWithWindow:(NSWindow*)initWindow {
    self = [super init];
    if (self != nil) {
        window = initWindow;
    }
    return self;
}
- (BOOL)windowShouldClose:(NSWindow *)sender {
    running = false;
    return NO;
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
    return YES;
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}
// copied from imgui example
-(void)keyUp:(NSEvent *)event               { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)keyDown:(NSEvent *)event             { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)flagsChanged:(NSEvent *)event        { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)mouseDown:(NSEvent *)event           { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)rightMouseDown:(NSEvent *)event      { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)otherMouseDown:(NSEvent *)event      { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)mouseUp:(NSEvent *)event             { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)rightMouseUp:(NSEvent *)event        { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)otherMouseUp:(NSEvent *)event        { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)mouseMoved:(NSEvent *)event          { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)rightMouseMoved:(NSEvent *)event     { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)otherMouseMoved:(NSEvent *)event     { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)mouseDragged:(NSEvent *)event        { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)rightMouseDragged:(NSEvent *)event   { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)otherMouseDragged:(NSEvent *)event   { ImGui_ImplOSX_HandleEvent(event, self); }
-(void)scrollWheel:(NSEvent *)event         { ImGui_ImplOSX_HandleEvent(event, self); }
@end
void create_menu_bar() {
    std::string name = thebanana::platform_specific::get_module_name(0);
    NSString* app_name = @(name.c_str());
    NSMenu* bar = [[NSMenu alloc] init];
    [NSApp setMainMenu:bar];
    NSMenuItem* appMenuItem = [bar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
    NSMenu* appMenu = [[NSMenu alloc] init];
    [appMenuItem setSubmenu:appMenu];
    [appMenu addItemWithTitle:[NSString stringWithFormat:@"About %@", app_name] action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];
    [appMenu addItem:[NSMenuItem separatorItem]];
    NSMenu* servicesMenu = [[NSMenu alloc] init];
    [NSApp setServicesMenu:servicesMenu];
    [[appMenu addItemWithTitle:@"Services" action:NULL keyEquivalent:@""] setSubmenu:servicesMenu];
    [servicesMenu release];
    [appMenu addItem:[NSMenuItem separatorItem]];
    [appMenu addItemWithTitle:[NSString stringWithFormat:@"Hide %@", app_name] action:@selector(hide:) keyEquivalent:@"h"];
    [[appMenu addItemWithTitle:@"Hide Others" action:@selector(hideOtherApplications:) keyEquivalent:@"h"] setKeyEquivalentModifierMask:NSEventModifierFlagOption | NSEventModifierFlagCommand];
    [appMenu addItemWithTitle:@"Show All" action:@selector(unhideAllApplications:) keyEquivalent:@""];
    [appMenu addItem:[NSMenuItem separatorItem]];
    [appMenu addItemWithTitle:[NSString stringWithFormat:@"Quit %@", app_name] action:@selector(terminate:) keyEquivalent:@"q"];
    NSMenuItem* windowMenuItem = [bar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
    [bar release];
    NSMenu* windowMenu = [[NSMenu alloc] initWithTitle:@"Window"];
    [NSApp setWindowsMenu:windowMenu];
    [windowMenuItem setSubmenu:windowMenu];
    [windowMenu addItemWithTitle:@"Minimize" action:@selector(performMiniaturize:) keyEquivalent:@"m"];
    [windowMenu addItemWithTitle:@"Zoom" action:@selector(performZoom:) keyEquivalent:@""];
    [windowMenu addItem:[NSMenuItem separatorItem]];
    [windowMenu addItemWithTitle:@"Bring All to Front" action:@selector(arrangeInFront:) keyEquivalent:@""];
    [windowMenu addItem:[NSMenuItem separatorItem]];
    [[windowMenu addItemWithTitle:@"Enter Full Screen" action:@selector(toggleFullScreen:) keyEquivalent:@"f"] setKeyEquivalentModifierMask:NSEventModifierFlagControl | NSEventModifierFlagCommand];
    SEL setAppleMenuSelector = NSSelectorFromString(@"setAppleMenu:");
    [NSApp performSelector:setAppleMenuSelector withObject:appMenu];
}
namespace thebanana {
    namespace platform_specific {
        BANANA_API bool omit_app_layer_update = false;
        CFBundleRef openglframework;
        void* load_opengl_function(const char* name) {
            CFStringRef symbol_name = CFStringCreateWithCString(kCFAllocatorDefault, name, kCFStringEncodingASCII);
            void* address = CFBundleGetFunctionPointerForName(openglframework, symbol_name);
            CFRelease(symbol_name);
            return address;
        }
        _window_t::~_window_t() {
            delete this->data;
        }
        void setup_window() {
            @autoreleasepool {
                NSApp = [NSApplication sharedApplication];
                [NSApp setDelegate:[[BananaAppDelegate alloc] init]];
                [NSBundle loadNibNamed:@"MainMenu" owner:[NSApp delegate]];
                [NSApp finishLaunching];
                openglframework = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
                assert(openglframework);
            }
        }
        int window_loop(application_layer* app_layer) {
            while (running) {
                if (!omit_app_layer_update) app_layer->gameloop();
                @autoreleasepool {
                    for (;;) {
                        NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
                        if (event == nil) {
                            break;
                        }
                        [NSApp sendEvent:event];
                   }
                }
                if (omit_app_layer_update) {
                    break;
                }
            }
            return 0;
        }
        window_t create_window(const std::string& title, size_t width, size_t height, game* g) {
            @autoreleasepool {
                NSRect frame = NSMakeRect(0, 0, width, height);
                NSWindow* window = [[NSWindow alloc] initWithContentRect:frame styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable backing:NSBackingStoreBuffered defer:NO];
                BananaContentView* view = [[BananaContentView alloc] initWithWindow:window];
                [window setContentView:view];
                [window makeFirstResponder:view];
                [window setTitle:@(title.c_str())];
                [window setDelegate:[[BananaWindowDelegate alloc] initWithWindow:window]];
                [window setAcceptsMouseMovedEvents:YES];
                [window setRestorable:NO];
                [window setIsVisible:YES];
                [window setOpaque:YES];
                [window setBackgroundColor:[NSColor whiteColor]];
                _window_t* window_struct = new _window_t;
                window_struct->data = new window_data;
                window_struct->data->window = window;
                return (size_t)window_struct;
            }
        }
        BANANA_API NSWindow* get_window(window_t window) {
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
            running = false;
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
