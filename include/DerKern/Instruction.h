#pragma once
#ifndef VisualStudioCode_SHUT_UP//BECAUSE IT APPARENTLY DOESN'T KNOW IT EXISTS.
	#include<stdexcept>
#else
	namespace std{
		class exception{};
	};
#endif
#include"Type.h"
//#include"compile.h"
namespace DerKern{
	struct CompileState;
	struct Instruction{
		bool constant=0;//if 1 then don't even think of destroying it.
		inline Instruction(int){constant=1;}
		inline Instruction(){}
		//static constexpr Return0*Error=(Return0*)(uint64_t)-1;	//Я БЫ С РАДОСТЬЮ, НО НЕ ПОЗВОЛЯЮТ.
		virtual bool compile(CompileState&);
		virtual~Instruction();
	};
	namespace Instructions{
		// VERY unsure of whether it's needed...
		// ENUM(IDs,uint16_t)
		// 	MoreComplicated=0x8000//before that is x86
		// 	Re
		// 	ENUM_END
		typedef Instruction Return0;
		extern Return0 ret0;
		struct Return1:Return0{
			Return0*bef;
			inline Return1(Return0*b){bef=b;}
			bool compile(CompileState&)override;
			~Return1()override;
		};
		inline Return0*Return(Return0*b){if(b)return new Return1(b);else return&ret0;}
		
		struct jmpLine:Return0{
			int16_t lineOffset;
			inline jmpLine(int16_t o){lineOffset=o;}
			bool compile(CompileState&)override;
		};
		struct Pong:Return0{
			inline Pong(int){constant=1;}
			bool compile(CompileState&)override;
		};extern Pong pong;
		struct Paste;
		struct SpawnVar:Return0{
			Variable v;
			inline SpawnVar(string n,Location v,Type*t):v(n,{t,v}){}
			bool compile(CompileState&)override;
		};
		
		struct Noth:Return0{inline Noth(){}inline Noth(int){constant=1;}bool compile(CompileState&)override;};extern Noth noth;//basically does nothing.

		struct NopO:Noth{pair<Type*,Location>out;inline NopO(pair<Type*,Location>ou):out(ou){}};//a base instruction that "outputs" a value
		struct NopIO:NopO{pair<Type*,Location>in;inline NopIO(pair<Type*,Location>inp,pair<Type*,Location>out):NopO(out),in(inp){}};//a base instruction that "inputs" and "outputs" a value
		struct NopIIO:NopIO{pair<Type*,Location>in2;inline NopIIO(pair<Type*,Location>inp,pair<Type*,Location>inp2,pair<Type*,Location>out):NopIO(inp,out),in2(inp2){}};//a base instruction that "inputs" twice and "outputs" a value
		struct Init:NopO{pair<Type*,Location>v;bool compile(CompileState&)override;};//calls default constructor
		#warning Instruction.h ain't ready for use.
		/*namespace UOps{
			struct Equ:NopO{//!
				NopO*a;//input comes from here
				inline Equ(NopO*in,pair<Type*,Location>out):NopO(out){a=in;}
				void compile(CompileState&)override;void eval(EvalState&)override;
			};
			#define __UOp(name) struct name:Equ{inline name(NopO*in,pair<Type*,Location>out):Equ(in,out){}void compile(CompileState&)override;void eval(EvalState&)override;};
				__UOp(Negate)__UOp(Plusate)//-,+
				__UOp(Not)__UOp(BNot)//!,~
				__UOp(Star)//*
				__UOp(Dolla)__UOp(Hash)//$,#
			#undef __UOp
		}*/
		/*namespace Ops{
			#define __Op(name) struct name:Set{inline name(NopO*in,NopO*in2,pair<Type*,Location>out):Set(in,in2,out){}void compile(CompileState&)override;void eval(EvalState&)override;};
				__UOp(Plus)__UOp(Minus)__UOp(Mul)__UOp(Div)//+,-,*,/
				__UOp(PlusE)__UOp(MinusE)__UOp(MulE)__UOp(DivE)//+=,-=,*=,/=

				__UOp(BOr)__UOp(BAnd)__UOp(BXor)__UOp(Mod)//|,&,^,%
				__UOp(BOrE)__UOp(BAndE)__UOp(BXorE)__UOp(ModE)//|=,&=,^=,%=

				__UOp(Left)__UOp(BLeft)__UOp(BRight)__UOp(Right)//<,<<,>>,>
				__UOp(LeftE)__UOp(BLeftE)__UOp(BRightE)__UOp(RightE)//<=,<<=,>>=,>=

				__UOp(Or)__UOp(And)__UOp(Eq)__UOp(NEq)//||,&&,==,!=
			#undef __Op
		}*/
		struct TransitionTo:Return0{
			inline TransitionTo(){}inline TransitionTo(int){constant=1;}
			bool compile(CompileState&)override;
		};extern TransitionTo transitTo;
		struct TransitionFrom:Return0{
			inline TransitionFrom(){}inline TransitionFrom(int){constant=1;}
			bool compile(CompileState&)override;
		};extern TransitionFrom transitFrom;
		struct Add:Return0{// I'm trying.
			Location out,in,in2;uint8_t siz;
			inline Add(Location ou,Location inp,Location inp2,uint8_t s):out(ou),in(inp),in2(inp2){siz=s;}
			bool compile(CompileState&)override;
		};
		bool _call(CompileState&,void*);
	}
	typedef Instructions::NopO InstructionWithO;
	typedef Instructions::NopIO InstructionWithIO;
}