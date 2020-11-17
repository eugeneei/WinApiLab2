#pragma once
#include <vector>
#include <string>

#define EMPTY_STRING L"";

typedef std::vector<std::vector<std::wstring>> TStringsMatrix;
typedef std::vector<std::wstring> TStringList;


class Phrases
{
	/*int _rows = 0, _columns = 0;*/
	std::wstring _emptyString = EMPTY_STRING;
public:

	TStringsMatrix phrases;

	Phrases();
	
};

