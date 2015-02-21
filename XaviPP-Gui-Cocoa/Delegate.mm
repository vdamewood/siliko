#import "Delegate.hh"

#include <XaviPP/InfixParser.hpp>
#include <XaviPP/SyntaxTree.hpp>
#include <XaviPP/FunctionCaller.hpp>
#include <XaviPP/CStringSource.hpp>

#import <Cocoa/Cocoa.h>

@implementation XAVIPPDelegate


- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	Xavi::FunctionCaller::SetUp();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	Xavi::FunctionCaller::TearDown();
}

- (IBAction) Calculate:(id)sender
{
	Xavi::SyntaxTreeNode *Ast;
	Xavi::Value Result;

	Ast = Xavi::ParseInfix(
		new Xavi::CStringSource(
			[[self.input stringValue] UTF8String]));
	Result = Ast->Evaluate();
	delete Ast;

	switch (Result.Status())
	{
	case Xavi::Value::INTEGER:
		[self.output setIntegerValue: Result.Integer()];
		break;
	case Xavi::Value::FLOAT:
		[self.output setFloatValue: Result.Float()];
		break;
        case Xavi::Value::MEMORY_ERR:
		[self.output setStringValue: @"Out of memory"];
		break;
        case Xavi::Value::SYNTAX_ERR:
		[self.output setStringValue: @"Syntax error"];
		break;
        case Xavi::Value::ZERO_DIV_ERR:
		[self.output setStringValue: @"Division by zero"];
		break;
        case Xavi::Value::BAD_FUNCTION:
		[self.output setStringValue: @"Function not found"];
		break;
        case Xavi::Value::BAD_ARGUMENTS:
		[self.output setStringValue: @"Bad arguments"];
		break;
        case Xavi::Value::DOMAIN_ERR:
		[self.output setStringValue: @"Domain error"];
		break;
        case Xavi::Value::RANGE_ERR:
		[self.output setStringValue: @"Range error"];
		break;
	}
}
@end
