#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include "read.h"
#include "initialize.h"
using namespace std;

vector<string> testlist;

string filename;

string readpath="../../file/test cases/";

void read_filename(string order)
{
	string testname;
	cout << endl;
 //   cout << "All right. Please wait for a moment." << endl;
	ifstream file(readpath + "testlist.txt");
	if (!file)
	{
		cout << "open testlist fail." << endl;
		exit(0);
	}
	else
	{
		while (getline(file, testname)) testlist.push_back(testname);
		file.close();
		return;
	}
}

bool isempty(string str)
{
	int i;
	for (i = 7; i<str.size(); i++)
	if (str[i] != ' ') return false;
	return true;
}

void read()
{
	string useful_instr, useless_instr;
	ifstream file(readpath+filename.c_str());
	if (!file)
	{
		cout << "open " << filename << " fail." << endl;
		exit(0);
	}
	cout << "open "+filename+" successfully.  Loading..." << endl;
	while (getline(file, useful_instr, '|'))
	{
		getline(file, useless_instr);
		if (!isempty(useful_instr))
		{
			instructions.push_back(useful_instr);
			long long locate = stoll(useful_instr.substr(0, 5), NULL, 16);
			location_point[locate] = instructions.size() - 1;
			Memory[locate] = shift_valC(instructions.size()-1,7);
		}
	}

	file.close();
	return;
}