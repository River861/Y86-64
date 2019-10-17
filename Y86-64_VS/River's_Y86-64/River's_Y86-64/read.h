#ifndef READ_H
#define READ_H
#include<vector>
#include<string>
#include<unordered_map>
using std::vector;
using std::string;
using std::unordered_map;

extern vector<string> testlist;

extern string filename;

extern string readpath;

void read_filename(string order);

void read();

bool isempty(string str);

void clear_initial();

#endif