/*
 * Generator.h
 *
 *  Created on: Jun 20, 2019
 *      Author: trance
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "predeclares.h"
#include "tools.h"

namespace core {


class Generator {
public:
	explicit Generator(byte * buf, int buflen);
	virtual ~Generator();
	// ---------------------------------------------------------------------------
	  // Code generation
	  //
	  // - function names correspond one-to-one to ia32 instruction mnemonics
	  // - unless specified otherwise, instructions operate on 32bit operands
	  // - instructions on 8bit (byte) operands/registers have a trailing '_b'
	  // - instructions on 16bit (word) operands/registers have a trailing '_w'
	  // - naming conflicts with C++ keywords are resolved via a trailing '_'

	  // NOTE ON INTERFACE: Currently, the interface is not very consistent
	  // in the sense that some operations (e.g. mov()) can be called in more
	  // the one way to generate the same instruction: The Register argument
	  // can in some cases be replaced with an Operand(Register) argument.
	  // This should be cleaned up and made more othogonal. The questions
	  // is: should we always use Operands instead of Registers where an
	  // Operand is possible, or should we have a Register (overloaded) form
	  // instead? We must be carefull to make sure that the selected instruction
	  // is obvious from the parameters to avoid hard-to-find code generation
	  // bugs.

	  // Insert the smallest number of nop instructions
	  // possible to align the pc offset to a multiple
	  // of m. m must be a power of 2.
	  void Align(int m);

	  void ret();
	  void emit(const Immediate&);
	  void emit(const Operand & operand );
	  void emitREXPrefix(Register, const Operand&);

	  // Stack
	  void pushad();
	  void popad();

	  void pushfd();
	  void popfd();

	  void push(const Immediate& x);
	  void push(Register src);
	  void push(const Operand& src);

	  void pop(Register dst);
	  void pop(const Operand& dst);

	  // Moves
	  void mov_b(Register dst, const Operand& src);
	  void mov_b(const Operand& dst, int8_t imm8);
	  void mov_b(const Operand& dst, Register src);

	  void mov_w(Register dst, const Operand& src);
	  void mov_w(const Operand& dst, Register src);

	  void mov(Register dst, int32_t imm32);
//	  void mov(Register dst, Handle<Object> handle);
	  void mov(Register dst, const Operand& src);
	  void mov(const Operand& dst, const Immediate& x);
//	  void mov(const Operand& dst, Handle<Object> handle);
	  void mov(const Operand& dst, Register src);

#ifdef IMPPLEMENTED

	  void movsx_b(Register dst, const Operand& src);

	  void movsx_w(Register dst, const Operand& src);

	  void movzx_b(Register dst, const Operand& src);

	  void movzx_w(Register dst, const Operand& src);

	  // Conditional moves
	  void cmov(Condition cc, Register dst, int32_t imm32);
	  void cmov(Condition cc, Register dst, Handle<Object> handle);
	  void cmov(Condition cc, Register dst, const Operand& src);

	  // Arithmetics
	  void adc(Register dst, int32_t imm32);
	  void adc(Register dst, const Operand& src);

	  void add(Register dst, const Operand& src);
	  void add(const Operand& dst, const Immediate& x);

	  void and_(Register dst, int32_t imm32);
	  void and_(Register dst, const Operand& src);
	  void and_(const Operand& src, Register dst);
	  void and_(const Operand& dst, const Immediate& x);

	  void cmp(Register reg, int32_t imm32);
	  void cmp(Register reg, Handle<Object> handle);
	  void cmp(Register reg, const Operand& op);
	  void cmp(const Operand& op, const Immediate& imm);

	  void dec_b(Register dst);

	  void dec(Register dst);
	  void dec(const Operand& dst);

	  void cdq();

	  void idiv(Register src);

	  void imul(Register dst, const Operand& src);
	  void imul(Register dst, Register src, int32_t imm32);

	  void inc(Register dst);
	  void inc(const Operand& dst);

	  void lea(Register dst, const Operand& src);

	  void mul(Register src);

	  void neg(Register dst);

	  void not_(Register dst);

	  void or_(Register dst, int32_t imm32);
	  void or_(Register dst, const Operand& src);
	  void or_(const Operand& dst, Register src);
	  void or_(const Operand& dst, const Immediate& x);

	  void rcl(Register dst, uint8_t imm8);

	  void sar(Register dst, uint8_t imm8);
	  void sar(Register dst);

	  void sbb(Register dst, const Operand& src);

	  void shld(Register dst, const Operand& src);

	  void shl(Register dst, uint8_t imm8);
	  void shl(Register dst);

	  void shrd(Register dst, const Operand& src);

	  void shr(Register dst, uint8_t imm8);
	  void shr(Register dst);

	  void sub(const Operand& dst, const Immediate& x);
	  void sub(Register dst, const Operand& src);
	  void sub(const Operand& dst, Register src);

	  void test(Register reg, const Immediate& imm);
	  void test(Register reg, const Operand& op);
	  void test(const Operand& op, const Immediate& imm);

	  void xor_(Register dst, int32_t imm32);
	  void xor_(Register dst, const Operand& src);
	  void xor_(const Operand& src, Register dst);
	  void xor_(const Operand& dst, const Immediate& x);

	  // Bit operations.
	  void bts(const Operand& dst, Register src);

	  // Miscellaneous
	  void hlt();
	  void int3();
	  void nop();
	  void rdtsc();
	  void ret(int imm16);
	  void leave();

	  // Label operations & relative jumps (PPUM Appendix D)
	  //
	  // Takes a branch opcode (cc) and a label (L) and generates
	  // either a backward branch or a forward branch and links it
	  // to the label fixup chain. Usage:
	  //
	  // Label L;    // unbound label
	  // j(cc, &L);  // forward branch to unbound label
	  // bind(&L);   // bind label to the current pc
	  // j(cc, &L);  // backward branch to bound label
	  // bind(&L);   // illegal: a label may be bound only once
	  //
	  // Note: The same Label can be used for forward and backward branches
	  // but it may be bound only once.

	  void bind(Label* L);  // binds an unbound label L to the current code position

	  // Calls
	  void call(Label* L);
	  void call(byte* entry, RelocInfo::Mode rmode);
	  void call(const Operand& adr);
	  void call(Handle<Code> code, RelocInfo::Mode rmode);

	  // Jumps
	  void jmp(Label* L);  // unconditional jump to L
	  void jmp(byte* entry, RelocInfo::Mode rmode);
	  void jmp(const Operand& adr);
	  void jmp(Handle<Code> code, RelocInfo::Mode rmode);

	  // Conditional jumps
	  void j(Condition cc, Label* L, Hint hint = no_hint);
	  void j(Condition cc, byte* entry, RelocInfo::Mode rmode, Hint hint = no_hint);
	  void j(Condition cc, Handle<Code> code, Hint hint = no_hint);

	  // Floating-point operations
	  void fld(int i);

	  void fld1();
	  void fldz();

	  void fld_s(const Operand& adr);
	  void fld_d(const Operand& adr);

	  void fstp_s(const Operand& adr);
	  void fstp_d(const Operand& adr);

	  void fild_s(const Operand& adr);
	  void fild_d(const Operand& adr);

	  void fist_s(const Operand& adr);

	  void fistp_s(const Operand& adr);
	  void fistp_d(const Operand& adr);

	  void fabs();
	  void fchs();

	  void fadd(int i);
	  void fsub(int i);
	  void fmul(int i);
	  void fdiv(int i);

	  void fisub_s(const Operand& adr);

	  void faddp(int i = 1);
	  void fsubp(int i = 1);
	  void fsubrp(int i = 1);
	  void fmulp(int i = 1);
	  void fdivp(int i = 1);
	  void fprem();
	  void fprem1();

	  void fxch(int i = 1);
	  void fincstp();
	  void ffree(int i = 0);

	  void ftst();
	  void fucomp(int i);
	  void fucompp();
	  void fcompp();
	  void fnstsw_ax();
	  void fwait();

	  void frndint();

	  void sahf();

	  void cpuid();

	  // SSE2 instructions
	  void cvttss2si(Register dst, const Operand& src);
	  void cvttsd2si(Register dst, const Operand& src);

//	  void cvtsi2sd(XMMRegister dst, const Operand& src);
//
//	  void addsd(XMMRegister dst, XMMRegister src);
//	  void subsd(XMMRegister dst, XMMRegister src);
//	  void mulsd(XMMRegister dst, XMMRegister src);
//	  void divsd(XMMRegister dst, XMMRegister src);
//
//	  // Use either movsd or movlpd.
//	  void movdbl(XMMRegister dst, const Operand& src);
//	  void movdbl(const Operand& dst, XMMRegister src);

#endif

private:
	  byte * pc;
	  byte * last_pc;
	  int buf_len;
};





} /* namespace core */

#endif /* GENERATOR_H_ */
