#include "pipe_register.h"
#include<iostream>
#include<string>
using namespace std;

//F:
long long predPC;
//D:
string d_stat = "AOK"; char d_icode; int d_ifun;  int rA = 15, rB = 15; long long d_valC = 0, d_valP = 0;
string D_stat = "AOK"; char D_icode; int D_ifun;  int srcA, srcB;       long long  D_valC, D_valP;
//E:
string e_stat = "AOK"; char e_icode; int e_ifun;  long long e_valC = 0, e_valA = 0, e_valB = 0; int e_dstE = 15, e_dstM = 15; int e_srcA = 15, e_srcB = 15;
string E_stat = "AOK"; char E_icode; int E_ifun;  long long E_valC, E_valA, E_valB;          int E_dstE, E_dstM;               int E_srcA = 15, E_srcB = 15;
//M:
string m_stat = "AOK"; char m_icode;         int m_cnd = 1;long long m_valE = 0, m_valA = 0; int m_dstE = 15, m_dstM = 15;
string M_stat = "AOK"; char M_icode;         int M_cnd = 1;long long M_valE, M_valA;        int M_dstE, M_dstM;
//W:
string w_stat = "AOK"; char w_icode;         long long   w_valE = 0, w_valM = 0;   int w_dstE = 15, w_dstM = 15;
string W_stat = "AOK"; char W_icode;         long long   W_valE, W_valM;           int W_dstE, W_dstM;