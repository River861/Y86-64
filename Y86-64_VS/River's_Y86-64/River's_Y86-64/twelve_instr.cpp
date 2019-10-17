#include<iostream>
#include<string>
#include "twelve_instr.h"
#include "write.h"
#include "output_format.h"
#include "initialize.h"
#include "pipe_register.h"
#include "outline.h"
#include "memory_operation.h"
using namespace std;

bool halt_stop = false;
int ifun;
bool call_signal = false;
bool ret_signal = false;
bool get_retadd;

void halt(int point, char stage)
{
	switch (stage)
	{
	case 'F':
		//output the state
		output_F(PC);          //the predPC in pipe_register means the current PC.
		//operate
		d_stat = "HLT";
		d_icode = instructions[point][7];
		d_ifun = instructions[point][8];
		rA = 15;
		rB = 15;
		d_valC = 0;
        d_valP = predPC = PC + 1;

		break;
	case 'D':
		//output the state
		output_D("halt     ", srcA, srcB, D_valC, D_valP, D_stat);

		//operate
		e_srcA = 15; e_srcB = 15;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = D_valC;
		e_valA = 0;
		e_valB = 0;
		e_dstE = 15;
		e_dstM = 15;

		break;
	case 'E':
		output_E("halt     ", E_valC, E_valA, E_valB, E_srcA,E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[0]++;
		//HLT makes the afterward instructions stop.
		m_stat = E_stat;
		halt_stop = true;

		m_icode = E_icode;
		m_valE = 0;
		m_valA = 0;
		m_dstE = E_dstE;
		m_dstM = E_dstM;
		ALU_stat = "    --------    ";
		break;
	case 'M':
		output_M("halt     ", M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		w_valE = M_valE;
		w_valM = M_valA;
		w_dstE = M_dstE;
		w_dstM = M_dstM;
		Mem_stat = "    --------    ";
		break;
	case 'W':
		output_W("halt     ", W_valE, W_valM, W_dstE, W_dstM, W_stat);
		WB_stat = "    --------    ";
		Stat = W_stat;

		break;
	}
}

void nop(int point, char stage)
{
	switch (stage)
	{
	case 'F':
		output_F(PC);

		d_stat = "AOK";
		d_icode = instructions[point][7];
		d_ifun = instructions[point][8];
		rA = 15;
		rB = 15;
		d_valC = 0;
		d_valP = predPC = PC + 1;

		break;
	case 'D':
		output_D("nop     ", srcA, srcB, D_valC, D_valP,D_stat);

		e_srcA = 15; e_srcB = 15;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = 0;
		e_valA = 0;
		e_valB = 0;
		e_dstE = 15;
		e_dstM = 15;

		break;
	case 'E':
		output_E("nop     ", E_valC, E_valA, E_valB, E_srcA, E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[1]++;
		m_stat = E_stat;
		m_icode = E_icode;
		m_valE = 0;
		m_valA = 0;
		m_dstE = E_dstE;
		m_dstM = E_dstM;
		ALU_stat = "    --------    ";
		break;
	case 'M':
		output_M("nop     ", M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		w_valE = M_valE;
		w_valM = M_valA;
		w_dstE = M_dstE;
		w_dstM = M_dstM;
		Mem_stat = "    --------    ";
		break;
	case 'W':
		output_W("nop     ", W_valE, W_valM, W_dstE, W_dstM, W_stat);
		WB_stat = "    --------    ";
		Stat = W_stat;

		break;
	case 'X':
		//We ignore INS in instrctions and pass this wrong instruction.
		output_F(PC);

		d_stat = "INS";
		error = "INS";
		d_icode = '-';
		d_ifun = 0;
		rA = 15;
		rB = 15;
		d_valC = 0;
		d_valP = predPC = PC + (instructions[point].size()-7)/2;

		break;
	}
}

void rrmovq(int point, char stage)
{
	switch (stage)
	{
	case 'F':
		output_F(PC);

		if (check_r(point) == false)
		{
			//if the register code is false,we stop the whole simulator.
			instr_nums[13]++;
			error = "INS";
			error = "INS";
			stop = true;
			return;
		}
		d_stat = "AOK";
		d_icode = instructions[point][7];
        d_ifun = instructions[point][8]-'0';
		rA = char_int(instructions[point][9]);
		rB = char_int(instructions[point][10]);
		d_valC = 0;
		d_valP = predPC = PC + 2;
		break;
	case 'D':
		output_D(ID_mov[D_ifun], srcA, srcB, D_valC, D_valP, D_stat);

		e_srcA = srcA; e_srcB = 15;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = 0;
		e_valA = *(register_seeker(srcA));
		e_valB = *(register_seeker(srcB));
		e_dstE = srcB;
		e_dstM = 15;

		break;
	case 'E':
		output_E(ID_mov[E_ifun], E_valC, E_valA, E_valB, E_srcA, E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[2]++;
		m_stat = E_stat;
		m_icode = E_icode;
		m_valE = E_valA + 0;
		ALU_stat = " valE = valA + 0 ";
		switch (E_ifun)
		{
		case 0:break;
		case 1:{ if (!(ZF || SF)) m_valE = E_valB; ALU_stat = "    --------    "; } break;
		case 2:{ if (!SF) m_valE = E_valB; ALU_stat = "    --------    "; } break;
		case 3:{ if (!ZF) m_valE = E_valB; ALU_stat = "    --------    "; } break;
		case 4:{ if (ZF) m_valE = E_valB; ALU_stat = "    --------    "; } break;
		case 5:{ if (SF) m_valE = E_valB; ALU_stat = "    --------    "; } break;
		case 6:{ if (ZF || SF) m_valE = E_valB; ALU_stat = "    --------    "; } break;
		default:
			instr_nums[13]++;
			error = "INS";
			error = "INS";
			stop = true;
			return;
		}
		m_valA = E_valA;
		m_dstE = E_dstE;
		m_dstM = E_dstM;

		break;
	case 'M':
		output_M(ID_mov[instructions[point][8] - '0'], M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		w_valE = M_valE;
		w_valM = M_valA;
		w_dstE = M_dstE;
		w_dstM = M_dstM;
		Mem_stat = "    --------    ";
		break;
	case 'W':
		output_W(ID_mov[instructions[point][8] - '0'], W_valE, W_valM, W_dstE, W_dstM, W_stat);

		Stat = W_stat;
		*(register_seeker(W_dstE)) = W_valE;
		WB_stat = "Wrote valE to register "+ID_register[W_dstE];
		break;
	}
}

void irmovq(int point, char stage)
{
	switch (stage)
	{
	case 'F':
		output_F(PC);

		if (check_r(point) == false)
		{
			instr_nums[13]++;
			error = "INS";
			stop = true;
			return;
		}
		d_stat = "AOK";
		d_icode = instructions[point][7];
		d_ifun = instructions[point][8];
		rA = char_int(instructions[point][9]);
		rB = char_int(instructions[point][10]);
		d_valC = shift_valC(point, 11);
		d_valP = predPC = PC + 10;

		break;
	case 'D':
		output_D("irmovq", srcA, srcB, D_valC, D_valP, D_stat);

		e_srcA = 15; e_srcB = 15;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = D_valC;
		e_valA = 0;
		e_valB = 0;
		e_dstE = srcB;
		e_dstM = 15;

		break;
	case 'E':
		output_E("irmovq", E_valC, E_valA, E_valB, E_srcA, E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[3]++;
		m_stat = E_stat;
		m_icode = E_icode;
		m_valE = E_valC + 0;
		ALU_stat = " valE = valC + 0 ";
		m_valA = 0;
		m_dstE = E_dstE;
		m_dstM = E_dstM;

		break;
	case 'M':
		output_M("irmovq", M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		w_valE = M_valE;
		w_valM = M_valA;
		w_dstE = M_dstE;
		w_dstM = M_dstM;
		Mem_stat = "    --------    ";
		break;
	case 'W':
		output_W("irmovq", W_valE, W_valM, W_dstE, W_dstM, W_stat);

		Stat = W_stat;

		*(register_seeker(W_dstE)) = W_valE;
		WB_stat = "Wrote valE to register " + ID_register[W_dstE];
		break;
	}
}

void rmmovq(int point, char stage)
{
	switch (stage)
	{
	case 'F':
		output_F(PC);

		if (check_r(point) == false)
		{
			instr_nums[13]++;
			error= "INS";
			stop = true;
			return;
		}
		d_stat = "AOK";
		d_icode = instructions[point][7];
		d_ifun = instructions[point][8];
		rA = char_int(instructions[point][9]);
		rB = char_int(instructions[point][10]);
		d_valC = shift_valC(point, 11);
		d_valP = predPC = PC + 10;

		break;
	case 'D':
		output_D("rmmovq", srcA, srcB, D_valC, D_valP, D_stat);

		e_srcA = srcA; e_srcB = 15;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = D_valC;
		e_valA = *(register_seeker(srcA));
		e_valB = *(register_seeker(srcB));
		e_dstE = 15;
		e_dstM = 15;

		break;
	case 'E':
		output_E("rmmovq", E_valC, E_valA, E_valB, E_srcA, E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[4]++;
		m_stat = E_stat;
		m_icode = E_icode;
		m_valE = E_valC + E_valB;
		ALU_stat = " valE = valC + valB ";
		m_valA = E_valA;
		m_dstE = E_dstE;
		m_dstM = E_dstE;

		break;
	case 'M':
		output_M("rmmovq", M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		w_valE = 0;
        w_valM = 0;
		if (M_valE >= 0 && M_valE<=max_location)               //condition:  the memory address is invalid.
		{
		    changed_Memory[M_valE] = Memory[M_valE];
			write_to_memory(M_valE,M_valA);
		    Mem_stat = " Wrote valA to memory  D(valE). ";
		}
		else
		{
			error = w_stat = "ADR";
			no_pro = false;
			Mem_stat = " Can't wrote to an invalid address.";
			stop = true;
		}
		w_dstE = M_dstE;
		w_dstM = M_dstM;
		break;
	case 'W':
		output_W("rmmovq", W_valE, W_valM, W_dstE, W_dstM, W_stat);

		Stat = W_stat;
		WB_stat = "    --------    ";
		break;
	}
}

void mrmovq(int point, char stage)
{
	switch (stage)
	{
	case 'F':
		output_F(PC);

		if (check_r(point) == false)
		{
			instr_nums[13]++;
			error = "INS";
			stop = true;
			return;
		}
		d_stat = "AOK";
		d_icode = instructions[point][7];
		d_ifun = instructions[point][8];
		rA = char_int(instructions[point][9]);
		rB = char_int(instructions[point][10]);
		d_valC = shift_valC(point, 11);
		d_valP = predPC = PC + 10;

		break;
	case 'D':
		output_D("mrmovq", srcA, srcB, D_valC, D_valP, D_stat);

		e_srcA = 15; e_srcB = srcB;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = D_valC;
		e_valA = 0;
		e_valB = *(register_seeker(srcB));
		e_dstE = 15;
		e_dstM = srcA;

		break;
	case 'E':
		output_E("mrmovq", E_valC, E_valA, E_valB, E_srcA, E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[5]++;
		m_stat = E_stat;
		m_icode = E_icode;
		m_valE = E_valC + E_valB;
		ALU_stat = " valE = valC + valB ";
		m_valA = E_valA;
		m_dstE = E_dstE;
		m_dstM = E_dstM;

		break;
	case 'M':
		output_M("mrmovq", M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		w_valE = 0;
		if (M_valE >= 0 && M_valE <= max_location)
		{
			w_valM=read_from_memory(M_valE);
            Mem_stat = " Read valM from memory D(valE). ";
		}
		else
		{
			error = w_stat = "ADR";
			no_pro = false;
			Mem_stat = " Can't read from an invalid address.";
			stop = true;
		}
		w_dstE = M_dstE;
		w_dstM = M_dstM;
		
		break;
	case 'W':
		output_W("mrmovq", W_valE, W_valM, W_dstE, W_dstM, W_stat);

		Stat = W_stat;
		*(register_seeker(W_dstM)) = W_valM;
		WB_stat = "Wrote valM to register " + ID_register[W_dstM];
		break;
	}
}

void OPq(int point, char stage)
{
	switch (stage)
	{
	case 'F':
		output_F(PC);

		if (check_r(point) == false)
		{
			instr_nums[13]++;
			error = "INS";
			stop = true;
			return;
		}
		d_stat = "AOK";
		d_icode = instructions[point][7];
		d_ifun = instructions[point][8] - '0';
		rA = char_int(instructions[point][9]);
		rB = char_int(instructions[point][10]);
		d_valC = 0;
		d_valP = predPC = PC + 2;

		break;
	case 'D':
		output_D(ID_OPq[D_ifun], srcA, srcB, D_valC, D_valP, D_stat);

		e_srcA = srcA; e_srcB = srcB;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = D_valC;
		e_valA = *(register_seeker(srcA));
		e_valB = *(register_seeker(srcB));
		e_dstE = srcB;
		e_dstM = 15;

		break;
	case 'E':
		output_E(ID_OPq[E_ifun], E_valC, E_valA, E_valB, E_srcA, E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[6]++;
		m_stat = E_stat;
		m_icode = E_icode;
		switch (E_ifun)
		{
		case 0:m_valE = E_valA + E_valB; ALU_stat = " valE = valA + valB "; break;
		case 1:m_valE = E_valB - E_valA; ALU_stat = " valE = valB - valA "; break;
		case 2:m_valE = E_valA&E_valB; ALU_stat = " valE = valA & valB "; break;
		case 3:m_valE = E_valA^E_valB; ALU_stat = " valE = valA ^ valB "; break;
		default:
			instr_nums[13]++;
			error = "INS";
			stop = true;
			return;
		}
		set_CC(m_valE, E_valA, E_valB);
		m_valA = 0;
		m_dstE = E_dstE;
		m_dstM = E_dstM;

		break;
	case 'M':
		output_M(ID_OPq[instructions[point][8] - '0'], M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		w_valE = M_valE;
		w_valM = M_valA;
		w_dstE = M_dstE;
		w_dstM = M_dstM;
		Mem_stat = "    --------    ";
		break;
	case 'W':
		output_W(ID_OPq[instructions[point][8] - '0'], W_valE, W_valM, W_dstE, W_dstM, W_stat);

		Stat = W_stat;
		*(register_seeker(W_dstE)) = W_valE;
		WB_stat = "Wrote valE to register " + ID_register[W_dstE];
		break;
	}
}

void jXX(int point, char stage)
{
	switch (stage)
	{
	case 'F':
		output_F(PC);

		d_valC = shift_valC(point, 9);
		if (M_cnd) predPC = d_valC;
		else
		{
			predPC = PC + 9;
			m_cnd = 1;
		}

		d_valP = PC + 9;

		d_stat = "AOK";
		d_icode = instructions[point][7];
        d_ifun = instructions[point][8] - '0';		
		rA = 15;
		rB = 15;

		break;
	case 'D':
		output_D(ID_jXX[D_ifun], srcA, srcB, D_valC, D_valP, D_stat);

		e_srcA = 15; e_srcB = 15;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = D_valC;
		e_valA = D_valP;
		e_valB = 0;
		e_dstE = 15;
		e_dstM = 15;

		break;
	case 'E':
		output_E(ID_jXX[E_ifun], E_valC, E_valA, E_valB, E_srcA, E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[7]++;
		m_stat = E_stat;
		m_icode = E_icode;
		switch (E_ifun)
		{
		case 0:break;
		case 1:if (!(ZF || SF)) { m_cnd = 0; predPC = E_valA; } break;
		case 2:if (!SF) { m_cnd = 0; predPC = E_valA; } break;
		case 3:if (!ZF) { m_cnd = 0; predPC = E_valA; } break;
		case 4:if (ZF) { m_cnd = 0; predPC = E_valA; } break;
		case 5:if (SF) { m_cnd = 0; predPC = E_valA; } break;
		case 6:if (ZF || SF) { m_cnd = 0; predPC = E_valA; } break;
		default:
			instr_nums[13]++;
			error = "INS";
			stop = true;
			return;
		}
		m_valE = 0;
		ALU_stat = "    --------    ";
		m_valA = E_valA;
		m_dstE = E_dstE;
		m_dstM = E_dstM;

		break;
	case 'M':
		output_M(ID_jXX[instructions[point][8] - '0'], M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		w_valE = M_valE;
		w_valM = 0;
		w_dstE = M_dstE;
		w_dstM = M_dstM;
		Mem_stat = "    --------    ";
		break;
	case 'W':
		output_W(ID_jXX[instructions[point][8] - '0'], W_valE, W_valM, W_dstE, W_dstM, W_stat);

		Stat = W_stat;
		WB_stat = "    --------    ";
		break;
	}
}

void call(int point, char stage)
{
	switch (stage)
	{
	case 'F':
		output_F(PC);

		d_valC = shift_valC(point, 9);
		predPC = d_valC;

		d_valP = PC + 9;

		d_stat = "AOK";
		d_icode = instructions[point][7];
		d_ifun = instructions[point][8] - '0';
		rA = 15;
		rB = 15;

		break;
	case 'D':
		srcB = 4;
		output_D("call  ", srcA, srcB, D_valC, D_valP, D_stat);

		e_srcA = 15; e_srcB = 15;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = D_valC;
		e_valA = D_valP;       //return address
		e_valB = *(register_seeker(srcB));
		e_dstE = 4;
		e_dstM = 15;

		break;
	case 'E':
		output_E("call  ", E_valC, E_valA, E_valB, E_srcA, E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[8]++;
		m_stat = E_stat;
		m_icode = E_icode;

		m_valE = E_valB - 8;
		ALU_stat = " valE = valB - 8 ";
		m_valA = E_valA;
		m_dstE = E_dstE;
		m_dstM = E_dstM;

		break;
	case 'M':
		output_M("call   ", M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		if (M_valE >= 0 && M_valE <= max_location)
		{
			changed_Memory[M_valE] = Memory[M_valE];
			write_to_memory(M_valE, M_valA);
            Mem_stat = " Wrote valA to memory D(valE). ";
		}
		else
		{
			error = w_stat = "ADR";
			no_pro = false;
			Mem_stat = " Can't wrote to an invalid address.";
			stop = true;
		}
		w_valE = M_valE;
		w_valM = 0;
		w_dstE = M_dstE;
		w_dstM = M_dstM;
		
		break;
	case 'W':
		output_W("call   ", W_valE, W_valM, W_dstE, W_dstM, W_stat);

		Stat = W_stat;
		*(register_seeker(W_dstE)) = W_valE;
		WB_stat = "Wrote valE to register " + ID_register[W_dstE];
		break;
	}
}

void ret(int point, char stage)
{
	switch (stage)
	{
	case 'F':
		writestr("F: predPC=----(instr = ret)\n");
		ret_signal = true;
		get_retadd = false;

		d_valP = PC + 1;
		d_valC = 0;
		d_stat = "AOK";
		d_icode = instructions[point][7];
		d_ifun = instructions[point][8] - '0';
		rA = 15;
		rB = 15;

		break;
	case 'D':
		srcA = srcB = 4;
		output_D("ret     ", srcA, srcB, D_valC, D_valP,D_stat);

		e_srcA = 15; e_srcB = 15;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = D_valC;

		e_valA = e_valB = *(register_seeker(srcA));
		e_dstE = 4;
		e_dstM = 15;

		break;
	case 'E':
		output_E("ret     ", E_valC, E_valA, E_valB, E_srcA, E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[9]++;
		m_stat = E_stat;
		m_icode = E_icode;

		m_valE = E_valB + 8;
		ALU_stat = " valE = valB + 8 ";
		m_valA = E_valA;
		m_dstE = E_dstE;
		m_dstM = E_dstM;

		break;
	case 'M':
		output_M("ret     ", M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		if (M_valA >= 0 && M_valA <= max_location)
		{
			 w_valM = read_from_memory(M_valA);
             Mem_stat = " Read valM from memory D(valA). ";
		}
		else
		{
			error = w_stat = "ADR";
			no_pro = false;
			Mem_stat = " Can't read from an invalid address.";
			stop = true;
		}
		w_valE = M_valE;
		w_dstE = M_dstE;
		w_dstM = M_dstM;
		
		break;
	case 'W':
		output_W("ret     ", W_valE, W_valM, W_dstE, W_dstM, W_stat);
		Stat = W_stat;
		*(register_seeker(W_dstE)) = W_valE;
		predPC = W_valM;
		get_retadd = true;
		WB_stat = "Wrote valE to register " + ID_register[W_dstE]+"       Wrote valM to predPC";
		break;
	}
}

void pushq(int point, char stage)
{
	switch (stage)
	{
	case 'F':
		output_F(PC);

		d_stat = "AOK";
		d_icode = instructions[point][7];
		d_ifun = instructions[point][8];
		rA = char_int(instructions[point][9]);
		rB = char_int(instructions[point][10]);
		d_valC = 0;
		d_valP = predPC = PC + 2;

		break;
	case 'D':
		output_D("pushq  ", srcA, srcB, D_valC, D_valP, D_stat);

		e_srcA = srcA; e_srcB = 15;
		srcB = 4;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = D_valC;
		e_valA = *(register_seeker(srcA));
		e_valB = *(register_seeker(srcB));
		e_dstE = 4;
		e_dstM = 15;

		break;
	case 'E':
		output_E("pushq  ", E_valC, E_valA, E_valB, E_srcA, E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[10]++;
		m_stat = E_stat;
		m_icode = E_icode;
		m_valE = E_valB - 8;
		ALU_stat = " valE = valB - 8 ";
		m_valA = E_valA;
		m_dstE = E_dstE;
		m_dstM = E_dstM;

		break;
	case 'M':
		output_M("pushq  ", M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		if (M_valE >= 0 && M_valE <= max_location)
		{
			changed_Memory[M_valE] = Memory[M_valE];
			write_to_memory(M_valE, M_valA);
		    Mem_stat = " Wrote valA to memory D(valE). ";
		}
		else
		{
			error = w_stat = "ADR";
			no_pro = false;
			Mem_stat = " Can't wrote to an invalid address.";
			stop = true;
		}
		w_valE = M_valE;
		w_valM = 0;
		w_dstE = M_dstE;
		w_dstM = M_dstM;

		break;
	case 'W':
		output_W("pushq  ", W_valE, W_valM, W_dstE, W_dstM, W_stat);

		Stat = W_stat;
		*(register_seeker(W_dstE)) = W_valE;
		WB_stat = "Wrote valE to register " + ID_register[W_dstE];
		break;
	}
}

void popq(int point, char stage)
{
	switch (stage)
	{
	case 'F':
		output_F(PC);

		d_stat = "AOK";
		d_icode = instructions[point][7];
		d_ifun = instructions[point][8];
		rA = char_int(instructions[point][9]);
		rB = char_int(instructions[point][10]);
		d_valC = 0;
		d_valP = predPC = PC + 2;

		break;
	case 'D':
		output_D("popq  ", srcA, srcB, D_valC, D_valP, D_stat);

		e_srcA = 15; e_srcB = 15;
		srcB = 4;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = D_valC;
		e_valA = e_valB = *(register_seeker(srcB));

		e_dstE = 4;
		e_dstM = srcA;

		break;
	case 'E':
		output_E("popq  ", E_valC, E_valA, E_valB, E_srcA, E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[11]++;
		m_stat = E_stat;
		m_icode = E_icode;
		m_valE = E_valB + 8;
		ALU_stat = " valE = valB + 8 ";
		m_valA = E_valA;

		m_dstE = E_dstE;
		m_dstM = E_dstM;

		break;
	case 'M':
		output_M("popq  ", M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		if (M_valA >= 0 && M_valA <= max_location)
		{
		    w_valM = read_from_memory(M_valA);
		    Mem_stat = " Read valM from memory D(valA). ";
		}
		else
		{
			error = w_stat = "ADR";
			no_pro = false;
			Mem_stat = " Can't read from an invalid address.";
			stop = true;
		}
		w_valE = M_valE;

		w_dstE = M_dstE;
		w_dstM = M_dstM;

		break;
	case 'W':
		output_W("popq  ", W_valE, W_valM, W_dstE, W_dstM, W_stat);

		Stat = W_stat;
		*(register_seeker(W_dstE)) = W_valE;
		*(register_seeker(W_dstM)) = W_valM;
		WB_stat = "Wrote valE to register " + ID_register[W_dstE]+"       Wrote valM to register "+ID_register[W_dstM];
		break;
	}
}

void iaddq(int point, char stage)        //append an extra instruction
{
	switch (stage)
	{
	case 'F':
		output_F(PC);

		if (check_r(point) == false)
		{
			instr_nums[13]++;
			error = "INS";
			stop = true;
			return;
		}
		d_stat = "AOK";
		d_icode = instructions[point][7];
		d_ifun = instructions[point][8];
		rA = char_int(instructions[point][9]);
		rB = char_int(instructions[point][10]);
		d_valC = shift_valC(point, 11);
		d_valP = predPC = PC + 10;

		break;
	case 'D':
		output_D("iaddq", srcA, srcB, D_valC, D_valP, D_stat);

		e_srcA = 15; e_srcB = srcB;
		e_stat = D_stat;
		e_icode = D_icode;
		e_ifun = D_ifun;
		e_valC = D_valC;
		e_valA = 0;
		e_valB = *(register_seeker(srcB));
		e_dstE = srcB;
		e_dstM = 15;

		break;
	case 'E':
		output_E("iaddq", E_valC, E_valA, E_valB, E_srcA, E_srcB, E_dstE, E_dstM, E_stat);
		instr_nums[12]++;
		m_stat = E_stat;
		m_icode = E_icode;
		m_valE = E_valC + E_valB;
		ALU_stat = " valE = valC + valB ";
		set_CC(m_valE,E_valC,E_valB);
		m_valA = 0;
		m_dstE = E_dstE;
		m_dstM = E_dstM;

		break;
	case 'M':
		output_M("iaddq", M_cnd, M_valE, M_valA, M_dstE, M_dstM, M_stat);

		w_stat = M_stat;
		w_icode = M_icode;
		w_valE = M_valE;
		w_valM = M_valA;
		w_dstE = M_dstE;
		w_dstM = M_dstM;
		Mem_stat = "    --------    ";
		break;
	case 'W':
		output_W("iaddq", W_valE, W_valM, W_dstE, W_dstM, W_stat);

		Stat = W_stat;

		*(register_seeker(W_dstE)) = W_valE;
		WB_stat = "Wrote valE to register " + ID_register[W_dstE];
		break;
	}
}