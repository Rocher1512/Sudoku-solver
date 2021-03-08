#include "SudokuPuzzle.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <chrono>

int timeschecked{};
int cellssolved{};

SudokuPuzzle::SudokuPuzzle()
{
	m_gridRows;
	m_gridColumns;
	m_gridBlocks;
	m_sodokugrid;
	for (int i = 0; i < 81; i++) {
		int number = 0;
		bool given = false;
		Cell* c;
		int &number2 = number;
		vector<int> &candidatelist = *new vector<int>{ number2 };
		c = new Cell(candidatelist, number, given);
		m_sodokugrid[i] = c;
	}
	int cnt = 0;
	CellGroup* CG;
	for (int i = 0; i < 81; i += 9)
	{
		CG = new CellGroup(m_sodokugrid, 1, i);
		m_gridRows[cnt] = CG;
		cnt++;
	}
	for (int i = 0; i < 9; i++)
	{
		CG = new CellGroup(m_sodokugrid, 2, i);
		m_gridColumns[i] = CG;
	}
	cnt = 0;
	for (int i = 0; i < 81; i += 18)
	{
		const int j = i + 7;
		for (i = i; i < j; i += 3) {
			CG = new CellGroup(m_sodokugrid, 3, i);
			m_gridBlocks[cnt] = CG;
			cnt++;
		}
	}
}

SudokuPuzzle::~SudokuPuzzle()
{
}

void SudokuPuzzle::solve(char filenameIn[])
{
	//load in the grid 
	ifstream fin(filenameIn);
	Loadinginfile(fin);
	CreateRows();
	CreateColumns();
	CreaterBlocks();
	bool check = true;
	bool hiddencheck = true;
	int amountofrunthoughs = 0;
	const auto startTime = std::chrono::high_resolution_clock::now();
	while (true)
	{
		check = NakedSingles();
		hiddencheck = HiddenSingles();
		amountofrunthoughs += 2;
		if (hiddencheck == false && check == false) 
		{
			break;
		}
	}
	const auto endTime = std::chrono::high_resolution_clock::now();
	const auto duration = (endTime - startTime).count();
	std::cout << "The time was " << duration << "ns" << std::endl;
	savingthegrid();
	cout << "The run throughs of the entire grid were " << amountofrunthoughs << endl;
	cout << "The amount of candidate values considered " << timeschecked << endl;
	cout << "The amount of cells solved " << cellssolved << endl;
	cout << "" << endl;
}

void SudokuPuzzle::Loadinginfile(std::ifstream &fin)
{
	int cnt = 0;
	while (!fin.eof()) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				int number;
				bool given = false;
				Cell* c;
				vector<int> &fullist = *new vector<int>;
				for (int i = 1; i < 10; i++) {
					fullist.push_back(i);
				}
				fin >> number;
				int &number2 = number;
				if (number != 0) {
					given = true;
					vector<int> &candidatelist = *new vector<int>{number2};
					c = new Cell(candidatelist, number, given);
					m_sodokugrid[cnt++] = c;
				}
				else 
				{
					vector<int>& candidatelist = *new vector<int>{ fullist };
					c = new Cell(candidatelist, number, given);
					m_sodokugrid[cnt++] = c;
				}
				
			}
		}
	}
}

// setting up the pointer grid
//creatE CG on the heap then point to it
void SudokuPuzzle::CreateRows()
{
	int cnt = 0;
	CellGroup* CG;
	for (int i = 0; i < 81; i += 9) 
	{
		CG = new CellGroup(m_sodokugrid, 1, i);
		m_gridRows[cnt] = CG;
		cnt++;
	}
}

void SudokuPuzzle::CreateColumns()
{
	CellGroup* CG;
	for (int i = 0; i < 9; i++) 
	{
		CG = new CellGroup(m_sodokugrid, 2, i);
		m_gridColumns[i] = CG;
	}
}

void SudokuPuzzle::CreaterBlocks()
{
	int cnt = 0;
	CellGroup* CG;
	for (int i = 0; i < 81; i += 18) 
	{
		const int j = i + 7;
		for (i = i; i < j; i += 3) {
			CG = new CellGroup(m_sodokugrid, 3, i);
			m_gridBlocks[cnt] = CG;
			cnt++;
		}
	}
}

bool SudokuPuzzle::NakedSingles() const
{
	//search through each cell
	int rownumber = 0;
	int columnnumber = 0;
	int blocknumber = 0;
	bool changed = false;
	Cell* cellchanging;
	Cell* cellcheck;
	for (int i = 0; i < 81; i++)
	{
		cellchanging = m_sodokugrid[i];
		vector<int> candidatelist = cellchanging->getcandidatelist();
		rownumber = Pickingrow(i);
		columnnumber = i % 9;
		blocknumber = Pickingblock(i);
		if (candidatelist.size() == 1) {
			continue;
		}
		//look through every row to check for any values and remove it from the candidate list if so
		for (int j = 0; j < 9; j++)
		{
			cellcheck = m_gridRows[rownumber]->Getcell(j);
			const int number = cellcheck->getvalue();
			for (unsigned int k = 0; k < candidatelist.size(); k++)
			{
				timeschecked++;
				if (candidatelist[k] == number)
				{
					candidatelist.erase(candidatelist.begin()+k);
					changed = true;
				}
			}
		}

		if (candidatelist.size() == 1) {
			cellchanging->setvalue(candidatelist[0]);
			cellssolved++;
			cellchanging->setcandidatelist(candidatelist);
			continue;
		}
		
		//repeat for columns
		for (int j = 0; j < 9; j++)
		{
			cellcheck = m_gridColumns[columnnumber]->Getcell(j);
			const int number = cellcheck->getvalue();
			for (unsigned int k = 0; k < candidatelist.size(); k++)
			{
				timeschecked++;
				if (candidatelist[k] == number)
				{
					candidatelist.erase(candidatelist.begin() + k);
					changed = true;
				}
			}
		}
		
		if (candidatelist.size() == 1) {
			cellchanging->setvalue(candidatelist[0]);
			cellssolved++;
			cellchanging->setcandidatelist(candidatelist);
			continue;
		}

		//repeat for blocks
		for (int j = 0; j < 9; j++)
		{
			cellcheck = m_gridBlocks[blocknumber]->Getcell(j);
			const int number = cellcheck->getvalue();
			for (unsigned int k = 0; k < candidatelist.size(); k++)
			{
				timeschecked++;
				if (candidatelist[k] == number)
				{
					candidatelist.erase(candidatelist.begin() + k);
					changed = true;
				}
			}
		}
		//put back the new list
		if (candidatelist.size() == 1) {
			cellchanging->setvalue(candidatelist[0]);
			cellssolved++;
		}
		cellchanging->setcandidatelist(candidatelist);
	}
	// check to makes sure at least one value was changed 
	return changed;
}

bool SudokuPuzzle::HiddenSingles() const
{
	
	//search through each cell 
	int rownumber = 0;
	int columnnumber = 0;
	int blocknumber = 0;
	bool changed = false;
	Cell* cellchanging;
	for (int i = 0; i < 81; i++)
	{
		cellchanging = m_sodokugrid[i];
		Cell* cellcheck;
		vector<int> candidatelist = cellchanging->getcandidatelist();
		rownumber = Pickingrow(i);
		columnnumber = i % 9;
		blocknumber = Pickingblock(i);
		const int blockselectioncolmn = (columnnumber % 3) + 1;
		int blockselectionrow = (((rownumber + 1) % 3) - 1) * 3;
		if (blockselectionrow == -3) {
			blockselectionrow = 6;
		}
		if (candidatelist.size() == 1) {
			continue;
		}
		// check each value in the candidate list vs all others in that row/coloumn/block if it doesnt apear in any other candidate list use that value
		for (int j = 0; j < 9; j++)
		{
			if (j == columnnumber) {
				continue;
			}
			cellcheck = m_gridRows[rownumber]->Getcell(j);
			vector<int> candidatecheck = cellcheck->getcandidatelist();
			for (unsigned int k = 0; k < candidatelist.size(); k++)
			{
				
				for (unsigned int l = 0; l < candidatecheck.size(); l++) 
				{
					timeschecked++;
					if (candidatelist.size() <= k) {
						continue;
					}
					if (candidatelist[k] == candidatecheck[l])
					{
						candidatelist.erase(candidatelist.begin() + k);
					}
				}
				
			}
		}

		if (candidatelist.size() == 1) {
			cellchanging->setvalue(candidatelist[0]);
			changed = true;
			cellchanging->setcandidatelist(candidatelist);
			cellssolved++;
			continue;
		}
		//repeat for columns
		for (int j = 0; j < 9; j++)
		{
			if (j == rownumber) {
				continue;
			}
			cellcheck = m_gridColumns[columnnumber]->Getcell(j);
			vector<int> candidatecheck = cellcheck->getcandidatelist();
			for (unsigned int k = 0; k < candidatelist.size(); k++)
			{
				for (unsigned int l = 0; l < candidatecheck.size(); l++)
				{
					timeschecked++;
					if (candidatelist.size() <= k) {
						continue;
					}
					if (candidatelist[k] == candidatecheck[l])
					{
						candidatelist.erase(candidatelist.begin() + k);
					}
				}
			}
		}

		if (candidatelist.size() == 1) {
			cellchanging->setvalue(candidatelist[0]);
			changed = true;
			cellchanging->setcandidatelist(candidatelist);
			cellssolved++;
			continue;
		}

		//repeat for blocks
		for (int j = 0; j < 9; j++)
		{
			if (j == (blockselectioncolmn + blockselectionrow)) {
				continue;
			}
			cellcheck = m_gridBlocks[blocknumber]->Getcell(j);
			vector<int> candidatecheck = cellcheck->getcandidatelist();
			for (unsigned int k = 0; k < candidatelist.size(); k++)
			{
				
				for (unsigned int l = 0; l < candidatecheck.size(); l++)
				{
					timeschecked++;
					if (candidatelist.size() <= k) {
						continue;
					}
					if (candidatelist[k] == candidatecheck[l])
					{
						candidatelist.erase(candidatelist.begin() + k);
					}
				}
			}
		}

		if (candidatelist.size() == 1) {
			cellchanging->setvalue(candidatelist[0]);
			changed = true;
			cellchanging->setcandidatelist(candidatelist);
			cellssolved++;
		}
		
	}
	return changed;
}

int SudokuPuzzle::Pickingblock(const int i) const
{
	if (i == 0 || i == 1 || i == 2 || i == 9 || i == 10 || i == 11 || i == 18 || i == 19 || i == 20) {
		return 0;
	}
	if (i == 3 || i == 4 || i == 5 || i == 12 || i == 13 || i == 14 || i == 21 || i == 22 || i == 23) {
		return 1;
	}
	if (i == 6 || i == 7 || i == 8 || i == 15 || i == 16 || i == 17 || i == 24 || i == 25 || i == 26) {
		return 2;
	}
	if (i == 27 || i == 28 || i == 29 || i == 36 || i == 37 || i == 38 || i == 45 || i == 46 || i == 47) {
		return 3;
	}
	if (i == 30 || i == 31 || i == 32 || i == 39 || i == 40 || i == 41 || i == 48 || i == 49 || i == 50) {
		return 4;
	}
	if (i == 33 || i == 34 || i == 35 || i == 42 || i == 43 || i == 44 || i == 51 || i == 52 || i == 53) {
		return 5;
	}
	if (i == 54 || i == 55 || i == 56 || i == 63 || i == 64 || i == 65 || i == 72 || i == 73 || i == 74) {
		return 6;
	}
	if (i == 57 || i == 58 || i == 59 || i == 66 || i == 67 || i == 68 || i == 75 || i == 76 || i == 77) {
		return 7;
	}
	if (i == 60 || i == 61 || i == 62 || i == 69 || i == 70 || i == 71 || i == 78 || i == 79 || i == 80) {
		return 8;
	}
	return NULL;
}

int SudokuPuzzle::Pickingrow(const int i) const
{
	if (i < 9) {
		return 0;
	}
	else if(i >= 9 && i < 18){
		return 1;
	}
	else if (i >= 18 && i < 27) {
		return 2;
	}
	else if (i >= 27 && i < 36) {
		return 3;
	}
	else if (i >= 36 && i < 45) {
		return 4;
	}
	else if (i >= 45 && i < 54) {
		return 5;
	}
	else if (i >= 54 && i < 63) {
		return 6;
	}
	else if (i >= 63 && i < 72) {
		return 7;
	}
	else if (i >= 72 && i < 81) {
		return 8;
	}
	return NULL;
}

void SudokuPuzzle::savingthegrid() const
{
	char input[] = "Sudoku_Solution.txt";
	ofstream outf(input);
	for (int i = 0; i < 81; i++)
	{
		if (i == 80) {
			cout << "" << endl;
		}
		outf <<m_sodokugrid[i]->getvalue();
		outf << " ";
		if ((i + 1) % 9 == 0 && i != 0) 
		{
			outf << endl;
		}
	}
	cout << "" << endl;
}




