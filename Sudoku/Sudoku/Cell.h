#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Cell
{
public:
	Cell();
	Cell(const vector<int>& candidatelist, const int value, const bool given);
	~Cell();

	//gettersetter
	int getvalue() const;
	vector<int> getcandidatelist() const;
	void setcandidatelist(const vector<int>& list);
	void setvalue(int i);

private:
	vector<int> m_candidateList;
	int m_value; 
	bool m_given;
};

