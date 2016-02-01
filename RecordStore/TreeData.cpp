#include "TreeData.h"


TreeData::TreeData()
{
}


TreeData::~TreeData()
{
}

TreeData::TreeData(const TreeData& Data)
{
	data = Data.data;
}

ostream& operator<<(ostream& OutStream, TreeData& Data)
{
	OutStream << Data.data;
	return OutStream;
}

const TreeData& TreeData::operator=(const TreeData& Data)
{
	data = Data.data;
	return *this;
}

