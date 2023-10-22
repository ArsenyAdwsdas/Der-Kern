#include"../src/Functions.cpp"
#include"../src/Values.cpp"
#include"../src/Instruction.cpp"
using namespace DerKern;

bool DerKern::Parse1(uint64_t*i,string S,ParseResult*r){throw std::exception();}
bool TypeCanBe(const Type*expected,const Type*got){
	#warning I am NOT morally ready for all this "const" stuff... It's nightmare fuel. Too much checking for proper support. This'll do it for now.
	if(expected->typeID==Type::IDs::Const){return expected==got||TypeCanBe(((const ConstType*)expected)->orig,got);}
	return expected==got;
}


Function*GetFunction(string name,Type**argv,uint8_t argc,const Scope*scop=0){
	FuncArgs args={argv,argc};
	for(uint8_t i=0;i<argc;i++){
		auto z=argv[i]->Static(name);
		if(z&&TypeCanBe(Type::overloadery.Const(),z->type)){
			OverloadedFunction*f=0;
			if(z->val.type==1)f=(OverloadedFunction*)z->val.imov;
			if(z->val.type==7)f=(OverloadedFunction*)z->val.ptr;
			if(f){auto _=f->tryGet(args);if(_)return _;}
		}
	}
	if(!scop)return 0;
	auto z=scop->get(name);
	if(z&&TypeCanBe(Type::overloadery.Const(),z->type)){
		OverloadedFunction*f=0;
		if(z->val.type==1)f=(OverloadedFunction*)z->val.imov;
		if(z->val.type==4)f=(OverloadedFunction*)z->val.ptr;
		if(f){auto _=f->tryGet(args);if(_)return _;}
	}
	return 0;
}
int main(){
	Scope scp;
	Type*_[2]={&Type::u16,&Type::u16};
	OverloadedFunction*overy;
	scp.set("+",{&Type::overloadery,overy=new OverloadedFunction()});
	auto op=overy->typery.set(new Function(&Type::u16,_,2,0));
	op->inl=new Instructions::Add(&op->retLoc,&op->argvLoc[0],&op->argvLoc[1],2);


	Environment env=Environment(0);
	//printf("?..\n");if(1)return 0;
	uint16_t a[]={1,2006};
	Location ret=a;
	const Location args[2]={(uint8_t)0,(uint8_t)1};env.regs[0]=1;env.regs[1]=2006;//{&a[0],&a[1]};
	Function*z;{z=GetFunction((string)"+",_,2,&scp);}if(!z)printf("GetFunction FAIL\n");

	CompileState com(&env,0);BBuf buf;com.b=&buf;

	Instructions::transitTo.compile(com);
	if(!z->compile(com,args[0],args,2)){printf("Function compilation failed!\n");return-1;}
	Instructions::transitFrom.compile(com);

	printf("Instructions: ");for(uint32_t i=0;i<buf.count;i++)printf("%.2x",buf[i]);printf("\n");

	RegisterState inpRegisters=env.regs;
	com.toExec()(env.regs.raw);
	printf("State:\n");
	for(uint8_t i=0;i<(sizeof(int*)<<1);i++)printf("  r%.2d: %p -> %p\n",(int)i,inpRegisters[i],env.regs[i]);
	return 0;
}