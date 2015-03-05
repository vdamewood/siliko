#import "Delegate.h"

#include <Xavi/StringSource.h>
#include <Xavi/InfixParser.h>
#include <Xavi/SyntaxTree.h>
#include <Xavi/FunctionCaller.h>

@implementation XAVIDelegate


- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	XaviFunctionCallerSetUp();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	XaviFunctionCallerTearDown();
}

- (IBAction) Calculate:(id)sender
{
	XaviSyntaxTreeNode *Ast;
	XaviValue Result;

	Ast = XaviParseInfix(XaviStringSourceNew([[self.input stringValue] UTF8String]));
	Result = XaviSyntaxTreeEvaluate(Ast);
	XaviSyntaxTreeDelete(Ast);

	char *ResultCString = XaviValueToString(Result);
	NSString *ResultString = [[NSString alloc] initWithUTF8String: ResultCString];
	free(ResultCString);
	[self.output setStringValue: ResultString];
}
@end
