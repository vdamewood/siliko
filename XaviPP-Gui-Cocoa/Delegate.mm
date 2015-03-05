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

	char *ResultCString = Result.ToCString();
	NSString *ResultString = [[NSString alloc] initWithUTF8String: ResultCString];
	delete[] ResultCString;
	[self.output setStringValue: ResultString];
}
@end
