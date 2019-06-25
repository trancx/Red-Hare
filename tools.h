/*
 * tools.h
 *
 *  Created on: Jun 22, 2019
 *      Author: trance
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdint.h>
#include <stdio.h>

#ifndef NULL
#define NULL ((void *)0)
#endif


typedef uint8_t byte;
typedef byte *  Address;



// FIXME: checker~
#define CHECK(condition) CheckHelper(__FILE__, __LINE__, #condition, condition)


#ifdef DEBUG
#define ASSERT(condition)    CHECK(condition)
#define ASSERT_EQ(v1, v2)    CHECK_EQ(v1, v2)
#define ASSERT_NE(v1, v2)   CHECK_NE(v1, v2)
#define STATIC_ASSERT(test)  STATIC_CHECK(test)
#define SLOW_ASSERT(condition) if (FLAG_enable_slow_asserts) CHECK(condition)
#else
#define ASSERT(condition)      ((void) 0)
#define ASSERT_EQ(v1, v2)      ((void) 0)
#define ASSERT_NE(v1, v2)     ((void) 0)
#define STATIC_ASSERT(test)    ((void) 0)
#define SLOW_ASSERT(condition) ((void) 0)
#endif

// define used for helping GCC to make better inlining.
//#ifdef __GNUC__
//#if (__GNUC__ >= 4)
//#define INLINE(header) inline header  __attribute__((always_inline))
//#else
//#define INLINE(header) inline __attribute__((always_inline)) header
//#endif
//#else
//#define INLINE(header) inline header
//#endif

#define INLINE(header) inline header  __attribute__((always_inline))
// -----------------------------------------------------------------------------
// Macros

// Testers for test.

#define HAS_SMI_TAG(value) \
  ((reinterpret_cast<int>(value) & kSmiTagMask) == kSmiTag)

#define HAS_FAILURE_TAG(value) \
  ((reinterpret_cast<int>(value) & kFailureTagMask) == kFailureTag)

#define HAS_HEAP_OBJECT_TAG(value) \
  ((reinterpret_cast<int>(value) & kHeapObjectTagMask) == kHeapObjectTag)

// OBJECT_SIZE_ALIGN returns the value aligned HeapObject size
#define OBJECT_SIZE_ALIGN(value)                                \
  ((value + kObjectAlignmentMask) & ~kObjectAlignmentMask)

// The expression OFFSET_OF(type, field) computes the byte-offset
// of the specified field relative to the containing type. This
// corresponds to 'offsetof' (in stddef.h), except that it doesn't
// use 0 or NULL, which causes a problem with the compiler warnings
// we have enabled (which is also why 'offsetof' doesn't seem to work).
// Here we simply use the non-zero value 4, which seems to work.
#define OFFSET_OF(type, field)                                          \
  (reinterpret_cast<intptr_t>(&(reinterpret_cast<type*>(4)->field)) - 4)


// The expression ARRAY_SIZE(a) is a compile-time constant of type
// size_t which represents the number of elements of the given
// array. You should only use ARRAY_SIZE on statically allocated
// arrays.
#define ARRAY_SIZE(a)                                   \
  ((sizeof(a) / sizeof(*(a))) /                         \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))


// The USE(x) template is used to silence C++ compiler warnings
// issued for (yet) unused variables (typically parameters).
template <typename T>
static inline void USE(T) { }


// FUNCTION_ADDR(f) gets the address of a C function f.
#define FUNCTION_ADDR(f)                                        \
  (reinterpret_cast<v8::internal::Address>(reinterpret_cast<intptr_t>(f)))


// FUNCTION_CAST<F>(addr) casts an address into a function
// of type F. Used to invoke generated code from within C.
template <typename F>
F FUNCTION_CAST(Address addr) {
  return reinterpret_cast<F>(reinterpret_cast<intptr_t>(addr));
}


// A macro to disallow the evil copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName)      \
  TypeName(const TypeName&);                    \
  void operator=(const TypeName&)


// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName();                                    \
  DISALLOW_COPY_AND_ASSIGN(TypeName)

#include <string.h>

#endif /* TOOLS_H_ */
