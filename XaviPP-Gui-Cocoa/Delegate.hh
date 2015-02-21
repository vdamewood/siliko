#import <Cocoa/Cocoa.h>

@interface XAVIPPDelegate : NSObject <NSApplicationDelegate>

@property (weak) IBOutlet NSTextField *input;
@property (weak) IBOutlet NSTextField *output;

-(IBAction) Calculate:(id)sender;

@end
