#pragma once
#include"Environment.h"
#include"Instruction.h"
#include"StandloExeMem.h"
namespace DerKern{
	#ifndef InstructionaryExtraS___
	#define InstructionaryExtraS___
	#endif
	struct Instructionary{//"ln()" is needed after all instructions are there
		ByteBooly<(sizeof(int*)>>2)>preserves;
		List<Instruction*>ins;Environment*env;bool inliny;
		inline void operator+=(Instruction*w){ins+=w;}
		void compile();
		void inlineInto(BBuf*,CompileState&parent);//basically "compile" except it's forcibly inlined straight into the buffer with all troubly stuff moved to parent
		void eval(RegisterState::regT regs[sizeof(int*)<<1]);//Yes, it's a pretty fake "eval"(swap registers,call,swap registers). ebp gets overwritten and it's not meant to be used as is
		inline uint32_t getSize(){if(!_compiled)return 0;return*(uint32_t*)(uintptr_t(_compiled)-4);}
		union{
			void*_compiled;
			//UNFORTUNATELY IT DOESN'T CARE ABOUT WHAT IT SHOULD DO AND SOMETIMES MAKES THESE POINTERS DIFFERENT
			// void(*call)();//use it like a normal function
		};
		inline Instructionary(Environment*e){preserves(4,preserves(5,1));inliny=0;_compiled=0;env=e;}
		List<uint16_t>lns;inline void ln(){lns.append(ins.count);}
		InstructionaryExtraS___;
		inline~Instructionary(){env->exeMem.free(_compiled);for(uint16_t i=0;i<ins.count;i++)if(!ins[i]->constant)delete ins[i];}

		static Instructionary&EVALER();//That's what "eval" uses... args=(RegisterState::regT*) where ebp is what to "eval"
	};
	struct CompileState{
		ByteBooly<(sizeof(int*)>>2)>usedRegisters;
		Instructionary*origin;Environment*e;bool inlining;
		uint16_t i,line;//the current instruction and the current line
		List<uint32_t>lns;//indecies of the first bytes each(except the last one) line doesn't take, do fix 'em all if manually editing "BBuf"
		BBuf*b;
		inline void ln(uint32_t i){lns.append(i);}
		inline bool UnusedRegister(uint8_t*r){for(uint8_t i=0;i<(sizeof(int*)<<1);i++)if(!usedRegisters(i)){*r=i;return 1;}return 0;}
		List<pair<pair<uint32_t,void*>,bool(*)(CompileState&,uint32_t,void*)>>posts;Allocery::Linear postsData;void post();
		inline~CompileState(){posts.death();lns.death();}

		//My logic kinda became dead inside when I realized how troublesome all "jmp" s--- will be...
		//KnownStateR regs;//For optimizing. You Do Realise That Threads Don't Just Use Same Registers, right?
		//KnownStateF flgs;//For optimizing. You Do Realise That Threads Don't Just Use Same Status Flags, right?.. How troublesome... I'll have to SOMEHOW update 'em myself...
		//KnownStateM mem;//THAT probably shouldn't be done/used because of threads and sh--... Will end up doing it though...
		
		bool compile();

		//https://langdev.stackexchange.com/questions/1570/how-can-we-backpatch-differently-sized-jump-instructions almost had me rely on internet for how to do it... Though I'm implementing it myself since I can't decipher all these eldritch arcane spells they expect people to understand...
		Dicto<string,uint32_t,uint16_t,16,cmp>labls;
		struct Backpaw{
			uint8_t type;union{
				int32_t abs;//0 absolute
				Location loca;//1
				uint16_t line;//2
				string*labl;//3
			};//4=inline ret
			inline Backpaw(int32_t a){type=0;abs=a;}
			inline Backpaw(Location l){type=1;loca=l;}
			inline static Backpaw Line(uint16_t l){Backpaw w;w.type=2;w.line=l;}
			inline Backpaw(string*l){type=3;labl=l;}
			inline Backpaw(){type=4;}
		};
		SortList<pair<uint32_t,Backpaw>,uint16_t,16,cmpA<uint32_t,Backpaw,cmp>>backpawchy;// 'tis a bother...

		inline CompileState(){usedRegisters(4,usedRegisters(5,1));}
		inline CompileState(Environment*env,Instructionary*orig,bool inl=0):CompileState(){e=env;origin=orig;inlining=inl;}
	};
	// namespace Instructions{struct Paste:Instruction{
	// 	Instructionary r;
	// 	inline Paste(Instructionary z){r=z;}
	// 	bool compile(CompileState&)override;
	// 	~Paste()override;
	// };}
}
