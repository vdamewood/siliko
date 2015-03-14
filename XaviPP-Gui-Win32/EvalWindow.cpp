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

#define _WIN32_WINNT NTDDI_WIN7
#include <afxwin.h>
#include <cstdlib>

#include <XaviPP/CStringSource.hpp>
#include <XaviPP/InfixParser.hpp>
#include <XaviPP/Value.hpp>

#include "Menu.h"
#include "About.hpp"
#include "EvalWindow.hpp"

#define WIDTH 442
#define HEIGHT (GetSystemMetrics(SM_CYMENU) + 112)
#define CALCULATOR_INPUT  0x91
#define CALCULATOR_OUTPUT 0x92
#define CALCULATOR_BUTTON 0x93

BEGIN_MESSAGE_MAP(EvalWindow, CFrameWnd)
	ON_COMMAND(CALCULATOR_BUTTON, &EvalWindow::Calculate)
	ON_COMMAND(CALCULATOR_EXIT, &EvalWindow::CalculatorExit)
	ON_COMMAND(HELP_ABOUT, &EvalWindow::HelpAbout)
	ON_WM_SIZING()
	ON_WM_SIZE()
END_MESSAGE_MAP()

EvalWindow::EvalWindow()
{
	Create(
		AfxRegisterWndClass(0, 0, (HBRUSH)(COLOR_WINDOW), 0),
		"XaviPP");

	m_input.Create(
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER,
		CRect(8, 8, 412, 32), this, CALCULATOR_INPUT);
	m_output.Create( 
		_T(""),
		WS_CHILD | WS_VISIBLE, 
		CRect(8, 32, 304, 56), this, CALCULATOR_OUTPUT);
	m_button.Create( 
		_T("Calculate"),
		WS_CHILD | WS_VISIBLE, 
		CRect(304, 32, 412, 60), this, CALCULATOR_BUTTON);

	SetWindowPos(NULL, 0, 0, WIDTH, HEIGHT, SWP_NOZORDER | SWP_NOMOVE);
	CMenu menu;
	menu.LoadMenu(XAVI_MENU);
	SetMenu(&menu);
	menu.Detach();
	m_input.SetFocus();
}

void EvalWindow::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	m_input.SetWindowPos(NULL, 8, 8, cx - 16, 24, SWP_NOZORDER);
	m_output.SetWindowPos(NULL, 8, 36, cx - 124, 24, SWP_NOZORDER);
	m_button.SetWindowPos(NULL, cx - 108, 36, 100, 28, SWP_NOZORDER);
}

BOOL EvalWindow::PreTranslateMessage(MSG *Message)
{
	CFrameWnd::PreTranslateMessage(Message);
	if (CWnd::FromHandle(Message->hwnd) == &m_input
		&& Message->message == WM_KEYDOWN
		&& Message->wParam == VK_RETURN)
	{
		PostMessage(WM_COMMAND, 102, 0);
		return TRUE;
	}

	return FALSE;
}

void EvalWindow::OnSizing(UINT Edge, LPRECT Area)
{
	CFrameWnd::OnSizing(Edge, Area);
	
	switch (Edge)
	{
	case WMSZ_TOP:
	case WMSZ_TOPLEFT:
	case WMSZ_TOPRIGHT:
		Area->top = Area->bottom - HEIGHT;
		break;
	case WMSZ_BOTTOM:
	case WMSZ_BOTTOMLEFT:
	case WMSZ_BOTTOMRIGHT:
		Area->bottom = Area->top + HEIGHT;
		break;
	}

	switch (Edge)
	{
	case WMSZ_LEFT:
	case WMSZ_TOPLEFT:
	case WMSZ_BOTTOMLEFT:
		if (Area->right - Area->left < WIDTH)
			Area->left = Area->right - WIDTH;
	case WMSZ_RIGHT:
	case WMSZ_TOPRIGHT:
	case WMSZ_BOTTOMRIGHT:
		if (Area->right - Area->left < WIDTH)
			Area->right = Area->left + WIDTH;
	}
}

void EvalWindow::Calculate()
{
	int ExpressionSize;
	char *ExpressionString;
	char *ValueString;
	Xavi::SyntaxTreeNode *Node;
	Xavi::Value Value;

	ExpressionSize = m_input.GetWindowTextLength() + 1;
	ExpressionString = (char*)GlobalAlloc(GPTR, ExpressionSize);
	m_input.GetWindowText(ExpressionString, ExpressionSize);
	
	Node = Xavi::ParseInfix(new Xavi::CStringSource(ExpressionString));
	Value = Node->Evaluate();
	std::free(Node);
	GlobalFree((HANDLE)ExpressionString);
	
	ValueString = Value.ToCString();
	m_output.SetWindowText(ValueString);
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
