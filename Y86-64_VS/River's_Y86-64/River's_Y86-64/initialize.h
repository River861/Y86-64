#ifndef REGISTER_H
#define REGISTER_H
#include<map>
#include<mutex>
#include<string>
#include<vector>
#include<unordered_map>
using std::map;
using std::string;
using std::vector;
using std::unordered_map;
using std::mutex;

extern bool print_cache;

extern long long max_location;

extern int speed;

extern bool No_cache;

extern vector<string> instructions;

extern mutex mt;

extern bool isorder;

extern bool isbreak;

extern long long rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi, r8, r9, r10, r11, r12, r13, r14, no_data;

extern map<long long, long long> Memory;

extern map<long long,long long> changed_Memory;

extern long long PC;

extern int saved_CC[3];

extern bool no_pro;

extern int ZF, SF, OF;

extern string Stat;

extern string error;

extern string ALU_stat;

extern string WB_stat;

extern string Mem_stat;

extern unordered_map<long long, int> location_point;

extern string ID_register[17];

extern string ID_OPq[4];

extern string ID_jXX[7];

extern string ID_mov[7];

extern string ID_instr[14];

extern int instr_nums[14];

long long* register_seeker(int ID);

int char_int(char c);

bool check_r(int point);

long long complement(string &str);

long long shift_valC(int point, int begin);

void set_CC(long long t, long long a, long long b);

#endif