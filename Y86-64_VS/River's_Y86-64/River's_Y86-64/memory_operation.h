#ifndef M_O_H
#define M_O_H
#include<iostream>
using namespace std;

struct Cache{
	int avail=0; //����cache�пյĸ���λ���±꣨<4��
	pair<long long, long long> copy[4];
};
extern Cache cache[4];
//�����ڴ洢����ʽ�Ĳ�ͬ�������ҵ�cacheû�а����鱾��ʵ�֣�����ͨ��4��cache��ģ�⣬ÿ��ɷ�4��ֵ

long long read_from_memory(long long locate);

void write_to_memory(long long locate, long long value);

#endif