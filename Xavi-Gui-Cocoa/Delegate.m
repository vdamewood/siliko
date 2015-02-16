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

	switch (Result.Status)
	{
	case XAVI_VAL_INTEGER:
		[self.output setIntegerValue: Result.Integer];
		break;
	case XAVI_VAL_FLOAT:
		[self.output setFloatValue: Result.Float];
		break;
        case XAVI_VAL_MEMORY_ERR:
		[self.output setStringValue: @"Out of memory"];
		break;
        case XAVI_VAL_SYNTAX_ERR:
		[self.output setStringValue: @"Syntax error"];
		break;
        case XAVI_VAL_ZERO_DIV_ERR:
		[self.output setStringValue: @"Division by zero"];
		break;
        case XAVI_VAL_BAD_FUNCTION:
		[self.output setStringValue: @"Function not found"];
		break;
        case XAVI_VAL_BAD_ARGUMENTS:
		[self.output setStringValue: @"Bad arguments"];
		break;
        case XAVI_VAL_DOMAIN_ERR:
		[self.output setStringValue: @"Domain error"];
		break;
        case XAVI_VAL_RANGE_ERR:
		[self.output setStringValue: @"Range error"];
		break;
	}
}
@end
