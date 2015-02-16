#import "Delegate.h"

@implementation XAVIDelegate


- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	[self.output setStringValue: @"Started"];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

/*
- (void)applicationWillTerminate:(NSNotification *)aNotification
{
}
*/

- (IBAction) Calculate:(id)sender
{
	[self.output setStringValue: @"Test"];
}
@end
