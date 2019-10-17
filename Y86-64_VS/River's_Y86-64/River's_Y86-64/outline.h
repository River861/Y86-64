#ifndef OUTLINE_H
#define OUTLINE_H
#include<mutex>
#include<string>
using std::mutex;
using std::string;

extern bool stop;

extern int flag;

bool has_repeat(int a);

void transmit();

bool forward_icode(char c);

bool check_forward();

void pipeline(string name);

#endif