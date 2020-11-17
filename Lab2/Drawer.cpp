#include "stdafx.h"
#include "Drawer.h"


Drawer::Drawer(int rowCount, int columnCount, TStringsMatrix phrases)
{
	if (rowCount > _maxRows)
	{
		_rowCount = _maxRows;
	}
	else
	{
		_rowCount = rowCount;
	}

	if (columnCount > _maxColumns)
	{
		_columnCount = _maxColumns;
	}
	else
	{
		_columnCount = columnCount;
	}
	_phrases = phrases;
}

void Drawer::SetClientSize(RECT clientSize)
{
	_clientSize.bottom = clientSize.bottom;
	_clientSize.top = clientSize.top;
	_clientSize.left = clientSize.left;
	_clientSize.right = clientSize.right;
}

void Drawer::RefreshTable(HDC hDc)
{
	_fontHeight = 15;
	_verticalTab = 0;
	CalculateHorizontalBorders();
	DrawTable(hDc, false);

	while (_clientSize.bottom - _totalHeight > 5)
	{
		_fontHeight++;
		CalculateHorizontalBorders();
		DrawTable(hDc, false);
	}

	while (_totalHeight > _clientSize.bottom)
	{
		_fontHeight--;
		CalculateHorizontalBorders();
		DrawTable(hDc, false);
	}

	while (_clientSize.bottom - _totalHeight > 5)
	{
		_totalHeight = RecalculateVerticalBorders();
		_verticalTab++;
	}

	DrawTable(hDc, true);
}

void Drawer::DrawBorders(HDC hDc)
{
	for (int i = 0; i < _rowCount - 1; i++)
	{
		MoveToEx(hDc, _clientSize.left, _verticalBorders[i][1], nullptr);
		LineTo(hDc, _clientSize.right, _verticalBorders[i][1]);
	}
	for (int i = 0; i < _columnCount - 1; i++)
	{
		MoveToEx(hDc, _horizontalBorders[i][1] - _horizontalTab, 0, nullptr);
		LineTo(hDc, _horizontalBorders[i][1] - _horizontalTab, _clientSize.bottom);
	}
}

void Drawer::PrintPhrases(HDC hDc)
{
	RECT rect;

	for (int i = 0; i < _rowCount; i++)
	{
		for (int j = 0; j < _columnCount; j++)
		{
			rect.top = _verticalBorders[i][0] + _verticalTab / 2;
			rect.bottom = _verticalBorders[i][1];
			rect.left = _horizontalBorders[j][0];
			rect.right = _horizontalBorders[j][1];
			//SetTextColor(hDc, RGB(255, 255, 100));
			DrawText(hDc, _phrases[i][j].c_str(), -1, &rect, DT_WORDBREAK | DT_WORD_ELLIPSIS);
		}
	}
}

int Drawer::CalculateVerticalBorders(HDC hDc)
{
	RECT rect;
	rect.top = 0;

	int totalHeight = 0;
	_verticalBorders[0][0] = 0;
	for (int i = 0; i < _rowCount; i++)
	{
		if (i > 0)
		{
			_verticalBorders[i][0] = _verticalBorders[i - 1][1];
		}
		int rowMaxHeight = 0;
		for (int j = 0; j < _columnCount; j++)
		{
			rect.bottom = INT_MAX;
			rect.left = _horizontalBorders[j][0];
			rect.right = _horizontalBorders[j][1];
			DrawText(hDc, _phrases[i][j].c_str(), -1, &rect, DT_CALCRECT | DT_WORDBREAK | DT_WORD_ELLIPSIS);
			rowMaxHeight = max(rowMaxHeight, rect.bottom);
		}
		totalHeight += rowMaxHeight;
		if (i > 0)
		{
			_verticalBorders[i][1] = _verticalBorders[i - 1][1] + rowMaxHeight;
		}
		else
		{
			_verticalBorders[0][1] = rowMaxHeight;
		}
	}
	return totalHeight;
}

int Drawer::RecalculateVerticalBorders()
{
	int counter = 1;
	int totalHeight = 0;
	for (int i = 0; i < _rowCount; i++)
	{
		if (i > 0)
		{
			_verticalBorders[i][0] += counter - 1;
		}
		_verticalBorders[i][1] += counter;

		counter++;
	}
	totalHeight += _verticalBorders[_rowCount - 1][1];

	return totalHeight;
}

void Drawer::CalculateHorizontalBorders()
{
	const auto step = (_clientSize.right - _clientSize.left) / _columnCount;
	for (int i = 0; i < _columnCount; i++)
	{
		_horizontalBorders[i][0] = i * step;
		_horizontalBorders[i][1] = (i + 1) * step;
	}
}

void Drawer::DrawTable(HDC hDc, bool isDraw)
{
	auto font = CreateFont(_fontHeight, 0, 0, 0, FW_NORMAL, false, false, false, 0, 0, 0, 0, 0, nullptr);
	SelectObject(hDc, font);
	if (isDraw)
	{
		PrintPhrases(hDc);
		DrawBorders(hDc);
	}
	else
	{
		_totalHeight = CalculateVerticalBorders(hDc);
	}
	DeleteObject(font);
}

