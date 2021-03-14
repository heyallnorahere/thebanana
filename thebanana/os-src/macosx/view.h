@interface BananaContentView : NSView<NSTextInputClient>
{
    NSWindow* window;
}
- (instancetype)initWithWindow:(NSWindow*)initWindow;
@end
