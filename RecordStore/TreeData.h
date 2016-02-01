#pragma once
#include <fstream>
using namespace std;
class TreeData
{
friend ostream& operator<<(ostream &OutStream, TreeData& Data);
private:
	char data;
public:
	TreeData();
	~TreeData();

	TreeData(char ch) { data = ch; }
	TreeData(const TreeData& Data);
	const TreeData& operator=(const TreeData& Data);
	bool operator<(const TreeData& Data) const { return (data < Data.data); }
	bool operator==(const TreeData& Data) const { return (data == Data.data); }
	bool operator!=(const TreeData& Data) const { return !(data == Data.data); }
};

