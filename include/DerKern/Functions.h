#pragma once
#include"Type.h"
#include"Compile.h"
#include"Instruction.h"
namespace DerKern{
	struct FuncArgs{Type**argv;uint8_t argc;inline FuncArgs(Type**a,uint8_t ac){argv=a;argc=ac;}};int cmp(const FuncArgs&,const FuncArgs&);
	struct FuncBase:FuncArgs{Type*rets;inline FuncBase(Type*r,Type**argv,uint8_t argc):FuncArgs(argv,argc){rets=r;}};int cmp(const FuncBase&,const FuncBase&);
	struct FuncType;
	int cmp(FuncType*const&a,FuncType*const&b);
	int cmp(FuncType*const&a,FuncBase const&b);
	struct FuncType:Type,FuncBase{
		inline static string NAME(Type*rets,Type**argv,uint8_t argc){
			string _;BBuf b;
			b.append((uint8_t*)rets->name.c_str(),rets->name.size());
			b+='(';if(argc)b.append((uint8_t*)argv[0]->name.c_str(),argv[0]->name.size());for(uint8_t i=1;i<argc;i++){b+=',';b.append((uint8_t*)argv[i]->name.c_str(),argv[i]->name.size());}b+=')';
			return string((char*)b.raw,b.count);
		}
		inline FuncType(string n,Type*rets,Type**argv,uint8_t argc):Type(n,0),FuncBase(rets,mallocCpy(argv,argc),argc){typeID=7;}
		inline FuncType(Type*_rets,Type**_argv,uint8_t _argc):FuncType(NAME(_rets,_argv,_argc),_rets,_argv,_argc){}
		static FuncType*Get(Type*ret,Type**argv=0,uint8_t argc=0);
		static SortList<FuncType*,uint16_t,16,cmp>_funcs;
	};
	struct Instruction;struct CompileState;
	struct Function{// oh https://godbolt.org/ my saviour...
		FuncType*type;
		struct Unconwented{
			void*p;
			uint8_t stackAlign,*argRegisters,*argFRegisters;//alignment 0 disables attempts to put arguments on stack
			bool SystemVStructing;//if no move constructor for an argument then it's passed on stack as is?
			RegisterState::bits mayChange;

		};
		bool raw;void*p;//raw=0 means "Instructionary*" instead of "void*" would be more proper
		//"this ptr" shall always be forced to be arg1 when it exists
		//nullptr in Type* array represents THE ALL CONSUMING VOI- i mean... the good ol' 3 dots("...") from C, right ah ha ha..
		ENUM(Conwenties,uint8_t)
			non,//Unconwented::p becomes a replacement for "p" since "p" means "Unconwented*"
			CDecl,
			StdCall,//https://en.wikipedia.org/wiki/X86_calling_conventions#pascal https://en.wikipedia.org/wiki/X86_calling_conventions#stdcall the hell is this? stack reordering?.. YOU---
			MicrosoftX64,

			SystemVAMD64ABI//https://www.ucw.cz/~hubicka/papers/abi/node10.html THE HELL IS THAT? DO YOU WANT ME TO HAVE A SEPARATE F--- PROGRAM FOR THAT OR WHAT?
			//https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf ...
			//even worse... what a trainwreck... it appears that passing structs as pointers won't always work... that is a very brave idea they had...
			//https://www.independent.co.uk/news/uk/home-news/chart-shows-what-british-people-say-what-they-really-mean-and-what-others-understand-a6730046.html

			,myOwn,//https://stackoverflow.com/questions/22214208/x86-assembly-why-is-ebx-preserved-in-calling-conventions
			//structs shall be passed full-MS64 style
			//eax=<set to return value> or unknown
			//ecx=int/struct(ptr if too big) arg3 or not changed
			//edx=not changed
			//ebx=not changed
			//esi=int/struct(ptr if too big) arg2 or not changed
			//edi=int/struct(ptr if too big) arg1 or not changed
			//other arguments to the stack, caller clean-up

			

			#if UINTPTR_MAX == 0xffffffff//https://stackoverflow.com/a/5273354
				Default=CDecl
			#elif _WIN64
				Default=MicrosoftX64
			#else//Falling to "System V AMD64 ABI"
				Default=SystemVAMD64ABI
			#endif
			ENUM_END Conwenties::T conw;
		inline Function(){}
		private:
			//https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions
		public:
		private:inline Function(Type*rT,Type**av,uint8_t ac,Conwenties::T c,void*_,bool _r){type=FuncType::Get(rT,av,ac);conw=c;p=_;raw=_r;}public:
		inline Function(void*_,Type*rT,Type**av,uint8_t ac):Function(rT,av,ac,Conwenties::Default,_,1){}
		inline Function(Instructionary*i,Type*rT,Type**av,uint8_t ac):Function(rT,av,ac,Conwenties::Default,i,0){}
		inline Function(void*_):Function(0,0,0,Conwenties::Default,_,1){}
		inline void*toCall()const{if(raw)return p;if(!((Instructionary*)p)->_compiled)((Instructionary*)p)->compile();return((Instructionary*)p)->_compiled;}
		inline static RegisterState::bits mayChange(Conwenties::T c,const Function*thee=0){
			typedef RegisterState::bits bits;
			constexpr bits st=(bits)0xff07,cd=(bits)0xff07,ms=(bits)0xf07,sv=(bits)0xfc7,mo[]={(bits)0x21,(bits)0xa1,(bits)0xa1,(bits)0xa1};
			if(c==0)return thee?(!thee->raw&&((Instructionary*)thee->p)->inliny)?((Instructionary*)thee->p)->mayChange:((Unconwented*)thee->p)->mayChange:0;
			if(!thee->raw&&((Instructionary*)thee->p)->inliny){assert(!thee->raw);return((Instructionary*)thee->p)->mayChange;}//http://refspecs.linuxbase.org/elf/abi386-4.pdf
			if(c==Conwenties::CDecl)return cd;//https://en.wikipedia.org/wiki/X86_calling_conventions#cdecl
			if(c==Conwenties::StdCall)return st;//https://en.wikipedia.org/wiki/X86_calling_conventions#stdcall //It does like change esp but I handle that
			if(c==Conwenties::MicrosoftX64)return ms;//https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions#register-volatility-and-preservation
			if(c==Conwenties::SystemVAMD64ABI)return sv;//https://stackoverflow.com/a/18024743
			if(c==Conwenties::myOwn)return mo[(thee?thee->type->argc>2?3:thee->type->argc:3)];
			return 0;
		}inline RegisterState::bits mayChange()const{return mayChange(conw,this);}
		bool _call(CompileState&,Location ret,Instructions::NopO**av,uint8_t ac);//It doesn't preserve anything. It's THAT bad.
		inline Location retCalc()const{
			if(conw==Conwenties::CDecl||conw==Conwenties::StdCall)return type->rets->memOnly()?Location::dStack(sizeof(int*)):Location(uint8_t(0));
			if(conw==Conwenties::MicrosoftX64)return type->rets->memOnly()?Location(9,0):type->rets->typeID==Type::IDs::flot?Location::xmm(3):Location(uint8_t(0));
			if(conw==Conwenties::SystemVAMD64ABI)return type->rets->memOnly()?Location(9,0):type->rets->typeID==Type::IDs::flot?Location::xmm(7):Location(uint8_t(0));
		}
		inline void argCalc(Location out[255])const{
			if(conw==Conwenties::CDecl||conw==Conwenties::StdCall){
				uint16_t p=sizeof(int*);
				for(uint8_t i=0;i<type->argc;i++){
					if(type->argv[i]->memba("+this+")){out[i]=Location::dStack(p);p+=sizeof(int*);}
					else{out[i]=Location::loca(-(int32_t)p);p+=ceilSize(type->argv[i]->size,sizeof(int*));}
				}
			}
			if(conw==Conwenties::MicrosoftX64){
				static uint8_t R[]={9,8,2,1};//reverse order because optimizing for "R[r]" to be argument location
				uint8_t r=sizeof(R)-1,f=3;
				uint16_t p=sizeof(int*)+32;
				if(type->rets->memOnly())r--;else if(type->rets->typeID==Type::IDs::flot)f--;
				for(uint8_t i=0;i<type->argc;i++){
					if(type->argv[i]->memOnly()){if(r!=255)out[i]=Location(R[r--],0);else{out[i]=Location::dStack(p);p+=sizeof(int*);}}
					else if(type->argv[i]->typeID==Type::IDs::flot&&f!=255)out[i]=Location::xmm(3-(f--));
					else if(r!=255)out[i]=Location(R[r--]);
					else{out[i]=Location::loca(-(int32_t)p);p+=sizeof(int*);}
				}
			}
			if(conw==Conwenties::SystemVAMD64ABI){
				static uint8_t R[]={9,8,2,3,6,7};//reverse order because optimizing for "R[r]" to be argument location
				uint8_t r=sizeof(R)-1,f=7;
				uint16_t p=sizeof(int*);
				if(type->rets->memOnly())r--;else if(type->rets->typeID==Type::IDs::flot)f--;
				for(uint8_t i=0;i<type->argc;i++){
					if(type->argv[i]->memba("+this+")){out[i]=Location::dStack(p);p+=sizeof(int*);}
					else if(type->argv[i]->memOnly()||(type->argv[i]->typeID==Type::IDs::flot?f:r)==255){out[i]=Location::loca(-(int32_t)p);p+=ceilSize(type->argv[i]->size,sizeof(int*));}
					else if(type->argv[i]->typeID==Type::IDs::flot)out[i]=Location::xmm(7-(f--));
					else out[i]=Location(R[r--]);
				}
			}
		}
		inline void allCalc(Location out[256])const{argCalc(out);out[255]=retCalc();}
	};
	namespace Instructions{
		struct RetCalc:Noth{Function*func;Location*ret;inline RetCalc(Function*f,Location*a){func=f;ret=a;}bool compile(CompileState&)override;};
		struct ArgCalc:RetCalc{inline ArgCalc(Function*f,Location*a):RetCalc(f,a){}bool compile(CompileState&)override;};
		struct AllCalc:RetCalc{inline AllCalc(Function*f,Location*a):RetCalc(f,a){}bool compile(CompileState&)override;};
		struct CallQuick:NopO{//Still doesn't preserve a thing
			Function*func;
			Instructions::NopO**argv;uint8_t argc;//those args must leave esp the same
			inline CallQuick(Function*f,Instructions::NopO**av,uint8_t ac,Location ret=(uint8_t)0):NopO(ret,f->type->rets->size){func=f;argv=av;argc=ac;}
			bool compile(CompileState&)override;
			RegisterState::bits inputs(CompileState&)const override;
			RegisterState::bits outputs(CompileState&)const override;
			RegisterState::bits undesired(CompileState&)const override;
		};
		struct Call:CallQuick{//This one preserves registers Function::mayChange says to change as well as an output register if there is a need for that.
			inline Call(Function*f,Instructions::NopO**av,uint8_t ac,Location ret=(uint8_t)0):CallQuick(f,av,ac,ret){}
			bool compile(CompileState&)override;
			RegisterState::bits inputs(CompileState&)const override;
			RegisterState::bits outputs(CompileState&)const override;
			RegisterState::bits undesired(CompileState&)const override;
		};
	}
	int cmp(Function*const&a,Function*const&b);
	int cmp(Function*const&a,FuncBase const&b);
	int cmp(Function*const&a,FuncArgs const&b);
	struct OverloadedFunction{
		static inline int cmps(const pair<Type**,uint8_t>&a,const pair<Type**,uint8_t>&b){
			auto c=a.b>b.b?a.b:b.b;
			for(uint8_t i=0;i<c;i++){int _=cmp(a.a[i],b.a[i]);if(_!=0)return _;}
			return cmp(a.b,b.b);
		}
		SortList<Function*,uint16_t,16,cmp>typery;
		inline Function*tryGet(const FuncArgs&a){auto _=typery.get<FuncArgs,cmp>(a);return _?*_:0;}
	};
}