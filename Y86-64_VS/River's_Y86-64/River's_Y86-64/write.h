#ifndef WRITE_H
#define WRITE_H
#include<string>

using std::string;

extern string writepath;

void clear_result();

void writestr(string information);

void writelong(long long integer);

void writelong_16(long long integer);

void writeint(int integer);

#endif