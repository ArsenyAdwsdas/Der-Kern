#pragma once
#include"../include/DerKern/Functions.h"
#include"Type.cpp"
namespace DerKern{
	Type Type::func_container=Type("<FUNCTION>",sizeof(Function));Type Type::overloadery=Type("overloadery",sizeof(OverloadedFunction));
	SortList<FuncType*,uint16_t,16,cmp>FuncType::_funcs;
	FuncType*FuncType::Get(Type*ret,Type**argv,uint8_t argc){
		string s=FuncType::NAME(ret,argv,argc);
		FuncBase _={ret,argv,argc};
		uint16_t b=_funcs.getI<FuncBase,cmp>(_);
		if(BinSearch<FuncType*,FuncBase,cmp,uint16_t>(0,b,_funcs.count,_funcs.raw,_))return _funcs.raw[b];
		auto z=_funcs._insert(new FuncType(s,ret,argv,argc),b);z->parent=&func_container;
		return z;
	}
	inline int cmp(const FuncArgs&a,const FuncArgs&b){
		uint8_t c=a.argc>b.argc?b.argc:a.argc;
		for(uint8_t i=0;i<c;i++){int _=cmp(a.argv[i],b.argv[i]);if(_!=0)return _;}
		return cmp(a.argc,b.argc);
	}
	inline int cmp(const FuncBase&a,const FuncBase&b){int _=cmp(a.rets,b.rets);if(_!=0)return _;return cmp((const FuncArgs&)a,(const FuncArgs&)b);}
	int cmp(FuncType*const&a,FuncType*const&b){return cmp((const FuncBase&)*a,(const FuncBase&)*b);}
	int cmp(FuncType*const&a,FuncBase const&b){return cmp((const FuncBase&)*a,b);}

	int cmp(Function*const&a,Function*const&b){return cmp((const FuncBase&)*a,(const FuncBase&)*b);}
	int cmp(Function*const&a,FuncBase const&b){return cmp((const FuncBase&)*a,b);}
	int cmp(Function*const&a,FuncArgs const&b){return cmp((const FuncArgs&)*a,b);}
	/*void FunctionCall_Delayed(CompileState&s,uint64_t pos,void*f){

		if(!Instructions::_call(s,*(void**)f,pos))printf("!!!FunctionCall_Delayed FAILED!!!\n");
	}
	bool Function::compile(CompileState&s,Location rv,const Location*av,uint8_t ac){
		bool nul[256]={0};
		if(!retLoc.type){retLoc=rv;nul[255]=1;}
		for(uint8_t i=0;i<argc;i++){
			if(!argvLoc[i]&&!av[i])return 0;
			else if(argvLoc[i]==av[i].resolve());
			else if(!argvLoc[i].type){argvLoc[i]=av[i].resolve();nul[i]=1;}
			else throw std::exception();//Not ready.
		}
		if(type==0){if(!inl->compile(s))return 0;}
		else if(!ptr)s.posts+={pair<uint64_t,void*>{s.b->count,&ptr},FunctionCall_Delayed};
		else{if(!Instructions::_call(s,ptr))return 0;}
		for(uint8_t i=0;i<(sizeof(int*)<<1);i++)if(mayChange[i])s.regs%i=0;

		if(nul[255])retLoc.type=0;
		for(uint8_t i=0;i<argc;i++){
			if(nul[i])argvLoc[i].type=0;
		}
		return 1;
	}
	bool Instructions::CallArged::compile(CompileState&s){return func->compile(s,ret,argv,argc);}*/
}