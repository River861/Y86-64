#include<iostream>
#include<Windows.h>
#include<string>
#include<thread>
#include<mutex>
#include<map>
#include<time.h>
#include<cstdio>
#include "outline.h"
#include "initialize.h"
#include "pipe_register.h"
#include "five_stages.h"
#include "write.h"
#include "twelve_instr.h"
#include "memory_operation.h"

using namespace std;

bool stop;

void transmit()
{
	//D: 
	D_stat = d_stat; D_icode = d_icode; D_ifun = d_ifun; D_valC = d_valC; D_valP = d_valP;
	//E:
	E_stat = e_stat; E_icode = e_icode; E_ifun = e_ifun; E_valC = e_valC; E_valA = e_valA; E_valB = e_valB; E_srcA = e_srcA; E_srcB = e_srcB; E_dstE = e_dstE; E_dstM = e_dstM;
	//M:
	M_stat = m_stat; M_icode = m_icode; M_cnd = m_cnd;   M_valE = m_valE; M_valA = m_valA; M_dstE = m_dstE; M_dstM = m_dstM;
	//W£º
	W_stat = w_stat; W_icode = w_icode; W_valE = w_valE; W_valM = w_valM; W_dstE = w_dstE; W_dstM = w_dstM;
	return;
}

bool has_repeat(int a)
{
	if (a == E_dstE || a == E_dstM || a == M_dstE || a == M_dstM || a == W_dstE || a == W_dstM) return true;
	return false;
}
bool forward_icode(char c)
{
	if (c == E_icode || c == M_icode || c == W_icode) return true;
	else return false;
}
bool check_forward()
{
	switch (D_icode)
	{
	    case '0':
	    case '1':
		case '3':
		case '7':return false;
		case '2':
		case '4':
			if (has_repeat(rA)) return true;
			else return false;
		case '5':  //mrmovq
			if (has_repeat(rB)) return true;
			else return false;
		case '6':  //OPq
			if (has_repeat(rA) || has_repeat(rB)) return true;
			else return false;

		case 'A':  //push
		case 'a':
			if (has_repeat(rA)) return true;
		case '8':  //call
		case '9':  //ret
		case 'B':  //pop
		case 'b':
			if (has_repeat(4)||forward_icode(8)||forward_icode('A')||forward_icode('B')) return true;//%rsp
			else return false;
        
		default: return false;
	}
}

void pipeline(string name)
{
	writepath = "../../file/cache state/";
	clear_result();
	writepath = "../../file/test process/";
	clear_result();
//use to realize pipeline
	PC = stoll(instructions[0].substr(0, 5), NULL, 16);
	int point_f = 0, point_d = -1, point_e = -1, point_m = -1, point_w = -1;
	ZF = 1; SF = OF = 0;
	int i = -1;
//timer begin.
	clock_t start, finish;
	double totaltime;
	start = clock();
//pipeline begin.
	if (!isbreak)
	{
		writestr("-----------------------------------------------------------------------------------------------------");
		writestr("\n\n\n######## This is the beginning of " + name + " ##########\n\n\n");
		writestr("-----------------------------------------------------------------------------------------------------\n");
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			writestr("-------------------------------cache " + to_string(i + 1) + ":------------------------------\n");
			for (int j = 0; j < cache[i].avail; j++)
				writestr("| "), writelong_16(cache[i].copy[j].first), writestr("  <-- "), writelong_16(cache[i].copy[j].second), writestr(" |\n");
			for (int j = cache[i].avail; j < 4; j++)
				writestr("|    empty                                                          |\n");
			writestr("-------------------------------------------------------------------------\n\n");
		}
		writestr("\n\n\n-----------------------------------------------------------------------------------------------------");
		writestr("\n\n\n############# It's a break point here. #################\n\n\n");
		writestr("-----------------------------------------------------------------------------------------------------\n\n\n");
	}
	while (1)
	{
		Sleep(20*(5-speed));
//five pipelines run in the same time.
		writestr("***********************Circle " + to_string(++i) + "**************************\n");
		writestr("ZF=" + to_string(ZF) + " SF=" + to_string(SF) + " OF=" + to_string(OF) + " Stat=" + Stat+"\n");
		thread t1(fetch, point_f);
		thread t2(decode, point_d);
		thread t3(execute, point_e);
		thread t4(memory, point_m);
		thread t5(write_back, point_w);
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
//simply descripe what have been done.
		writestr("        Execute: ALU:"+ALU_stat+"\n");
		writestr("        Memory: "+Mem_stat+"\n");
		writestr("        Writeback: "+WB_stat+"\n");
//cope with different situations.
		if (no_pro){ saved_CC[0] = ZF; saved_CC[1] = SF; saved_CC[2] = OF; }
		transmit();
		bool wait = check_forward();
		srcA = rA; srcB = rB;//put it this place to avoid it waits all the time

		if (M_cnd == 0)                //condition: jump false
		{
			point_f = location_point[predPC]; PC = predPC; point_d = -1;  point_w = point_m;  point_m = point_e; point_e = -1; 
			ret_signal = false;
			m_cnd = 1;
		}
		else if (halt_stop == true)    //condition: halt
		{
			point_f = -1;  point_d = -1;  point_w = point_m;  point_m = point_e; point_e = -1; 
		}
		else if (wait == true)         //conditon: forward
		{
			point_w = point_m;  point_m = point_e;  point_e = point_d;  point_d = -1;//fetch stage repeat the same.
			wait = false;
		}
		else if (ret_signal == true)   //condition: ret
		{
			if (get_retadd == true)
			{
				point_w = point_m;  point_m = point_e;  point_e = point_d;  point_d = point_f;
				point_f = location_point[predPC];  PC = predPC;
				ret_signal = false;
			}
			else
			{
				point_w = point_m;  point_m = point_e;  point_e = point_d;  point_d = point_f;
                point_f = -1;
			}
		}
		else
		{
			point_w = point_m;  point_m = point_e;  point_e = point_d;  point_d = point_f;  
			point_f = location_point[predPC]; PC = predPC;
		}
		if (PC > stoll(instructions[instructions.size() - 1].substr(0, 5), NULL, 16))//condition: the end is around the cornor
			point_f = -1;
//print out the cache
		print_cache = true;
		writepath = "../../file/cache state/";
		writestr("***********************Circle " + to_string(i) + "**************************\n");
		for (int i = 0; i < 4; i++)
		{
			writestr("-------------------------------cache " + to_string(i + 1) + ":------------------------------\n");
			for (int j = 0; j < cache[i].avail; j++)
				writestr("| "), writelong_16(cache[i].copy[j].first), writestr("  <-- "), writelong_16(cache[i].copy[j].second), writestr(" |\n");
			for (int j = cache[i].avail; j < 4; j++)
				writestr("|    empty                                                          |\n");
			writestr("-------------------------------------------------------------------------\n\n");
		}
		print_cache = false;
		writepath = "../../file/test process/";
		writestr("*******************************************************\n");
		writestr("\n");

//to stop the pipeline
		if (stop == true || (point_f == -1 && point_d == -1 && point_e == -1 && point_m == -1 && point_w == -1))
		{
			if (error != "None")  //set final Stat
			{
				Stat = error;
				error = "None";
			}
			 writestr("######## The Y86 is stopped. Stat=" + Stat + " ##########\n\n");
			 break;
		}
	}
//timer stop.
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;

	writepath = "../../file/test result/";
	clear_result();
	writestr("State="+Stat+"\n");
//print CC.
	ZF = saved_CC[0]; SF = saved_CC[1]; OF = saved_CC[2];
	writestr("Condition Code:  ZF=" + to_string(ZF) + " SF=" + to_string(SF) + " OF=" + to_string(OF) + "\n");
	int flag=1;
	writestr(to_string(instructions.size()) + " codes read.\n");
	writestr(to_string(i) + " circles excuted.\n\n");
//quantities of every instructions
	writestr("The quantities of every instructions executed:\n");
	for (int k = 0; k < 5; k++)
		writestr(ID_instr[k] + ": " + to_string(instr_nums[k]) + "   ");
	writestr("\n");
	for (int k = 5; k < 10; k++)
		writestr(ID_instr[k] + ": " + to_string(instr_nums[k]) + "   ");
	writestr("\n");
	for (int k = 10; k < 14; k++)
		writestr(ID_instr[k] + ": " + to_string(instr_nums[k]) + "   ");
	writestr("\n\n");
//CPI.
	double CPI =( i+0.0) / instructions.size();
	char buffer1[10];
	sprintf_s(buffer1, "%g", CPI);
	string str1 = buffer1;
	writestr("CPI: " + str1 + "\n");
//Runtime.
	char buffer2[10];
	sprintf_s(buffer2,"%g",totaltime);
	string str2 = buffer2;
	writestr("Runtime: " + str2+ " s\n");
//print max memory and speed level.
	long long m = max_location / 1024 / 1024;
	writestr("Memory Size: "+to_string(m)+"MB\n");
	writestr("Speed Level: "+to_string(speed)+"\n\n");
//printf the register & Memory.
	writestr("Changed Register State:\n");
	for (i = 0; i < 15; i++)
		if (*(register_seeker(i)) != 0)
		{
			writestr(ID_register[i] + ":   "); writelong_16(0); writestr("      "); writelong_16(*(register_seeker(i))); writestr("\n");
			flag = 0;
		}
	if (flag) writestr("None.\n");

	writestr("Changed Memory State:\n");
	flag = 1;
	for (map<long long, long long>::iterator it = changed_Memory.begin(); it != changed_Memory.end(); it++)
	{
		if (Memory[it->first] == 0) continue;
		writelong(it->first); writestr(":  "); writelong_16(it->second); writestr("  "); writelong_16(Memory[it->first]); writestr("\n");
		flag = 0;
	}
	if (flag == 1) writestr("None.\n");
	return;
}