#ifndef PIPE_REGISTER_H
#define PIPE_REGISTER_H
#include<string>
using std::string;

//F:
extern long long predPC;
//D:
extern string d_stat;  extern char d_icode;  extern int d_ifun, rA, rB;     extern long long d_valC, d_valP;
extern string D_stat;  extern char D_icode;  extern int  D_ifun, srcA, srcB;extern long long D_valC, D_valP;
//E:
extern string e_stat;  extern char e_icode;  extern int e_ifun;extern long long e_valC, e_valA, e_valB; extern int e_dstE, e_dstM; extern int e_srcA, e_srcB;
extern string E_stat;  extern char E_icode;  extern int E_ifun;extern long long E_valC, E_valA, E_valB; extern int E_dstE, E_dstM; extern int E_srcA, E_srcB;
//M:
extern string m_stat;  extern char m_icode;         extern int m_cnd;extern long long m_valE, m_valA;  extern int m_dstE, m_dstM;
extern string M_stat;  extern char M_icode;         extern int M_cnd;extern long long M_valE, M_valA;  extern int M_dstE, M_dstM;
//W:
extern string w_stat;  extern char w_icode;         extern  long long  w_valE, w_valM;  extern int w_dstE, w_dstM;
extern string W_stat;  extern char W_icode;         extern  long long  W_valE, W_valM;  extern int W_dstE, W_dstM;

#endif