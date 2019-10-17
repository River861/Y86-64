#include<iostream>
#include<thread>
#include<mutex>
#include "five_stages.h"
#include "write.h"
#include "choose_road.h"
#include "initialize.h"
#include "outline.h"
#include "twelve_instr.h"
#include "pipe_register.h"
using namespace std;

void fetch(int point)
{
	lock_guard<mutex> lck(mt);
	if (point == -1 || point >= instructions.size())     //when point=-1, it means Bubble.
	{
		if (halt_stop == true) 	writestr("F : Fetch stage is doing nothing.          State=HLT\n");
		else  writestr("F : Fetch stage is doing nothing.          State=BUB\n");
		return;
	}
	if (PC>max_location)
	{
		error = "ADR"; no_pro = false; stop = true; return;
	}//set ADR
	if (location_point.count(PC) == 0)
	{
		if (PC<stoll(instructions[instructions.size()-1].substr(0, 5), NULL, 16))
		{
			error = "ADR"; no_pro = false; stop = true; return;
		}//set ADR
		else
		{
			writestr("F : Fetch stage is doing nothing.\n");
			return;
		}
	}
	choose_road(point, 'F');
	return;
}

void decode(int point)
{

	lock_guard<mutex> lck(mt);
	if (point == -1 || point >= instructions.size())
	{
		e_stat = "BUB"; e_icode = 0; e_ifun = 0; e_valC = e_valA = e_valB = 0; e_srcA = e_srcB = 15;
		e_dstE = e_dstM = srcA = srcB = 16;        //avoid it waits when forward is bubble
		if (halt_stop == true) 	writestr("D : Decode stage is doing nothing.      State=HLT\n");
		else  writestr("D : Decode stage is doing nothing.      State=BUB\n");
		return;
	}
	if (D_stat == "INS")                           //deal with invalid instructions
	{
		writestr("D : A wrong instruction.                       State=INS\n");
		error = "INS";
		return;
	}
	choose_road(point, 'D');
	return;
}

void execute(int point)
{
	lock_guard<mutex> lck(mt);
	if (point == -1 || point >= instructions.size())
	{
		ALU_stat = "    --------    ";
		m_stat = "BUB"; m_icode = 0; m_valE = m_valA = 0;
		m_dstE = m_dstM = 16;
		if (halt_stop == true) 	writestr("E : Execute stage is doing nothing.      State=HLT\n");
		else  writestr("E : Execute stage is doing nothing.      State=BUB\n");
		return;
	}
	if (E_stat == "INS")
	{
		writestr("E : A wrong instruction.                       State=INS\n");
		error = "INS";
		return;
	}
	choose_road(point, 'E');
	return;
}

void memory(int point)
{
	lock_guard<mutex> lck(mt);

	if (point == -1 || point >= instructions.size())
	{
		Mem_stat = "    --------    ";
		w_stat = "BUB"; w_icode = 0; w_valE = w_valM = 0;
		w_dstE = w_dstM = 16;
		if (halt_stop == true) 	writestr("M: Memory stage is doing nothing.     State=HLT\n");
		else  writestr("M: Memory stage is doing nothing.     State=BUB\n");
		return;
	}
	if (M_stat == "INS")
	{
		writestr("M: A wrong instruction.                       State=INS\n");
		error = "INS";
		return;
	}
	choose_road(point, 'M');
	return;
}

void write_back(int point)
{
	lock_guard<mutex> lck(mt);
	if (point == -1)
	{
		WB_stat = "    --------    ";
		if (halt_stop == true) 	writestr("W: Write-back stage is doing nothing. State=HLT\n");
		else writestr("W: Write-back stage is doing nothing. State=BUB\n");
		return;
	}
	else if (point >= instructions.size())
	{
		WB_stat = "    --------    ";
		stop = true;
		if (halt_stop == true) 	writestr("W: Write-back stage is doing nothing. State=HLT\n");
		else writestr("W: Write-back stage is doing nothing. State=BUB\n");
		return;
	}
	if (W_stat == "INS")
	{
		writestr("W: A wrong instruction.                       State=INS\n");
		error = "INS";
		return;
	}
	choose_road(point, 'W');
	return;
}