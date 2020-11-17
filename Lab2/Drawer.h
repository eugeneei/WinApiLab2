#pragma once
#include "Phrases.h"

#define MAX_ROWS 10;
#define MAX_COLUMNS 10;

class Drawer
{
	const int _horizontalTab = 2;
	int _maxRows = MAX_ROWS;
	int _maxColumns = MAX_COLUMNS;
	RECT _clientSize;
	TStringsMatrix _phrases;
	int _fontHeight = 0, _verticalTab = 0, _totalHeight = 0;
	int _rowCount = 0, _columnCount = 0;
	int _horizontalBorders[10][2];
	int _verticalBorders[10][2];

	
	void DrawBorders(HDC hDc);
	void DrawTable(HDC hDc, bool isDraw);
	void PrintPhrases(HDC hDc);
	int RecalculateVerticalBorders();
	void CalculateHorizontalBorders();
	int CalculateVerticalBorders(HDC hDc);

public:
	Drawer(int rowCount, int columnCount, TStringsMatrix phrases);

	void RefreshTable(HDC hDc);
	void SetClientSize(RECT clientSize);
};

