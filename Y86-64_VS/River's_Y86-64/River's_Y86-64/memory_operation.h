#ifndef M_O_H
#define M_O_H
#include<iostream>
using namespace std;

struct Cache{
	int avail=0; //该组cache中空的副本位置下标（<4）
	pair<long long, long long> copy[4];
};
extern Cache cache[4];
//由于内存储存形式的不同，这里我的cache没有按照书本的实现，而是通过4组cache来模拟，每组可放4个值

long long read_from_memory(long long locate);

void write_to_memory(long long locate, long long value);

#endif