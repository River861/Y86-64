#include<iostream>
#include<string>
#include<thread>
#include<mutex>
#include "output_format.h"
#include "write.h"
#include "initialize.h"
#include "pipe_register.h"
using namespace std;
mutex mt1, mt2, mt3, mt4, mt5;

void output_F(long long predPC)
{
	unique_lock<mutex> lck1(mt1, std::defer_lock);
	unique_lock<mutex> lck2(mt2, std::defer_lock);
	unique_lock<mutex> lck3(mt3, std::defer_lock);
	unique_lock<mutex> lck4(mt4, std::defer_lock);
	unique_lock<mutex> lck5(mt5, std::defer_lock);
	lock(lck1, lck2, lck3, lck4, lck5);

	writestr("F :  predPC="); writelong(predPC); writestr("\n");
	return;
}

void output_D(string instr, int rA, int rB, long long valC, long long valP, string stat)
{
	unique_lock<mutex> lck1(mt1, std::defer_lock);
	unique_lock<mutex> lck2(mt2, std::defer_lock);
	unique_lock<mutex> lck3(mt3, std::defer_lock);
	unique_lock<mutex> lck4(mt4, std::defer_lock);
	unique_lock<mutex> lck5(mt5, std::defer_lock);
	lock(lck2, lck3, lck4, lck5, lck1);

	writestr("D : instr=" + instr + "  rA=" + ID_register[rA] + "  rB=" + ID_register[rB]);
	writestr("  valC="); writelong(D_valC);
	writestr("  valP="); writelong(D_valP);
	writestr("  State=" + D_stat);
	writestr("\n");
	return;
}

void output_E(string instr, long long valC, long long valA, long long valB, int srcA, int srcB, int dstE, int dstM, string stat)
{
	unique_lock<mutex> lck1(mt1, std::defer_lock);
	unique_lock<mutex> lck2(mt2, std::defer_lock);
	unique_lock<mutex> lck3(mt3, std::defer_lock);
	unique_lock<mutex> lck4(mt4, std::defer_lock);
	unique_lock<mutex> lck5(mt5, std::defer_lock);
	lock(lck3, lck4, lck5, lck1, lck2);

	writestr("E : instr=" + instr);
	writestr("       valC="); writelong(valC);
	writestr("   valA="); writelong(valA);
	writestr("  valB="); writelong(valB); writestr("\n");
	writestr("      srcA=" + ID_register[srcA] + "  srcB=" + ID_register[srcB] + "  dstE=" + ID_register[dstE] + "  dstM=" + ID_register[dstM] + "  State=" + stat);
	writestr("\n");
	return;
}

void output_M(string instr, int cnd, long long valE, long long valA, int dstE, int dstM, string stat)
{
	unique_lock<mutex> lck1(mt1, std::defer_lock);
	unique_lock<mutex> lck2(mt2, std::defer_lock);
	unique_lock<mutex> lck3(mt3, std::defer_lock);
	unique_lock<mutex> lck4(mt4, std::defer_lock);
	unique_lock<mutex> lck5(mt5, std::defer_lock);
	lock(lck4, lck5, lck1, lck2, lck3);

	writestr("M: instr=" + instr);
	writestr("  Cnd=" + to_string(cnd));
	writestr("  valE="); writelong(valE);
	writestr("  valA="); writelong(valA);
	writestr("  dstE=" + ID_register[dstE] + "  dstM=" + ID_register[dstM] + "  State=" + stat);
	writestr("\n");
	return;
}

void output_W(string instr, long long valE, long long valM, int dstE, int dstM, string stat)
{
	unique_lock<mutex> lck1(mt1, std::defer_lock);
	unique_lock<mutex> lck2(mt2, std::defer_lock);
	unique_lock<mutex> lck3(mt3, std::defer_lock);
	unique_lock<mutex> lck4(mt4, std::defer_lock);	
	unique_lock<mutex> lck5(mt5, std::defer_lock);
	lock(lck5, lck1, lck2, lck3, lck4);

	writestr("W: instr=" + instr);
	writestr("   valE="); writelong(valE);
	writestr("   valM="); writelong(valM);
	writestr("  dstE=" + ID_register[dstE] + "  dstM=" + ID_register[dstM] + "  State=" + stat);
	writestr("\n");
	return;
}