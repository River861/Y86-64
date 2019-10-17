#include<iostream>
#include<string>
#include<map>
#include<thread>
#include<mutex>
#include "initialize.h"
#include "read.h"
using namespace std;

bool print_cache=false;

long long max_location;

int speed;

bool No_cache;

vector<string> instructions;

mutex mt;

bool isorder = false;

bool isbreak = false;

//simulate registers 
long long rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi, r8, r9, r10, r11, r12, r13, r14, no_data;

//simulate memory 
map<long long, long long> Memory;

map<long long, long long> changed_Memory;

long long PC;
//saved CC,to deal with HLT,ADR
int saved_CC[3];

bool no_pro=true;
//condition code
int ZF = 1, SF = 0, OF = 0;

string Stat = "AOK";

string error = "None";

string ALU_stat;

string WB_stat;

string Mem_stat;

unordered_map<long long, int> location_point;

string ID_register[17] = { "%rax", "%rcx", "%rdx", "%rbx", "%rsp", "%rbp", "%rsi ", "%rdi ", "%r8  ", "%r9  ", "%r10", "%r11", "%r12", "%r13", "%r14", "----","---" };

string ID_OPq[4] = { "addq", "subq", "andq", "xorq" };

string ID_jXX[7] = { "jmp", "jle", "jl", "je", "jne", "jge", "jg" };

string ID_mov[7] = {"rrmovq", "cmovle","cmovl","cmove","cmovne","cmovge","cmovg"};

string ID_instr[14] = {"halt  ","nop   ","rrmovq","irmovq","rmmovq","mrmovq","OPq   ","jXX   ","call    ","ret       ","pushq ","popq     ","iaddq    ","INS    "};

int instr_nums[14];

long long* register_seeker(int ID)
{
	switch (ID)
	{
	case 0: return &rax;
	case 1: return &rcx;
	case 2: return &rdx;
	case 3: return &rbx;
	case 4: return &rsp;
	case 5: return &rbp;
	case 6: return &rsi;
	case 7: return &rdi;
	case 8: return &r8;
	case 9: return &r9;
	case 10:return &r10;
	case 11:return &r11;
	case 12:return &r12;
	case 13:return &r13;
	case 14:return &r14;
	default: return &no_data;
	}
}

int char_int(char c)
{
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return 0;
}

bool check_r(int point)
{
	for (int i = 0; i<2; i++)
	{
		int ID = char_int(instructions[point][9 + i]);
		if (ID<0 || ID>15) return false;
	}
	return true;
}

long long complement(string &str)                 //deal with negative valC
{
	long long ans=0;
	for (int i = 2; i < str.size(); i++)
		ans =(ans<<4)+ (15-char_int(str[i]));
	return -(ans + 1);
}

long long shift_valC(int point, int begin)
{
	string str = instructions[point].substr(begin, 16);
	string ans = "";
	int i;
	for (i = 0; i<str.size(); i += 2)
		ans = str.substr(i, 2) + ans;
	ans = "0x" + ans;
	int sign = char_int(ans[2]);
	return sign >= 8 ? complement(ans) : strtoll(&ans[0], NULL, 16);
}

void set_CC(long long t, long long a, long long b)
{
	if (t == 0) ZF = 1;
	else ZF = 0;
	if (t<0) SF = 1;
	else SF = 0;
	if ((a<0 == b<0) && (t<0 != a<0)) OF = 1;
	else OF = 0;
	return;
}