#include<iostream>
#include<fstream>
#include<string>
#include <iomanip>
#include "initialize.h"
#include "write.h"
#include "read.h"

string  writepath="../../file/test result/";

using namespace std;

void clear_result()
{
	if (isorder)
		return;
	ofstream file(writepath + filename+".txt");
	if (!file)
	{
		cout << "clear result fail" << endl;
		exit(0);
	}
	file.close();
	return;
}

void writestr(string information)
{
	if (isorder && !print_cache)
	{
		cout << information;
		return;
	}
	ofstream file(writepath + filename+".txt", ios::app);
	if (!file)
	{
		cout << "result writing fail" << endl;
		exit(0);
	}
	file << information;
	file.close();
	return;
}

void writelong(long long integer)
{
	if (isorder)
	{
		cout << "0x" << hex << integer;
		return;
	}
	ofstream file(writepath + filename+".txt", ios::app);
	if (!file)
	{
		cout << "result writing fail" << endl;
		exit(0);
	}
	file << "0x" << hex << integer;
	file.close();
	return;
}

void writelong_16(long long integer)
{
	if (isorder)
	{
		cout << "0x" << setw(16) << setfill('0') << hex << integer;
		return;
	}
	ofstream file(writepath + filename+".txt", ios::app);
	if (!file)
	{
		cout << "result writing fail" << endl;
		exit(0);
	}
	file << "0x" << setw(16) << setfill('0') << hex << integer;
	file.close();
	return;
}

void writeint(int integer)
{
	if (isorder)
	{
		cout << "0x" << hex << integer;
		return;
	}
	ofstream file(writepath + filename+".txt", ios::app);
	if (!file)
	{
		cout << "result writing fail" << endl;
		exit(0);
	}
	file << "0x"  << hex << integer;
	file.close();
	return;
}