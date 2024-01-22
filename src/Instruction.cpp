#pragma once
#include"../include/DerKern/Instruction.h"
#include"Erroring.cpp"
#include"Type.cpp"
#if defined(__x86_64__) || defined(_M_X64) || defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
	#include"../include/DerKern/platformie/x86.h"
	#include"platformie/x86.cpp"
#endif
namespace DerKern{
	Instruction::~Instruction(){}
	RegisterState::bits Instruction::outputs(CompileState&)const{return 0;}
	RegisterState::bits Instruction::inputs(CompileState&)const{return 0;}
	RegisterState::bits Instruction::undesired(CompileState&)const{return 0;}
	Instruction::TmpLoc Instruction::temps(CompileState&,uint8_t index){return{0,0,0,0};}
	namespace Instructions{
		decltype(ret)ret{1};
		//RetStacked retStacked{1};
		//Pong pong{1};
		//Noth noth{1};
		TransitionTo transitTo{1};
		TransitionFrom transitFrom{1};
		StackPURGE stackPURGE{1};


		//KeepRegs::~KeepRegs(){if(!stuff->constant)delete stuff;}
		//Scope::~Scope(){for(uint16_t i=0;i<stuffCount;i++)if(!stuff[i]->constant)delete stuff[i];}
		//Return1::~Return1(){bef->~Return0();delete bef;}
		RegisterState::bits Scope::outputs(CompileState&c)const{RegisterState::bits _=0,f=-1;for(uint16_t i=stuffCount-1;i!=uint16_t(-1);i--){_=_|(stuff[i]->outputs(c)&f);f&=~stuff[i]->undesired(c);if(!((~_)&&f))return _;}return _;}
		RegisterState::bits Scope::inputs(CompileState&c)const{RegisterState::bits _=0,f=-1;for(uint16_t i=0;i<stuffCount;i++){_=_|(stuff[i]->inputs(c)&f);f&=~stuff[i]->outputs(c);if(!((~_)&&f))return _;}return _;}
		RegisterState::bits Scope::undesired(CompileState&c)const{RegisterState::bits _=0,f=-1;for(uint16_t i=0;i<stuffCount;i++){_=_|(stuff[i]->undesired(c)&f);f&=~stuff[i]->outputs(c);if(!((~_)&&f))return _;}return _;}

		bool Noth::compile(CompileState&s){return 1;}
		bool StackAlign::compile(CompileState&c){return c.stackClaimed%al==of?1:StackAlign(al-((c.stackClaimed+al-of)%al)).compile(c);}
		bool StackUnRequire::compile(CompileState&c){c.stackUsed-=v;return 1;}
		bool StackUnRequire_d::compile(CompileState&c){c.stackClaimed-=v;c.stackUsed-=v;return 1;}
		bool StackAllo::compile(CompileState&c){*dst=Location::loca(c.stackUsed+s);return StackRequire(s).compile(c);}
		bool StackPrepare::compile(CompileState&c){return StackRequire(s).compile(c)&&StackUnRequire(s).compile(c);}
		bool SetO::compile(CompileState&s){
			if(!out)return 0;outSize=inpI->outSize;
			auto&in=inpI->out.resolve();
			if(!in){in=out;bool _=inpI->compile(s);in.type=0;return _;}
			auto&ou=out.resolve();
			return inpI->compile(s)&&(in==ou||Set(ou,in,inpI->outSize).compile(s));
		}
		bool Scope::compile(CompileState&s){for(uint16_t i=0;i<stuffCount;i++)if(!stuff[i]->compile(s))return 0;return 1;}
		bool ScopeReversed::compile(CompileState&s){for(uint16_t i=stuffCount-1;i!=(uint16_t)-1;i--)if(!stuff[i]->compile(s))return 0;return 1;}
	}
}