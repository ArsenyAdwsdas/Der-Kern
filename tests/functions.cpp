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
	Type*_[2]={&Type::u64,&Type::u64};
	OverloadedFunction*overy;
	scp.set("+",{&Type::overloadery,overy=new OverloadedFunction()});
	auto op=overy->typery.set(new Function(&Type::u64,_,2,0));
	Function*z;{z=GetFunction((string)"+",_,2,&scp);}if(!z)printf("GetFunction FAIL\n");



	printf("?..\n");if(1)return 0;
	uint64_t a[]={1,2006};
	pair<Type*,Location>ret={&Type::u64,a};
	const pair<Type*,Location>args[2]={{&Type::u64,&a[0]},{&Type::u64,&a[1]}};
	RegisterState regs;
	z->eval(&regs,ret,args,2);
	return 0;
}