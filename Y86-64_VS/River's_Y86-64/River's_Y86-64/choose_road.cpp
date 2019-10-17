#include<iostream>
#include "choose_road.h"
#include "twelve_instr.h"
#include "initialize.h"


using namespace std;

void choose_road(int point, char stage)
{
	switch (instructions[point][7])
	{
	case '0':halt(point, stage); break;
	case '1':nop(point, stage);  break;
	case '2':rrmovq(point, stage); break;
	case '3':irmovq(point, stage); break;
	case '4':rmmovq(point, stage); break;
	case '5':mrmovq(point, stage); break;
	case '6':OPq(point, stage);   break;
	case '7':jXX(point, stage);   break;
	case '8':call(point, stage);  break;
	case '9':ret(point, stage);   break;
	case 'a':
	case 'A':pushq(point, stage); break;
	case 'b':
	case 'B':popq(point, stage); break;
	case 'c':
	case 'C':iaddq(point, stage);break;
	default: nop(point, 'X');    instr_nums[13]++;  break;
	}
	return;
}