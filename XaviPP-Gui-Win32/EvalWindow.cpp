/*
 * EvalWindow.cpp: Expression evaluation window
 * Copyright 2015 Vincent Damewood
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

#include <cstdlib>
#include <afxwin.h>

#include <XaviPP/CStringSource.hpp>
#include <XaviPP/InfixParser.hpp>
#include <XaviPP/Value.hpp>

#include "About.hpp"
#include "EvalWindow.hpp"
#include "Id.h"

#define OUT_BORDER    8
#define IN_BORDER     4
#define TEXT_HEIGHT   24
#define BUTTON_HEIGHT 28
#define BUTTON_WIDTH  100

#define MIN_HEIGHT    (70 + OUT_BORDER * 2 + TEXT_HEIGHT + BUTTON_HEIGHT + IN_BORDER)
#define MIN_WIDTH     420

#define INPUT_X       OUT_BORDER
#define INPUT_Y       OUT_BORDER
#define INPUT_HT      TEXT_HEIGHT
#define INPUT_WT      (OUT_BORDER * 2)
#define INPUT_X2      (MIN_WIDTH - OUT_BORDER*4)
#define INPUT_Y2      (INPUT_X + INPUT_HT)


#define OUTPUT_X      OUT_BORDER
#define OUTPUT_Y      (OUT_BORDER + TEXT_HEIGHT + IN_BORDER)
#define OUTPUT_HT     TEXT_HEIGHT
#define OUTPUT_WT     (OUT_BORDER * 2 + IN_BORDER + BUTTON_WIDTH)
#define OUTPUT_X2     (MIN_WIDTH - OUT_BORDER*4 - IN_BORDER - BUTTON_WIDTH)
#define OUTPUT_Y2     (OUTPUT_Y + OUTPUT_HT)

#define BUTTON_X      (BUTTON_WIDTH + OUT_BORDER)
#define BUTTON_Y      OUTPUT_Y
#define BUTTON_HT     BUTTON_HEIGHT
#define BUTTON_WT     BUTTON_WIDTH
#define BUTTON_X2     (MIN_WIDTH - OUT_BORDER*4)
#define BUTTON_Y2     (BUTTON_Y + BUTTON_WT)



BEGIN_MESSAGE_MAP(EvalWindow, CFrameWnd)
	ON_COMMAND(CALCULATOR_BUTTON, &EvalWindow::Calculate)
	ON_COMMAND(CALCULATOR_EXIT, &EvalWindow::CalculatorExit)
	ON_COMMAND(HELP_ABOUT, &EvalWindow::HelpAbout)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
END_MESSAGE_MAP()

EvalWindow::EvalWindow()
{
	Create(
		AfxRegisterWndClass(0, 0, (HBRUSH)(COLOR_WINDOW), 0),
		"XaviPP");

	Input.Create(
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER,
		CRect(INPUT_X, INPUT_Y, INPUT_X2, INPUT_Y2), this, CALCULATOR_INPUT);
	Output.Create( 
		_T(""),
		WS_CHILD | WS_VISIBLE, 
		CRect(OUTPUT_X, OUTPUT_Y, OUTPUT_X2, OUTPUT_Y2), this, CALCULATOR_OUTPUT);
	Button.Create( 
		_T("Calculate"),
		WS_CHILD | WS_VISIBLE, 
		CRect(MIN_WIDTH - BUTTON_X, BUTTON_Y, BUTTON_X2, OUTPUT_Y2), this, CALCULATOR_BUTTON);

	SetWindowPos(NULL, 0, 0, MIN_WIDTH, MIN_HEIGHT, SWP_NOZORDER | SWP_NOMOVE);
	CMenu menu;
	menu.LoadMenu(XAVI_MENU);
	SetMenu(&menu);
	menu.Detach();
	Input.SetFocus();
}

void EvalWindow::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	Input.SetWindowPos(NULL, INPUT_X, INPUT_Y, cx - INPUT_WT, INPUT_HT, SWP_NOZORDER);
	Output.SetWindowPos(NULL, OUTPUT_X, OUTPUT_Y, cx - OUTPUT_WT, OUTPUT_HT, SWP_NOZORDER);
	Button.SetWindowPos(NULL, cx - BUTTON_X, BUTTON_Y, BUTTON_WT, BUTTON_HT, SWP_NOZORDER);
}

void EvalWindow::OnGetMinMaxInfo(MINMAXINFO* info)
{
	info->ptMinTrackSize.x = MIN_WIDTH;
	info->ptMinTrackSize.y = MIN_HEIGHT;
}


BOOL EvalWindow::PreTranslateMessage(MSG *Message)
{
	CFrameWnd::PreTranslateMessage(Message);
	if (CWnd::FromHandle(Message->hwnd) == &Input
		&& Message->message == WM_KEYDOWN
		&& Message->wParam == VK_RETURN)
	{
		PostMessage(WM_COMMAND, CALCULATOR_BUTTON, 0);
		return TRUE;
	}

	return FALSE;
}

void EvalWindow::Calculate()
{
	int ExpressionSize;
	char *ExpressionString;
	char *ValueString;
	Xavi::SyntaxTreeNode *Node;
	Xavi::Value Value;

	ExpressionSize = Input.GetWindowTextLength() + 1;
	ExpressionString = (char*)GlobalAlloc(GPTR, ExpressionSize);
	Input.GetWindowText(ExpressionString, ExpressionSize);
	
	Node = Xavi::ParseInfix(new Xavi::CStringSource(ExpressionString));
	Value = Node->Evaluate();
	std::free(Node);
	GlobalFree((HANDLE)ExpressionString);
	
	ValueString = Value.ToCString();
	Output.SetWindowText(ValueString);
	std::free(ValueString);	
}

void EvalWindow::CalculatorExit()
{
	PostMessage(WM_CLOSE, 0, 0);
}

void EvalWindow::HelpAbout()
{
	AboutDialog MyAbout;
	MyAbout.DoModal();
}
