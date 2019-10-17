#ifndef OUTPUT_FORMAT_H
#define OUTPUT_FORMAT_H
using namespace std;

void output_F(long long predPC);

void output_D(string instr, int rA, int rB, long long valC, long long valP, string stat);

void output_E(string instr, long long valC, long long valA, long long valB,int srcA,int srcB, int dstE, int dstM, string stat);

void output_M(string instr, int cnd, long long valE, long long valA, int dstE, int dstM, string stat);

void output_W(string instr, long long valE, long long valM, int dstE, int dstM, string stat);

#endif