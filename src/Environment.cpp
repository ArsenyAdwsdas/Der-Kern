#include"../include/DerKern/Environment.h"
namespace DerKern{
	Environment::Environment(){
		stackSize=1024*512;stack=(uint8_t*)malloc(stackSize);
		regs[0]=regs[1]=regs[2]=regs[3]=regs[6]=regs[7]=0;
		regs[4]=regs[5]=(decltype(*regs))stack+stackSize;
		if(sizeof(int*)==8)regs[8]=regs[9]=regs[10]=regs[11]=regs[12]=regs[13]=regs[14]=regs[15]=0;
		ip=0;
	}
	Environment::~Environment(){
		free(stack);
	}
	int32_t&Environment::pointTo(string name,Type*t,void*loc){

	}
}