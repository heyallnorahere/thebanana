@interface BananaAppDelegate : NSObject<NSApplicationDelegate>
@end
@interface BananaWindowDelegate : NSObject<NSWindowDelegate>
{
    NSWindow* window;
}
- (instancetype)initWithWindow:(NSWindow*)initWindow;
@end
