#pragma once
#include "CellGroup.h"
#include "Cell.h"
class SudokuPuzzle
{
public:
	SudokuPuzzle();
	~SudokuPuzzle();

	void solve(char filenameIn[]);
	void Loadinginfile(std::ifstream &fin);
	void CreateRows();
	void CreateColumns();
	void CreaterBlocks();
	bool NakedSingles() const;
	bool HiddenSingles() const;
	int	 Pickingblock(int i) const;
	int  Pickingrow(int i) const;
	void savingthegrid() const;
	

private:
	CellGroup* m_gridRows[9];
	CellGroup* m_gridColumns[9];
	CellGroup* m_gridBlocks[9];
	Cell* m_sodokugrid[81];	
};

