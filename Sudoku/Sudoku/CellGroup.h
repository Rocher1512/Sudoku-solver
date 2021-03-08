#pragma once
#include"Cell.h"

class CellGroup
{
public:
	CellGroup(Cell* list[], const int type, const int place);
	~CellGroup();

	Cell* Getcell(int i) const;
private:
	Cell* m_cells[9];
};

