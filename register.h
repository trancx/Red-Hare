/*
 * register.h
 *
 *  Created on: Jun 21, 2019
 *      Author: trance
 */

#ifndef REGISTER_H_
#define REGISTER_H_

#include "predeclares.h"
#include "tools.h"

namespace core {

//enum RegisterTypes {
//	B_reg = 1,
//	W_reg = 2,
//	L_reg = 4,
//	Q_reg = 8
//};


/**
 * 		r8~r15  we need Rex extension~ Or simply fill 0x48 prefix
 */
class Register {
  public:
	explicit Register(int reg);

	explicit Register(int reg, const char * name );

	virtual ~Register();
	bool needRexPrefix() const {
		// FIXME： 其实不只是小于7要，其实像 pop push 默认都是64位的则没有前缀
		// 而 mov 即使是小于7 仍然要前缀 0x48 | prefix，看手册
		if( reg > 7 )
			return true;
		return false;
	}
	int getRexPrefix() const {
		return reg >> 3;
	}
	inline int getReg() const {
		return this->reg;
	}

	void setName(const char * name ) {
		this->name = name;
	}

	const char * getName() const {
		return name;
	}
  private:
	const char * name;
	int reg;
	friend class Generator;
};


class RexPrefix {

};


enum ScaleFactor {
  times_1 = 0,
  times_2 = 1,
  times_4 = 2,
  times_8 = 3,
  times_16 = 4
};


enum AddressForm {
	BASE = 0,
	BASE_disp8 = 1,
	BASE_disp32 = 2,
	DIRECT	= 3
};

/**
 *   REX + Mod + R/M + SIB + DIS
 */
class Operand {
  public:
	 // reg
	  INLINE( explicit Operand(Register reg) );
//	  inline explicit Operand(Register reg)  __attribute__((always_inline));
//	  INLINE(explicit Operand(Register reg));
	  // [disp/r]
	  INLINE( explicit Operand(int32_t disp) );
	  // disp only must always be relocated

	  // [base + disp/r]
	  explicit Operand(Register base, int32_t disp);

	  // [base + index*scale + disp/r]
	  explicit Operand(Register base,
	                   Register index,
	                   ScaleFactor scale,
	                   int32_t disp);
	             

	  // [index*scale + disp/r]
	  explicit Operand(Register index,
	                   ScaleFactor scale,
	                   int32_t disp
	                   );
	  inline int getRexPrefix() const {
		  return rex_prefix;
	  }

	  inline bool isSIB() const {
		  return is_SIB;
	  }
  private:
#define Operand_LENGTH 8
	  mutable byte buffer [Operand_LENGTH];
	  int length;
	  int rex_prefix;	/* when SIB  rex may == 3, means  11 */
	  bool is_SIB; /* easy deal with REX prefix */
	  inline void set_mod(AddressForm b);
	  inline void set_rm(byte b);
	  friend class Generator;
#ifdef DEBUG
	  char name[32];
  public:
	  const char * getName() const {
		  return name;
	  }
#endif
};

class Immediate  {
  public:
	  inline explicit Immediate(int x) {
		  x_ = x;
	  }
//	  inline explicit Immediate(const char* s) {
//		  x_ = reinterpret_cast<int32_t>(s);
//	  }
	  bool is_zero() const { return x_ == 0;}

	  // 8 or 32
	  bool is_int8() const {
	    return -128 <= x_ && x_ < 128;
	  }

	 private:
	  int x_;
	  friend class Generator;
};



// most significant 2 bits
inline void Operand::set_mod(AddressForm b) {
	buffer[0] &= 0x3f;
	buffer[0] |= (byte)b << 6;
}


// least 3 bits
inline void Operand::set_rm(byte b) {
	buffer[0] &= 0xf8;
	buffer[0] |= (b&7);
}

/**
 * 	if operand use 64bits register
 * 	then Rex = 0x48 | specific bits
 */
inline Operand::Operand(Register reg) : length(0), is_SIB(false) {
	byte rm;
	memset(buffer, 0, Operand_LENGTH);
#ifdef DEBUG
	memset(name, 0, 32);
	strncpy(name, reg.getName(), strlen(reg.getName())+1 );
#endif
	set_mod(DIRECT);
	rm = reg.getReg();
	set_rm(rm);
	length = 1;
	if( reg.needRexPrefix( ) )
		rex_prefix = 1;
	else
		rex_prefix = 0;
}

inline Operand::Operand(int32_t disp) {
	length = 0;
	rex_prefix = 0;
	is_SIB = false;


}


} /* namespace core */

#endif /* REGISTER_H_ */
