#include"../src/Functions.cpp"
using namespace DerKern;
int main(){
	Type*t=&Type::u8;
	Instructionary func0(0),w(0);
	Function func((Instructionary*)0,&Type::u8,&t,1);func.p=&func0;
	printf(".\n");

	Location args[256];func.allCalc(args);//No need to make it an instruction, this updates "args" immediately.
	printf(".\n");
	func0.inliny=1;func0.add<Instructions::Set>(args[255],args[0],1);func0.compile();
	//func0.inliny=0;//Uncomment this if you want it to "call" and not "inline"

	Instructions::NopO* arg=w.allo<Instructions::Set8>(Location(),0xcd);
	w.add<Instructions::CallQuick>(&func,&arg,1);
	w.inliny=1;w.compile();

	uint32_t _=((uint8_t(*)())w._compiled)()*0x1010101;
	printf("%lx\t\tExpected a bunch of \"cd\"\n",_);
}