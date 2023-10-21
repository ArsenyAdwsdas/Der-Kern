#include"../src/Functions.cpp"
#include"../src/Values.cpp"
using namespace DerKern;

bool TypeCanBe(const Type*expected,const Type*got){
	#warning I am NOT morally ready for all this "const" stuff... It's nightmare fuel. Too much checking for proper support. This'll do it for now.
	if(expected->typeID==Type::IDs::Const){return TypeCanBe(((const ConstType*)expected)->orig,got);}
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
		if(z->val.type==7)f=(OverloadedFunction*)z->val.ptr;
		if(f){auto _=f->tryGet(args);if(_)return _;}
	}
	return 0;
}
int main(){
	Function*z;{Type*_[2]={&Type::u64,&Type::u64};z=GetFunction((string)"+",_,0);}
	uint64_t a[]={1,2006};
	pair<Type*,Location>ret={&Type::u64,a};
	const pair<Type*,Location>args[2]={{&Type::u64,&a[0]},{&Type::u64,&a[1]}};
	RegisterState regs;
	z->eval(&regs,ret,args,2);
	return 0;
}