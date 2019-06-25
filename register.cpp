/*
 * register.cpp
 *
 *  Created on: Jun 21, 2019
 *      Author: trance
 */

#include "core.h"
#include "tools.h"

namespace core {

Register::Register(int reg_) :  name(NULL), reg(reg_) {
	// TODO Auto-generated constructor stub
}

Register::~Register() {
	// TODO Auto-generated destructor stub
}

Register::Register(int reg_, const char * name ):name(name), reg(reg_)  {

}
	// predefined registers
	Register rax = Register(0, "rax");
	Register rcx = Register(1, "rcx");
	Register rdx = Register(2, "rdx");
	Register rbx = Register(3, "rbx");
	Register rsp = Register(4, "rsp");
	Register rbp = Register(5, "rbp");
	Register rsi = Register(6, "rsi");
	Register rdi = Register(7, "rdi");
	Register r8 = Register(8, "r8");
	Register r9 = Register(9, "r9");
	Register r10 = Register(10, "r10");
	Register r11 = Register(11, "r11");
	Register r12 = Register(12, "r12");
	Register r13 = Register(13, "r13");
	Register r14 = Register(14, "r14");
	Register r15 = Register(15, "r15");






} /* namespace core */


