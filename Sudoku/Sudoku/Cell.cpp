#include "Cell.h"


//constructor
Cell::Cell(const vector<int> &candidatelist, const int value, const bool given)
{
	m_candidateList = candidatelist;
	m_value = value;
	m_given = given;
}

Cell::~Cell()
{
}

int Cell::getvalue() const
{
	return m_value;
}

vector<int> Cell::getcandidatelist() const
{
	return m_candidateList;
}

void Cell::setcandidatelist(const vector<int>& list)
{
	m_candidateList = list;
}

void Cell::setvalue(const int i)
{
	m_value = i;
}

