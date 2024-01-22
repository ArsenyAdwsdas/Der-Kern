#pragma once
#include"../include/DerKern/Functions.h"
#include"Type.cpp"
#include"Compile.cpp"
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

	// int __cdecl cd(int){int z;return 1;}
	// int __stdcall st(int){int z;return 1;}
	// int main(){
	// 	cd(1);
	// 	st(1);
	// 	return 2;
	// }
	namespace Instructions{
		RegisterState::bits CallQuick::inputs(CompileState&c)const{RegisterState::bits _=0;for(uint8_t i=0;i<argc&&~_;i++)_|=argv[i]->inputs(c);return _;}
		RegisterState::bits CallQuick::outputs(CompileState&c)const{return func->retCalc().regDireBit();}
		RegisterState::bits CallQuick::undesired(CompileState&c)const{RegisterState::bits _=func->mayChange();for(uint8_t i=0;i<argc&&~_;i++)_|=argv[i]->outputs(c)|argv[i]->undesired(c);return _&~func->retCalc().regDireBit();}
	}
	bool Instructions::CallQuick::compile(CompileState&co){return func->_call(co,out,argv,argc);}
	bool Instructions::RetCalc::compile(CompileState&co){*ret=func->retCalc();return 1;}
	bool Instructions::ArgCalc::compile(CompileState&co){func->argCalc(ret);return 1;}
	bool Instructions::AllCalc::compile(CompileState&co){func->allCalc(ret);return 1;}
	/*bool Instructions::Call::compile(CompileState&co){
		uint16_t r=CallQuick::mayChange(co)&~func->mayChange(),_;
		if(!Instructions::SaveRegs(r,&_).compile(co))return 0;
		if(!func->_call(co,out,argv,argc))return 0;
		return Instructions::LoadRegs(_,&r).compile(co);
	}*/
	bool Function::_call(CompileState&co,Location ret,Instructions::NopO**av,uint8_t ac){//TODO: TEST ALL
		if(ac!=type->argc){Error("ARGUMENT MISMATCH IN Function::_call");return 0;}
		uint16_t olden=co.stackUsed;//can't reasonably know how it changes without saving.
		Location args[256]={};
		bool memRet=0;
		if(conw==Function::Conwenties::CDecl){//https://en.wikipedia.org/wiki/X86_calling_conventions#cdecl
			memRet=type->rets->typeID!=Type::IDs::inte&&type->rets->typeID!=Type::IDs::flot;
			Instructions::StackAlign(sizeof(int*)).compile(co);
			if(!ret&&memRet&&!Instructions::StackAllo(type->rets->size,args+255).compile(co))return 0;//just in case the caller doesn't want the return value
			Instructions::StackAlign(sizeof(int*)).compile(co);
			for(uint8_t i=ac-1;i!=(uint8_t)-1;i--)//makes all the arguments that have move constructors since that's the most sane option
				if(type->argv[i]->memba("+this+")&&(!Instructions::StackAllo(ceilSize(type->argv[i]->size,sizeof(int*)),args+i).compile(co)||!Instructions::SetO(args+i,av[i]).compile(co)))return 0;
			
			#if defined(__GNUC__)//gcc hi!.. //https://en.wikipedia.org/wiki/X86_calling_conventions#cdecl
				Instructions::StackAlign(16,16-sizeof(int*)).compile(co);
			#else
				Instructions::StackAlign(sizeof(int*)).compile(co);
			#endif
			Instruction*_scp[257];Instructions::ScopeReversed scp(_scp,0);Allocery::AlloclessLinear<> alo;uint16_t stackNeeds=0;//Because I need a way to make arguments go in the opposite direction
			if(memRet){//add a secret argument: return address
				stackNeeds+=sizeof(int*);
				_scp[scp.stuffCount++]=alo.make<Instructions::Lea>(Location::stack(co.stackUsed+stackNeeds),args+255);
			}
			for(uint8_t i=0;i<ac;i++){//"pushes" all the arguments...
				if(args[i]){
					stackNeeds+=sizeof(int*);
					_scp[scp.stuffCount++]=alo.make<Instructions::Lea>(Location::stack(co.stackUsed+stackNeeds),args+i);
				}else{
					uint16_t w=ceilSize(type->argv[i]->size,sizeof(int*));stackNeeds+=w;//should just round up the size
					_scp[scp.stuffCount++]=alo.make<Instructions::SetO>(Location::stack(co.stackUsed+stackNeeds),av[i]);
				}
			}
			#if defined(__GNUC__)//Look, I know it may look bad but it seems to be the best way
				stackNeeds=ceilSize(stackNeeds,16);
			#endif
			if(stackNeeds<co.stackClaimed-co.stackUsed)_scp[scp.stuffCount++]=alo.make<Instructions::StackRequire>(co.stackClaimed-co.stackUsed-stackNeeds);
			else _scp[scp.stuffCount++]=alo.make<Instructions::StackPrepare>(stackNeeds-co.stackClaimed+co.stackUsed);
			scp.compile(co);

			if(raw){if(!Instructions::call(p).compile(co))return 0;}else if(((Instructionary*)p)->inliny)((Instructionary*)p)->inlineInto(co);else if(!Instructions::call(((Instructionary*)p)->_compiled).compile(co))return 0;

			if(type->rets->typeID==Type::IDs::flot){if(!Instructions::Set(ret,Location::st(0),type->rets->size).compile(co))return 0;}
		}else if(conw==Function::Conwenties::StdCall){//https://en.wikipedia.org/wiki/X86_calling_conventions#stdcall it's literally just CDecl except stack adjusting because MS decided they want to modify "esp"... And the alignment's always 4 instead of questionable GCC mistake-to-decision 16 byte alignment
			memRet=type->rets->typeID!=Type::IDs::inte&&type->rets->typeID!=Type::IDs::flot;
			Instructions::StackAlign(sizeof(int*)).compile(co);
			if(!ret&&memRet&&!Instructions::StackAllo(type->rets->size,args+255).compile(co))return 0;//just in case the caller doesn't want the return value
			Instructions::StackAlign(sizeof(int*)).compile(co);
			for(uint8_t i=ac-1;i!=(uint8_t)-1;i--)//makes all the arguments that have move constructors since that's the most sane option
				if(type->argv[i]->memba("+this+")&&(!Instructions::StackAllo(ceilSize(type->argv[i]->size,sizeof(int*)),args+i).compile(co)||!Instructions::SetO(args+i,av[i]).compile(co)))return 0;
			
			Instructions::StackAlign(sizeof(int*)).compile(co);
			Instruction*_scp[257];Instructions::ScopeReversed scp(_scp,0);Allocery::AlloclessLinear<> alo;uint16_t stackNeeds=0;//Because I need a way to make arguments go in the opposite direction
			if(memRet){//add a secret argument: return address
				stackNeeds+=sizeof(int*);
				_scp[scp.stuffCount++]=alo.make<Instructions::Lea>(Location::stack(co.stackUsed+stackNeeds),args+255);
			}
			for(uint8_t i=0;i<ac;i++){//"pushes" all the arguments...
				if(args[i]){
					stackNeeds+=sizeof(int*);
					_scp[scp.stuffCount++]=alo.make<Instructions::Lea>(Location::stack(co.stackUsed+stackNeeds),args+i);
				}else{
					uint16_t w=ceilSize(type->argv[i]->size,sizeof(int*));stackNeeds+=w;//should just round up the size
					_scp[scp.stuffCount++]=alo.make<Instructions::SetO>(Location::stack(co.stackUsed+stackNeeds),av[i]);
				}
			}
			if(stackNeeds<co.stackClaimed-co.stackUsed)_scp[scp.stuffCount++]=alo.make<Instructions::StackRequire>(co.stackClaimed-co.stackUsed-stackNeeds);
			else _scp[scp.stuffCount++]=alo.make<Instructions::StackPrepare>(stackNeeds-co.stackClaimed+co.stackUsed);
			scp.compile(co);

			if(raw){if(!Instructions::call(p).compile(co))return 0;}else if(((Instructionary*)p)->inliny)((Instructionary*)p)->inlineInto(co);else if(!Instructions::call(((Instructionary*)p)->_compiled).compile(co))return 0;
			Instructions::StackUnRequire_d(stackNeeds).compile(co);//YES, Microsoft CAUSED THIS.

			if(type->rets->typeID==Type::IDs::flot){if(!Instructions::Set(ret,Location::st(0),type->rets->size).compile(co))return 0;}
		}else if(conw==Function::Conwenties::MicrosoftX64){
			static uint8_t R[]={9,8,2,1};//reverse order because optimizing for "R[r]" to be argument location
			uint8_t r=sizeof(R)-1,f=3;
			Instructions::StackAlign(16,16-sizeof(int*)).compile(co);
			memRet=type->rets->memOnly();
			Instructions::StackAlign(sizeof(int*)).compile(co);
			if(!ret&&memRet&&!Instructions::StackAllo(type->rets->size,args+255).compile(co))return 0;//just in case the caller doesn't want the return value
			Instructions::StackAlign(sizeof(int*)).compile(co);
			for(uint8_t i=ac-1;i!=(uint8_t)-1;i--)//makes all the arguments that have move constructors since that's the most sane option
				if(type->argv[i]->memOnly()&&(!Instructions::StackAllo(ceilSize(type->argv[i]->size,sizeof(int*)),args+i).compile(co)||!Instructions::SetO(args+i,av[i]).compile(co)))return 0;
			
			Instructions::StackAlign(16,8).compile(co);
			Instruction*_scp[257];Instructions::ScopeReversed scp(_scp,0);Allocery::AlloclessLinear<> alo;uint16_t stackNeeds=0;
			if(memRet)_scp[scp.stuffCount++]=alo.make<Instructions::Lea>(R[r--],args+255);
			for(uint8_t i=0;i<ac;i++){
				if(args[i]){
					stackNeeds+=sizeof(int*);
					_scp[scp.stuffCount++]=alo.make<Instructions::Lea>(Location::stack(co.stackUsed+stackNeeds),args+i);
				}else if(type->argv[i]->memOnly()||(type->argv[i]->typeID==Type::IDs::flot?f:r)==255){
					uint16_t w=ceilSize(type->argv[i]->size,sizeof(int*));stackNeeds+=w;//should just round up the size
					_scp[scp.stuffCount++]=alo.make<Instructions::SetO>(Location::stack(co.stackUsed+stackNeeds),av[i]);
				}else if(type->argv[i]->typeID==Type::IDs::flot)_scp[scp.stuffCount++]=alo.make<Instructions::SetO>(Location::xmm(3-(f--)),av[i]);
				else _scp[scp.stuffCount++]=alo.make<Instructions::SetO>(R[r--],av[i]);
			}
			if(stackNeeds<32)stackNeeds=32;//"Shadow space"... FU--- YOU! GO TO THE SHADOW REALM!
			else stackNeeds=ceilSize(stackNeeds,16);// https://codereview.stackexchange.com/q/278940 thanks for clarifying how to align
			if(stackNeeds<co.stackClaimed-co.stackUsed)_scp[scp.stuffCount++]=alo.make<Instructions::StackRequire>(co.stackClaimed-co.stackUsed-stackNeeds);
			else _scp[scp.stuffCount++]=alo.make<Instructions::StackPrepare>(stackNeeds-co.stackClaimed+co.stackUsed);
			scp.compile(co);

			if(raw){if(!Instructions::call(p).compile(co))return 0;}else if(((Instructionary*)p)->inliny)((Instructionary*)p)->inlineInto(co);else if(!Instructions::call(((Instructionary*)p)->_compiled).compile(co))return 0;

			if(type->rets->typeID==Type::IDs::flot){if(!Instructions::Set(ret,Location::xmm(0),type->rets->size).compile(co))return 0;}
		}else if(conw==Function::Conwenties::SystemVAMD64ABI){
			static uint8_t R[]={9,8,2,3,6,7};//reverse order because optimizing for "R[r]" to be argument location
			uint8_t r=sizeof(R)-1,f=7;
			memRet=type->rets->memOnly();
			Instructions::StackAlign(sizeof(int*)).compile(co);
			if(!ret&&memRet&&!Instructions::StackAllo(type->rets->size,args+255).compile(co))return 0;//just in case the caller doesn't want the return value
			Instructions::StackAlign(sizeof(int*)).compile(co);
			for(uint8_t i=ac-1;i!=(uint8_t)-1;i--)//makes all the arguments that have move constructors since that's the most sane option
				if(type->argv[i]->memba("+this+")&&(!Instructions::StackAllo(ceilSize(type->argv[i]->size,sizeof(int*)),args+i).compile(co)||!Instructions::SetO(args+i,av[i]).compile(co)))return 0;
			
			Instructions::StackAlign(16,8).compile(co);
			Instruction*_scp[257];Instructions::ScopeReversed scp(_scp,0);Allocery::AlloclessLinear<> alo;uint16_t stackNeeds=0;
			if(memRet)_scp[scp.stuffCount++]=alo.make<Instructions::Lea>(R[r--],args+255);
			for(uint8_t i=0;i<ac;i++){
				if(args[i]){
					stackNeeds+=sizeof(int*);
					_scp[scp.stuffCount++]=alo.make<Instructions::Lea>(Location::stack(co.stackUsed+stackNeeds),args+i);
				}else if(type->argv[i]->memOnly()||(type->argv[i]->typeID==Type::IDs::flot?f:r)==255){
					uint16_t w=ceilSize(type->argv[i]->size,sizeof(int*));stackNeeds+=w;//should just round up the size
					_scp[scp.stuffCount++]=alo.make<Instructions::SetO>(Location::stack(co.stackUsed+stackNeeds),av[i]);
				}else if(type->argv[i]->typeID==Type::IDs::flot)_scp[scp.stuffCount++]=alo.make<Instructions::SetO>(Location::xmm(7-(f--)),av[i]);
				else _scp[scp.stuffCount++]=alo.make<Instructions::SetO>(R[r--],av[i]);
			}
			stackNeeds=ceilSize(stackNeeds,16);
			if(stackNeeds<co.stackClaimed-co.stackUsed)_scp[scp.stuffCount++]=alo.make<Instructions::StackRequire>(co.stackClaimed-co.stackUsed-stackNeeds);
			else _scp[scp.stuffCount++]=alo.make<Instructions::StackPrepare>(stackNeeds-co.stackClaimed+co.stackUsed);
			scp.compile(co);

			if(raw){if(!Instructions::call(p).compile(co))return 0;}else if(((Instructionary*)p)->inliny)((Instructionary*)p)->inlineInto(co);else if(!Instructions::call(((Instructionary*)p)->_compiled).compile(co))return 0;

			if(type->rets->typeID==Type::IDs::flot){if(!Instructions::Set(ret,Location::xmm(0),type->rets->size).compile(co))return 0;}
		}else return 0;
		if(!ret){//even if the caller doesn't want the return value, the destructor must be called.
			Type::Memba*f=type->rets->memba("~this~");if(f){
				Type*t=type->rets->pointer2();
				assert(f->type==3&&f->sta.a==FuncType::Get(&Type::VOID,&t,1));
				Instructions::Lea _={{},args[255]||(Location)(uint8_t)0};Instructions::NopO*__=&_;
				if(!((Function*)f->sta.b)->_call(co,{},&__,1))return 0;
			}
		}else if(!memRet&&!Instructions::Set(ret,(uint8_t)0,type->rets->size).compile(co))return 0;
		co.stackUsed=olden;return 1;
	}
	namespace Conwenties{
		/*bool MicrosoftX64::call(CompileState&c,Location out,Instructions::NopO**av,uint8_t ac){//https://en.wikipedia.org/wiki/X86_calling_conventions#Microsoft_x64_calling_convention
			// https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2008/ew5tede7(v=vs.90) ... "Shadow space"... FU--- YOU! GO TO THE SHADOW REALM!
			// https://stackoverflow.com/a/49397524 ... 16 byte alignment...
			// https://codereview.stackexchange.com/q/278940 thanks for clarifying how to align
			if(ac!=argTc){Error("ARGUMENT MISMATCH: MicrosoftX64::call");return 0;}
			uint16_t _stack=c.stacked;Location ou;
			if(outputType->size>sizeof(int*)){ou=out?out:Location::loca(c.stacked+outputType->size);}
			for(uint8_t i=0;i<ac;i++)if(argTv[i]->typeID==Type::IDs::flot){Error("UNIMPLEMENTED: MicrosoftX64 FLOT ARG");return 0;}
			if(ac>4-!ou){
				uint8_t I;
				if(ou){
					if(!Instructions::SetO(2,av[0]).compile(c)
					 ||!Instructions::SetO(8,av[1]).compile(c)
					 ||!Instructions::SetO(9,av[2]).compile(c))return 0;
					I=3;
				}else{
					if(!Instructions::SetO(1,av[0]).compile(c)
					 ||!Instructions::SetO(2,av[1]).compile(c)
					 ||!Instructions::SetO(8,av[2]).compile(c)
					 ||!Instructions::SetO(9,av[3]).compile(c))return 0;
					I=4;
				}
				uint16_t stackO=0;
				for(uint8_t i=0;i<argTc;i++)stackO+=argTv[i]->size;
				if(stackO>32)c.stackClaim(stackO);
				elsec.stackClaim(32);
				for(uint8_t i=argTc-1-I;i!=255;i--){
					stackO-=argTv[i]->size;
					Instructions::SetO(Location::stack(stackO),av[i]).compile(c);
				}
			}else{
				uint8_t I=0;
				if(ou){
					if((ac>0&&!Instructions::SetO(2,av[0]).compile(c))
					 ||(ac>1&&!Instructions::SetO(8,av[1]).compile(c))
					 ||(ac>2&&!Instructions::SetO(9,av[2]).compile(c)))return 0;
				}else{
					if((ac>0&&!Instructions::SetO(1,av[0]).compile(c))
					 ||(ac>1&&!Instructions::SetO(2,av[1]).compile(c))
					 ||(ac>2&&!Instructions::SetO(8,av[2]).compile(c))
					 ||(ac>3&&!Instructions::SetO(9,av[3]).compile(c)))return 0;
				}
				c.stackClaim(32);
			}
			if(ou){Instructions::Lea((uint8_t)1),ou};
			if(!Instructions::call(location).compile(c))return 0;

			if(!ou){
				if(outputType->typeID==Type::IDs::flot){//I'll deal with it later
					Error("UNIMPLEMENTED: MicrosoftX64 FLOT");return 0;
				}else if(!Instructions::Set(out,(uint8_t)0,outputType->size).compile(c))return 0;
			}

			c.stackUnclaim(c.stacked-_stackO);
			return 1;
		}*/
		/*bool SystemVAMD64ABI::call(CompileState&c,Location out,Instructions::NopO**av,uint8_t ac){//https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI
			
			Instructions::call(c,location);

		}*/
	}
}