#ifndef OPERATIONS_H
#define OPERATIONS_H
using namespace std;

extern bool halt_stop;
extern bool call_signal;
extern bool ret_signal;
extern bool get_retadd;

void halt(int point, char stage);

void nop(int point, char stage);

void rrmovq(int point, char stage);

void irmovq(int point, char stage);

void rmmovq(int point, char stage);

void mrmovq(int point, char stage);

void OPq(int point, char stage);

void jXX(int point, char stage);

void call(int point, char stage);

void ret(int point, char stage);

void pushq(int point, char stage);

void popq(int point, char stage);

void iaddq(int point, char stage);

#endif