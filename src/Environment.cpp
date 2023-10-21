#include"../include/DerKern/Environment.h"
#include"Values.cpp"
namespace DerKern{
	Environment::Environment(size_t ss){
		stackSize=ss;stack=(uint8_t*)malloc(ss);
		regs[0]=regs[1]=regs[2]=regs[3]=regs[5]=regs[6]=regs[7]=0;
		regs[4]=(RegisterState::regT)stack+ss;
		if(sizeof(int*)==8)regs[8]=regs[9]=regs[10]=regs[11]=regs[12]=regs[13]=regs[14]=regs[15]=0;
		ip=0;
	}
	Environment::~Environment(){
		free(stack);
	}
	int32_t&Environment::pointTo(string name,Type*t,void*loc){

	}
}