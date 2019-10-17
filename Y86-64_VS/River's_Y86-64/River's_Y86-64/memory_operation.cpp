#include "memory_operation.h"
#include "initialize.h"
#include<iostream>
#include<time.h>
#include<Windows.h>
using namespace std;
/*
struct Cache{
	int avail; //����cache�пյĸ���λ���±꣨<4��
	pair<long long, long long> copy[4];
}; 
//�����ڴ洢����ʽ�Ĳ�ͬ�������ҵ�cacheû�а����鱾��ʵ�֣�����ͨ��4��cache��ģ�⣬ÿ��ɷ�4��ֵ
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
	long long value = Memory[locate];//���ڴ�
	if (cache[index].avail <= 3)//������п�λ
	{
		cache[index].copy[cache[index].avail] = make_pair(locate, value);
		cache[index].avail++;
	}
	else
		cache[index].copy[0] = make_pair(locate, value);//���ǵ�һ��
	return value;
}

//����ֱд �� ��д����
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
	if (cache[index].avail <= 3)//������п�λ��д��ȥ
	{
		cache[index].copy[cache[index].avail] = make_pair(locate, value);
		cache[index].avail++;
	}
	else
		cache[index].copy[0] = make_pair(locate, value);//���ǵ�һ��
	Memory[locate] = value;
	return;
}