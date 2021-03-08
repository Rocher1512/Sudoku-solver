#include "CellGroup.h"




CellGroup::CellGroup(Cell* list[], const int type, const int place)
{
	if (type == 1)
	{
		//Rows
		int cnt = 0;
		for (int i = place; i < place + 9; i++) {
			m_cells[cnt] = list[i];
			cnt++;
		}
	}
	else if (type == 2)
	{
		//Columns
		int cnt = 0;
		for (int i = place; i < 81; i += 9) {
			m_cells[cnt] = list[i];
			cnt++;
		}
	}
	else
	{
		//Blocks
		int cnt = 0;
		for (int i = place; i < place + 27; i += 9)
		{
			for (int j = i; j < i + 3; j++)
			{
				m_cells[cnt] = list[j];
				cnt++;
			}
		}
	}
}


CellGroup::~CellGroup()
{
}

Cell * CellGroup::Getcell(int i) const
{
	return m_cells[i];
}



