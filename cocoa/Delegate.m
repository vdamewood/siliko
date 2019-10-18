/* Delegate.m: Delegate for Cocoa
 * Copyright 2014, 2015, 2016 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Siliko/StringSource.h>
#include <Siliko/InfixParser.h>
#include <Siliko/SyntaxTree.h>
#include <Siliko/FunctionCaller.h>

#import "Delegate.h"

@implementation SilikujoDelegate


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
