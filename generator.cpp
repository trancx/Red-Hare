/*
 * Generator.cpp
 *
 *  Created on: Jun 20, 2019
 *      Author: trance
 */


#include "core.h"
#include <stdio.h>

extern FILE * file;

namespace core {

class Register;

#ifdef DEBUG

#define EMIT(x) \
		do { \
			byte temp = (byte)(x);\
			*pc++ = (x); \
			printf("%#hhx ", temp); \
		} while(0)


#define IMM_EMIT(x)	\
		do {			\
			char temp = (char)x; \
			printf("%#hhx ", x); \
		} while(0)


#define DELIMIT	"\t|| "

#define ASM(format,arg...) \
		do { \
			fprintf(file, format, ##arg); \
			printf(DELIMIT format,##arg); \
		} while(0)

#else

#define EMIT(x) \
	*pc++ = (x)

#define IMM_EMIT(x)
#define ASM(format,arg...) \
		do {} while(0)


#define DELIMIT


#endif
Generator::Generator( byte* pc, int len): pc(pc), buf_len(len) {
	// TODO Auto-generated constructor stub
	last_pc = NULL;

}


Generator::~Generator() {
	// TODO Auto-generated destructor stub
}



void Generator::emit(const Immediate& x) {
	*reinterpret_cast<uint32_t*>(pc) = x.x_;
			 pc += sizeof(uint32_t);
#ifdef DEBUG
		printf("%#08x ", x.x_);
#endif
}


/**
 * 	SIB or NO SIB, operand knows it best~
 * 	So , Do we need  Operand.isSIB() ?
 *
 * 	For Opcode cases, means B bits is used to
 * 	    expand the Opcode, we cope them in each
 * 	    instructions, That means No Operand!!!
 *	 	Ofcourse, W bit is ZERO
 *
 */
void Generator::emitREXPrefix(Register reg, const Operand& operand ) {
	int REX = 0x48;
	byte b;
	REX |= (reg.getRexPrefix() << 2); // W R X B
	REX |=  operand.getRexPrefix();	// B or  XB
	b = (byte)(REX & 0xff);
	EMIT(b);
}


// FIXME: space left check~

//  0x68  Word for 16-bit operand-size
//      or doubleword for 32 or 64-bit operand-size.
//  0x6a 8 imm
void Generator::push(const Immediate &x) {
	if( x.is_int8() ) {
		EMIT(0x6a);
		EMIT(x.x_);
	} else {
		EMIT(0x68);
		emit(x);
	}
	ASM("pushq $%d\n", x.x_);
}

void Generator::push(Register reg) {
	int code = reg.getReg();
	if( !reg.needRexPrefix() ) {
		EMIT( 0x50 | code );
	} else {
		code -= 8;
		EMIT( 0x40 | reg.getRexPrefix() );
		EMIT( 0x50 | code );
	}
	ASM("pushq %%%s\n", reg.getName());
}

void Generator::push(const Operand &src) {

}

// REX 0100 WRXB, we need B
// TELLME: why w=0?
// TELLYOU: w -> tell us about the operand~~~
void Generator::pop(Register dst) {
	int code = dst.getReg();
	if( !dst.needRexPrefix() ) {
		EMIT( 0x58 | code );
	} else {
		// r8~r15
		code &= 7;
		EMIT( 0x40 | dst.getRexPrefix() ); // fill B
		EMIT( 0x58 | code );
	}
	ASM("popq %%%s\n", dst.getName());
}

void Generator::ret() {
	EMIT(0xc3);
	ASM("retq\n");
}

void Generator::pop(const Operand &dst) {



}

// 1. 0xc7  mov Ev, Iz   mod = 11, R/m = dst  REX extended
// 2. 0xb8 + reg, + IMM32 directly MOV immediate word or
//	   double into word, double, or quad register, REX extended
void Generator::mov(Register dst, int32_t imm32) {
	int code = dst.getReg();
	if( !dst.needRexPrefix( ) ) {
		EMIT( 0xb8 | code );
	} else {
		// r8~r15
		// why w=0? 0x40 not 0x48, COZ no Operand
		code &= 7;
		EMIT( 0x40 | dst.getRexPrefix() ); // fill B
		EMIT( 0xb8 | code );
	}
	// FIXME : dont use local var, use emit(int32_t)
	emit(Immediate(imm32));
	ASM("movq $%#08x, %%%s\n", imm32, dst.getName());
}

// mov Ev, Iz  0xc7  mod , R/m = dst
void Generator::mov(const Operand &dst, const Immediate &x) {

}



// mov Gv, Ev
void Generator::mov(Register dst, const Operand &src) {
	int i  = 0, len;
	emitREXPrefix(dst, src);
	EMIT(0x8B);
	src.buffer[0] |= (byte)((dst.getReg() & 0x7) << 3);  // most  significant in REX prefix
	len = src.length;
	while( i < len ) {
		EMIT(src.buffer[i++]);
	}
#ifdef DEBUG
		ASM("mov %%%s, %%%s\n", src.getName(), dst.getName());
#endif
}

// mov Ev, Gv
void Generator::mov(const Operand &dst, Register src) {
	int i  = 0, len;
	emitREXPrefix(src, dst);
	EMIT(0x89);
	dst.buffer[0] |= (byte)((src.getReg() & 0x7) << 3);  // most  significant in REX prefix
	len = dst.length;
	while( i < len ) {
			EMIT(dst.buffer[i++]);
	}
#ifdef DEBUG
		ASM("mov %%%s, %%%s\n", src.getName(), dst.getName());
#endif
}



} /* namespace core */
