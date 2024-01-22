#pragma once
#include"Type.h"
//#include"Compile.h"
namespace DerKern{
	struct CompileState;struct Function;
	struct Instruction{
		bool constant=0;//if 1 then don't even think of destroying it.
		inline Instruction(int){constant=1;}
		inline Instruction(){}
		virtual bool compile(CompileState&);
		virtual~Instruction();

		virtual RegisterState::bits outputs(CompileState&)const;//maybe wanted changes to registers
		virtual RegisterState::bits inputs(CompileState&)const;//what registers change the outcome
		virtual RegisterState::bits undesired(CompileState&)const;//unwanted changes to registers
		struct TmpLoc{
			Location*p;//if the pointer isn't 0 then you should change it to whatever Location you don't mind changing
			uint8_t siz;
			bool canIgnor;//1 when you're allowed to ignore
			uint8_t typeExpect;//0 to ignore or Location type you want
		};
		virtual TmpLoc temps(CompileState&,uint8_t index);
	};
	namespace Instructions{
		extern Instruction ret;typedef Instruction Return0;
		/*struct Ret:Return0{
			inline Ret(int){constant=1;}
			bool compile(CompileState&)override;
		};extern Ret ret;
		struct RetStacked:Return0{
			inline RetStacked(int){constant=1;}
			bool compile(CompileState&)override;
		};extern RetStacked retStacked;
		struct RetJmp:Return0{
			inline RetJmp(int){constant=1;}
			bool compile(CompileState&)override;
		};extern RetJmp retJmp;*/
		
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
		struct call:Return0{Location loc;inline call(Location l){loc=l;}bool compile(CompileState&)override;};
		
		struct Noth:Return0{inline Noth(){}inline Noth(int){constant=1;}bool compile(CompileState&)override;};extern Noth noth;//basically does nothing.

		//a base instruction that "outputs" a value
			struct NopO:Noth{Location out;uint16_t outSize;inline NopO(Location ou,uint16_t s){out=ou;outSize=s;}RegisterState::bits outputs(CompileState&)const override;};
		//a base instruction that "outputs" a value according to "input"
			struct NopOI:NopO{Location inp;inline NopOI(Location ou,uint16_t s,Location in):NopO(ou,s){inp=in;}RegisterState::bits inputs(CompileState&)const override;};
		//a base instruction that "outputs" a value according to the "output" of NopO
			struct NopIO:NopO{NopO*inpI;inline NopIO(Location ou,uint16_t s,NopO*in):NopO(ou,s){inpI=in;}RegisterState::bits inputs(CompileState&)const override;RegisterState::bits outputs(CompileState&)const override;RegisterState::bits undesired(CompileState&)const override;};
		//And these are for when 2 "DReg Location"s lead to a register requirement
			struct NopOI_v2:NopOI{Location useReg;inline NopOI_v2(Location ou,uint16_t s,Location in):NopOI(ou,s,in),useReg(){}RegisterState::bits undesired(CompileState&)const override;TmpLoc temps(CompileState&,uint8_t i)override;};
			struct NopIO_v2:NopIO{Location useReg;inline NopIO_v2(Location ou,uint16_t s,NopO*in):NopIO(ou,s,in),useReg(){}RegisterState::bits undesired(CompileState&)const override;TmpLoc temps(CompileState&,uint8_t i)override;};

		struct Set8:NopO{uint8_t val;inline Set8(Location t,uint8_t v):NopO(t,1){val=v;}bool compile(CompileState&)override;};
		struct Set16:NopO{uint16_t val;inline Set16(Location t,uint16_t v):NopO(t,2){val=v;}bool compile(CompileState&)override;};
		struct Set32:NopO{uint32_t val;inline Set32(Location t,uint32_t v):NopO(t,4){val=v;}bool compile(CompileState&)override;};
		struct Set64:NopO{uint64_t val;inline Set64(Location t,uint64_t v):NopO(t,8){val=v;}bool compile(CompileState&)override;};
		struct Set:NopOI_v2{inline Set(Location t,Location f,uint8_t s):NopOI_v2(t,s,f){}bool compile(CompileState&)override;};
		struct memecpy:NopOI_v2{inline memecpy(Location t,Location f,uint16_t s):NopOI_v2(t,s,f){}bool compile(CompileState&)override;};//basically "Set" except tries to work with any size
		struct SetO:NopIO{inline SetO(Location t,NopO*f):NopIO(t,f->outSize,f){}bool compile(CompileState&)override;RegisterState::bits undesired(CompileState&)const override;};
		struct Swap:Set{inline Swap(Location t,Location f,uint8_t s):Set(t,f,s){}bool compile(CompileState&)override;};
		struct Lea:NopO{Location loc;inline Lea(Location t,Location l):NopO(t,sizeof(int*)){loc=l;}bool compile(CompileState&)override;};

		struct Add8_1:NopO{uint8_t val;inline Add8_1(Location t,uint8_t v):NopO(t,1){val=v;}bool compile(CompileState&)override;};
		struct Add16_1:NopO{uint16_t val;inline Add16_1(Location t,uint16_t v):NopO(t,2){val=v;}bool compile(CompileState&)override;};
		struct Add32_1:NopO{int32_t val;inline Add32_1(Location t,uint32_t v):NopO(t,4){val=v;}bool compile(CompileState&)override;};
		struct Add_1:NopOI_v2{uint8_t size;inline Add_1(Location t,Location f,uint8_t s):NopOI_v2(t,s,f){}bool compile(CompileState&)override;};


		struct KeepStack:Noth{Instruction*stuff;inline KeepStack(Instruction*s){stuff=s;}bool compile(CompileState&)override;RegisterState::bits inputs(CompileState&)const override;RegisterState::bits outputs(CompileState&)const override;RegisterState::bits undesired(CompileState&)const override;};
		struct KeepRegs:KeepStack{RegisterState::bits regs=0;inline KeepRegs(Instruction*s, RegisterState::bits b):KeepStack(s){regs=b;}bool compile(CompileState&)override;RegisterState::bits inputs(CompileState&)const override;RegisterState::bits outputs(CompileState&)const override;RegisterState::bits undesired(CompileState&)const override;};
		struct Scope:Noth{Instruction**stuff;uint16_t stuffCount;inline Scope(Instruction**s,uint16_t c){stuff=s;stuffCount=c;}bool compile(CompileState&)override;RegisterState::bits inputs(CompileState&)const override;RegisterState::bits outputs(CompileState&)const override;RegisterState::bits undesired(CompileState&)const override;};
		struct ScopeReversed:Scope{inline ScopeReversed(Instruction**s,uint16_t c):Scope(s,c){}bool compile(CompileState&)override;};

		struct StackPURGE:Noth{inline StackPURGE(){}inline StackPURGE(int){constant=1;}bool compile(CompileState&)override;RegisterState::bits inputs(CompileState&)const override;RegisterState::bits outputs(CompileState&)const override;};extern StackPURGE stackPURGE;
		struct StackAlign:StackPURGE{uint8_t al,of;inline StackAlign(uint8_t align,uint8_t BytesAfterAligned=0){al=align;of=BytesAfterAligned;}bool compile(CompileState&)override;};
		struct StackRequire:StackPURGE{uint16_t v;inline StackRequire(uint16_t w){v=w;}bool compile(CompileState&)override;};
		struct StackUnRequire:StackPURGE{uint16_t v;inline StackUnRequire(uint16_t w){v=w;}bool compile(CompileState&)override;};
		struct StackUnRequire_d:StackPURGE{uint16_t v;inline StackUnRequire_d(uint16_t w){v=w;}bool compile(CompileState&)override;};//A notification that <v> bytes at stack are already thrown away by something
		struct StackAllo:StackPURGE{uint16_t s;Location*dst;inline StackAllo(uint16_t alloSize,Location*d){s=alloSize;dst=d;}bool compile(CompileState&)override;};
		struct StackPrepare:StackPURGE{uint16_t s;inline StackPrepare(uint16_t w){s=w;}bool compile(CompileState&)override;};

		//ebp doesn't matter and it's not meant to be used as is
		struct TransitionTo:Noth{inline TransitionTo(){}inline TransitionTo(int){constant=1;}bool compile(CompileState&)override;};extern TransitionTo transitTo;
		struct TransitionFrom:Noth{inline TransitionFrom(){}inline TransitionFrom(int){constant=1;}bool compile(CompileState&)override;};extern TransitionFrom transitFrom;
		bool _set(CompileState&,Location,RegisterState::regT v);
		bool _call(CompileState&,Location);
	}
	typedef Instructions::NopO InstructionWO;
	//typedef Instructions::NopIO InstructionWithIO;
}
#if defined(__x86_64__) || defined(_M_X64) || defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)//https://stackoverflow.com/a/66249936
	#include"platformie/x86.h"
#endif