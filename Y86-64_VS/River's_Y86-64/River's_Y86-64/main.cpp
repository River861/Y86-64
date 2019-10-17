#include<iostream>
#include<string>
#include "read.h"
#include "initialize.h"
#include "choose_road.h"
#include "five_stages.h"
#include "twelve_instr.h" 
#include "write.h"
#include "outline.h"
#include "pipe_register.h"
#include "memory_operation.h"

using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void clear_initial()
{
	print_cache = false;
	No_cache = false;
	for (int i = 0; i < 4; i++)  { cache[i].avail = 0;for (int j = 0; j < 4; j++) cache[i].copy[j] = make_pair(0, 0); }
	instructions.clear();
	Memory.clear();
	changed_Memory.clear();
	location_point.clear();
	for (int i = 0; i < 14; i++) instr_nums[i] = 0;
	isorder = isbreak = false;
	rax = 0, rcx = 0, rdx = 0, rbx = 0, rsp = 0, rbp = 0, rsi = 0, rdi = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r13 = 0, r14 = 0, no_data = 0;
	PC = 0;
	saved_CC[0] = saved_CC[1] = saved_CC[2] = 0;
	ZF = 0, SF = 0, OF = 0;
	Stat = "AOK";
	ALU_stat = WB_stat = Mem_stat = "    --------    ";
	stop = false;
	no_pro = true;
	 predPC=0;
	 
	 d_stat = "AOK"; d_icode = '1';  d_ifun = 0;  srcA = rA = 15; srcB = rB = 15; d_valC = 0; d_valP = 0;
	 D_stat = "AOK"; 
	 e_stat = "AOK"; e_icode = '1';  e_ifun = 0;  e_valC = 0; e_valA = 0; e_valB = 0; e_dstE = 15, e_dstM = 15;  e_srcA = 15, e_srcB = 15;
	 E_stat = "AOK";
	 m_stat = "AOK"; m_icode='1';    m_cnd = 1;   m_valE = 0, m_valA = 0; m_dstE = 15, m_dstM = 15;
	 M_stat = "AOK"; 
	 w_stat = "AOK"; w_icode='1';    w_valE = 0, w_valM = 0;  w_dstE = 15, w_dstM = 15;
	 W_stat = "AOK";

	 halt_stop=call_signal=ret_signal=get_retadd=false;
	return;
}

int main(int argc, char** argv) {
	string order,cache_or_not;
	int m;
	cout << "Memory Size is:";
	cin >> m; cout << m << endl;
	max_location = m * 1024 * 1024;
	cout << "Speed level is:"<<endl;
	cin >> speed; cout << speed << endl;
	cin >> cache_or_not;
	if (cache_or_not == "no_cache.") No_cache = true;
	else No_cache = false;
//	cout << "Print 'test_begin.' to start all the Y86-64 tests one by one," << endl;
//	cout << "or print 'test_one.' to test the one test you want, or print 'order.'to send an instant code." << endl;
	cin >> order;
	if (order == "test_begin.")
	{
	    read_filename(order);
	    for (int i = 0; i < testlist.size(); i++)
	    {
		    filename = testlist[i];
		    clear_initial();
	        read();
	        pipeline(filename);
	    }
	    cout << endl;
	    cout << "All finished. Now you can check the results in the 'test result' file~~" << endl;
	    return 0;
	}
	else if (order == "test_one.")
	{
		cout << endl;
		cout << "All right. Tell me the name of the test you want:" << endl;
		cin >> filename;
		read();
		pipeline(filename);
		cout << endl;
		cout << "Finished. Now you can check the result in the 'test result' file~~" << endl;
		return 0;
	}
	else if (order == "order.")
	{
		isorder = true;
		string instant_code;
        filename = "instant_code";
		while (getline(cin,instant_code))
		{
			if (instant_code == "EOF")
			{
				pipeline(filename);
				break;
			}
			else if (instant_code == "break")
			{

				pipeline(filename);
				instructions.clear();
				isbreak = true;
				continue;
			}
			else
			{
				if (isempty(instant_code)) continue;
				while (instant_code.size() < 27) instant_code.push_back(' ');
				instructions.push_back(instant_code);
				long long locate = stoll(instant_code.substr(0, 5), NULL, 16);
				location_point[locate] = instructions.size() - 1;
				Memory[locate] = shift_valC(instructions.size() - 1, 7);
			}
		}

		cout << endl;
		cout << "Finished." << endl;
		return 0;
	}
	else
	{
		cout << "It's a wrong order." << endl;
		exit(0);
	}
}