#import "Delegate.h"

#include "StringSource.h"
#include "InfixParser.h"
#include "SyntaxTree.h"
#include "FunctionCaller.h"

@implementation SilikoDelegate


- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	SilikoFunctionCallerSetUp();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	SilikoFunctionCallerTearDown();
}

- (IBAction) Calculate:(id)sender
{
	SilikoSyntaxTreeNode *Ast;
	SilikoValue Result;

	Ast = SilikoParseInfix(SilikoStringSourceNew([[self.input stringValue] UTF8String]));
	Result = SilikoSyntaxTreeEvaluate(Ast);
	SilikoSyntaxTreeDelete(Ast);

	char *ResultCString = SilikoValueToString(Result);
	NSString *ResultString = [[NSString alloc] initWithUTF8String: ResultCString];
	free(ResultCString);
	[self.output setStringValue: ResultString];
}
@end
