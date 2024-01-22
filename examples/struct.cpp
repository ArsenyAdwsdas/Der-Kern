#include"../src/extra/type_stuff.cpp"
#include"../include/DerKern/extra/simpler.h"
using namespace DerKern;

//This example quickly makes a Type with a member
//Makes an object of that Type on stack
//"Set"s the object's member
//"Get"s and returns the value of the member
int main(){
	Type t=Type("type",8);//makes a Type, usually only a pointer to it should be kept manually, yet this is good enough since nothing outlives the "int main"
	Type::Memba*me=&t.membas.set("test",{0,&Type::u32});//marks a member(some memory for each instance of the Type) and stores a pointer since it's needed later
	Instructionary ins(0);
	Location obj;//this keeps the object's place, but ONLY when "ins.compile()" happens
	//Note: "&obj" should always get resolved to the current value of "obj" at the "compile" moment of each Instruction
	//Because of that you basically should only use "&obj"
	//WARNING: for the same reason taking "obj" out of scope would

	//Kinda looks like asm, I guess... As long as you remove all my explaining...
	ins.add<Instructions::StackAllo>(t.size,&obj);//ONLY AT "ins.compile()": Makes some space for the object and updates "obj". DON'T YOU DARE USE "obj" RIGHT AFTER THIS AND EXPECT IT TO BE UPDATED.
	ins[&obj]+=&t;//not neccessary here since it only calls the default constructor if there is one
	ins[(uint8_t)0](4)=0xA123456A;//"Set" register 0
	ins[&obj][me]=(uint8_t)0;//"Set" object's member to register 0's value
	ins[(uint8_t)0](4)=0;//Zeroes register 0; NOT needed.
	ins[(uint8_t)0]={&obj,me};//"Get" object's member into register 0 since that's the return value...
	ins[&obj]-=&t;//not neccessary here since it only calls the destructor if there is one
	
	ins.inliny=1;//takes care of cleaning up the stack and returning
	ins.compile();//turns all the mess above into a function
	uint32_t _=((uint32_t(*)())ins._compiled)();//this spell-like thing calls the newborn function
	printf("%lX\t\tExpected \"A123456A\"\n",_);

}