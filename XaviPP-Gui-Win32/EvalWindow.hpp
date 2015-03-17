/*
 * EvalWindow.hpp: Expression evaluation window
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

#if !defined EVAL_WINDOW_HPP
#define EVAL_WINDOW_HPP

#include <afxwin.h>

class EvalWindow : public CFrameWnd
{
public:
	EvalWindow();
	BOOL PreTranslateMessage(MSG *Message);

protected:
	void OnGetMinMaxInfo(MINMAXINFO* pMMI);
	void OnSize(UINT nType, int cx, int cy);
	void Calculate();
	void CalculatorExit();
	void HelpAbout();
	DECLARE_MESSAGE_MAP();

private:
	CStatic Output;
	CEdit Input;
	CButton Button;
};

#endif /* EVAL_WINDOW_HPP */
