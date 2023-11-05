#include"../../src/Compile.cpp"

using namespace DerKern;
int main(){
	Environment env=Environment(32);
	RegisterState inpRegisters=env.regs;
	Instructionary ins(&env);ins.inliny=1;

	ins+=new Instructions::Set16((uint8_t)1,0x1111);
	ins+=new Instructions::Set32((uint8_t)2,0x11111111);
	if(sizeof(int*)==8)ins+=new Instructions::Set64((uint8_t)13,0x1313131313131313);
	ins+=&Instructions::ret;
	ins+=new Instructions::Set8((uint8_t)0,0xcd);
	
	ins.ln();
	ins.compile();
	if(!ins._compiled){printf("COMPILATION FAIL\n");return-1;}
	ins.eval(env.regs.raw);
	printf("State:\n");for(uint8_t i=0;i<(sizeof(int*)<<1);i++)printf("  r%.2d: %p -> %p\n",(int)i,inpRegisters[i],env.regs[i]);
}