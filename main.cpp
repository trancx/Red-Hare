#include <stdio.h>
#include <linux/types.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//char cmd[]  = { 0x55, 0x48, 0x89, 0xe5, 0x89, 0x7d, 0xfc, 0x89, 0x75, 0xf8,
//		0x8b, 0x45, 0xf8, 0x8b, 0x55, 0xfc, 0x1, 0xd0, 0x5d, 0xc3, 0x55,
//		0x48, 0x89, 0xe5, 0x48, 0x83, 0xec, 0x40, 0xc7, 0x45, 0xec,
//		0x7, 0,};

#define DEFINE_REG_OPERAND(reg) \
	core::Operand reg = core::Operand(core::reg)


typedef int (*func)(void);
#include "core.h"

FILE * file = NULL;

int main() {

	int ret;
	func f;
	int prot = PROT_READ | PROT_WRITE | PROT_EXEC;
	size_t size = 0x10000;

	void * address = mmap(NULL, size, prot,
			MAP_PRIVATE | MAP_ANONYMOUS ,
            -1, 0 );

//	void * buff = address;
//	memcpy(buff, (void *)cmd, 33 );
	file = fopen("template.asm", "w+");
	if( !file ) {
		perror("error when open file");
		exit(-1);
	}
	core::Generator * g;
	g = new core::Generator(reinterpret_cast<byte*>(address), size);

	DEFINE_REG_OPERAND(rax);
	DEFINE_REG_OPERAND(rbx);
	DEFINE_REG_OPERAND(rcx);
	DEFINE_REG_OPERAND(rdx);
	DEFINE_REG_OPERAND(rsi);
	DEFINE_REG_OPERAND(rdi);
	DEFINE_REG_OPERAND(rbp);
	DEFINE_REG_OPERAND(rsp);
	DEFINE_REG_OPERAND(r8);
	DEFINE_REG_OPERAND(r11);
//	byte buf[1024];
//	memset(buf, 0, 1024);
//	g = new core::Generator(buf, size);
	puts("CODE_BEGIN");
	puts("------------------------------------------");
	{
		g->push(core::rbp);
		g->mov(core::rbx, 10);
//		g->mov(rax, core::Immediate(10));
		g->push(rbx);
		g->pop(rax);
//		g->mov(rax, core::r11);
//		g->push(core::r11);
//		g->pop(core::rax);
		g->pop(core::rbp);
		g->ret();
	}
	puts("------------------------------------------");
	puts("CODE_END");


	fflush(file);
	fclose(file);

	system("sh ./src/run.sh");

	f = (func)address;
	ret = f();
	printf("ret = %d\n", ret);
//	char * p = (char * )buf;
//	for(ret=0; ret<50; ret++ ) {
//		printf("%#hhx ", *p++);
//		if( (ret+1) % 5 == 0)
//			puts("");
//	}

	delete g;
	munmap(address, size);

//	char * array = (char *)malloc(33);
//	memcpy(array, buff, 33 );
//
//	char * tmp = array;
//
//	for(ret=0; ret<33; ret++ ) {
//		printf("%#hhx, ", *tmp++);
//	}
//
//	memcpy(buff, array, 33 );


}
