#include"../src/extra/type_stuff.cpp"
#include"../include/DerKern/extra/simpler.h"
#include"../include/DerKern/extra/type_interface.h"
using namespace DerKern;
struct type{
	uint32_t v;
};
int main(){
	Type*t=new Type("type",sizeof(type));auto me=ExposeMemba(t,&type::v,"v");//make a Type, get a pointer Type to it and expose "v"
	Type*pt=t->pointer();//an argument "list" for Function
	Function func((Instructionary*)0,&Type::VOID,&pt,1);//a Function's shell
	Instructionary fIns(0);func.p=&fIns;//can be replaced with something like FunctionBuilder

	Location args[256];fIns.add<Instructions::ArgCalc>(&func,args);//this'll tell where arguments are

	fIns[(uint8_t)2](sizeof(int*))=args+0;//takes the pointer argument and sents it to register 2 since it's needed in a register, reg 1 gets taken away as an argument on WIN64
	//Sure, I could optimize it since on x64 the arguments are sent straight to registers... Eh, too bothersome to think of how to make it readable.
	fIns[(uint8_t)0]=uint32_t(0xcdcdcdcc);//sets register 0 for MovM
	fIns.add<Instructions::MovM>(Location{(uint8_t)2,0},me,(uint8_t)0);//this'll set "v" by using results of the previous 2 lines

	fIns.inliny=1;fIns.compile();//compile it all
	type stru;stru.v=0;
	uint32_t _=((uint32_t(*)(type*))fIns._compiled)(&stru);//calls a newborn function
	printf("%lx\t\tExpected \"cdcdcdcc\"\n",stru.v);
}