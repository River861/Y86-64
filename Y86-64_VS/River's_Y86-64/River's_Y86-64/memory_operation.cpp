#include "memory_operation.h"
#include "initialize.h"
#include<iostream>
#include<time.h>
#include<Windows.h>
using namespace std;
/*
struct Cache{
	int avail; //该组cache中空的副本位置下标（<4）
	pair<long long, long long> copy[4];
}; 
//由于内存储存形式的不同，这里我的cache没有按照书本的实现，而是通过4组cache来模拟，每组可放4个值
*/
Cache cache[4];
int get_group(long long locate)
{
	return (locate & 0x30)>>4;
}

long long read_from_memory(long long locate)
{
	if (No_cache)
	{
		Sleep(300);
		return Memory[locate];
	}
	int index = get_group(locate);
//hit
	for (int i = cache[index].avail - 1; i >= 0;--i)
		if (cache[index].copy[i].first == locate) return cache[index].copy[i].second;
//miss
	Sleep(5);
	long long value = Memory[locate];//读内存
	if (cache[index].avail <= 3)//如果还有空位
	{
		cache[index].copy[cache[index].avail] = make_pair(locate, value);
		cache[index].avail++;
	}
	else
		cache[index].copy[0] = make_pair(locate, value);//覆盖第一个
	return value;
}

//采用直写 和 非写分配
void write_to_memory(long long locate, long long value)
{	
	if (No_cache)
	{
		Sleep(300);
		Memory[locate] = value;
		return;
	}
	int i;
	int index = get_group(locate);
//hit
	for (int i = cache[index].avail - 1; i >= 0; --i)
	if (cache[index].copy[i].first == locate) cache[index].copy[i].second = value;
//miss
	Sleep(5);
	if (cache[index].avail <= 3)//如果还有空位就写进去
	{
		cache[index].copy[cache[index].avail] = make_pair(locate, value);
		cache[index].avail++;
	}
	else
		cache[index].copy[0] = make_pair(locate, value);//覆盖第一个
	Memory[locate] = value;
	return;
}