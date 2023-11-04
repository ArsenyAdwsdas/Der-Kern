#pragma once
#include"Type.h"
#include"compile.h"
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
		static FuncType*Get(Type*ret,Type**argv,uint8_t argc);
		static SortList<FuncType*,uint16_t,16,cmp>_funcs;
	};
	struct Instruction;struct CompileState;
	struct Function:FuncBase{
		inline FuncType*TYPE()const{return FuncType::Get(rets,argv,argc);}
		ENUM(Types,uint8_t)
			Inline,//That's a literal Copy-Paste. Feel free to change argv[i].b
			Conwenty,//And that's a comfy(but not really optimizable) "call" with a "ret" that follows call conventions.
			Unconwenty//An uncomfy "Instructions::x86_64::call" to "ptr" that doesn't follow call conventions.
			ENUM_END
		Types::T type;
		union{
			Instruction*inl;//what to "paste"
			void*ptr;//what to "call"
		};
		Location retLoc,*argvLoc;//basically rets,argv except it stores "Location"s instead of types

		//A lot of thinking is needed to figure out a good way to implement my "why save registers so much when it's not needed?" idea... Well that's the price of overthinking and being stubborn about optimizing the hell out of everything...
		bool mayChange[sizeof(int*)<<1];//what registers may change. [4](esp)+[5](ebp) will probably mean "special esp/ebp hell"("swap sp,bp", all the actions, ret value to sp, swap again, "ret")

		inline Function(Type*rets,Type**argv,uint8_t argc,Types::T t=Types::Conwenty):FuncBase(rets,argv,argc){
			//https://stackoverflow.com/a/38974980 oh hi...
			type=t;
			argvLoc=(Location*)malloc(sizeof(Location)*argc);
			for(uint8_t i=0;i<argc;i++)argvLoc[i]=Location();
			for(uint8_t i=0;i<sizeof(int*)<<1;i++)mayChange[i]=0;
			
			if(t==1){
				//Thanks https://stackoverflow.com/a/5273354
				#if UINTPTR_MAX == 0xffffffff//Now what registers may change there?..
					#ifdef CALL_CONVENTION//#if CALL_CONVENTION==__cdecl//Thanks https://en.wikipedia.org/wiki/X86_calling_conventions#cdecl
						for(uint8_t i=0;i<3;i++)mayChange[i]=1;
					//#elif CALL_CONVENTION==__stdcall//THANK YOU http://jdebp.info/FGA/function-calling-conventions.html FOR CONFIRMING THE REGISTERS.
					//	for(uint8_t i=0;i<3;i++)mayChange[i]=1;
					#else//The hell is that?
						//I dunno what idiot(callee) may dare try changing esp and ebp so let's not mark 'em ourselves. Everything else... Well...
						for(uint8_t i=0;i<4;i++)mayChange[i]=1;
						for(uint8_t i=6;i<(sizeof(int*)<<1);i++)mayChange[i]=1;
					#endif
				#elif _WIN64//Following "Microsoft x64 calling convention" where those registers are allowed to be changed by callee.
					for(uint8_t i=0;i<3;i++)mayChange[i]=1;
					for(uint8_t i=8;i<12;i++)mayChange[i]=1;
				#else//Following "System V AMD64 ABI" where those registers are allowed to be changed by callee.
					for(uint8_t i=0;i<4;i++)mayChange[i]=1;
					for(uint8_t i=6;i<12;i++)mayChange[i]=1;
				#endif
			}
		}
		Function(const FuncType*);//This'll have some problems. For now it can't be usable.
		bool compile(CompileState&,Location ret,const Location*argv,uint8_t argc);
	};
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