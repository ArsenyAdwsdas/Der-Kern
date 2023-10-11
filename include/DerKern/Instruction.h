#pragma once
#include<stdexcept>
#include"Type.h"
namespace DerKern{
	struct CompileState;struct EvalState;
	namespace Instructions{
		struct Return0{
			bool constant=0;//if 1 then don't even think of destroying it.
			inline Return0(int){constant=1;}
			inline Return0(){}
			//static constexpr Return0*Error=(Return0*)(uint64_t)-1;	//Я БЫ С РАДОСТЬЮ, НО НЕ ПОЗВОЛЯЮТ.
			virtual void compile(CompileState&);
			virtual void eval(EvalState&);
			virtual~Return0();
		};extern Return0 ret0;
		struct Return1:Return0{
			Return0*bef;
			inline Return1(Return0*b){bef=b;}
			void compile(CompileState&)override;
			void eval(EvalState&)override;
			~Return1()override;
		};
		inline Return0*Return(Return0*b){if(b)return new Return1(b);else return&ret0;}
		
		struct jmpLine:Return0{
			int16_t lineOffset;
			inline jmpLine(int16_t o){lineOffset=o;}
			void compile(CompileState&)override;
			void eval(EvalState&)override;
		};
		struct Pong:Return0{
			inline Pong(int){constant=1;}
			void compile(CompileState&)override;
			void eval(EvalState&)override;
		};extern Pong pong;
		struct Paste;
		struct SpawnVar:Return0{
			Variable v;
			inline SpawnVar(string n,Location v,Type*t):v({t,v},n){}
			void compile(CompileState&)override;
			void eval(EvalState&)override;
		};
		
		struct NopO:Return0{pair<Type*,Location>v;void compile(CompileState&)override;void eval(EvalState&)override;}//a base instruction that "outputs" a value
		struct Init:NopO{pair<Type*,Location>v;void compile(CompileState&)override;void eval(EvalState&)override;}//calls default constructor
		namespace UOps{
			struct Not:NopO{//!
				NopO*a;//input comes from here
				inline Not(Return0*in,pair<Type*,Location>out){a=in;v=out;}
				void compile(CompileState&)override;void eval(EvalState&)override;
			};
			//~,*,-,=,$,#,+
			#define __UOp(name) struct name:Not{inline name(Return0*in,pair<Type*,Location>out):Not(in,out){}void compile(CompileState&)override;void eval(EvalState&)override;};
				__UOp(BNot)__UOp(Star)__UOp(Negate)__UOp(Equ)__UOp(Dolla)__UOp(Hash)__UOp(Plugate)
			#undef __UOp
		}
	}
	typedef Instructions::Return0 Instruction;
	typedef Instructions::NopO InstructionWithOutput;
}