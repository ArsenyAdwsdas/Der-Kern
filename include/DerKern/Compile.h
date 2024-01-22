#pragma once
#include"Environment.h"
#include"Instruction.h"
#include"StandloExeMem.h"
namespace DerKern{
	#ifndef InstructionaryExtraS___
	#define InstructionaryExtraS___
	#endif
	namespace Simpler{struct insLoc;}
	struct Instructionary{//"ln()" is needed after all instructions are there
		List<Instruction*>ins;Environment*env;bool inliny;Allocery::Allo*insData;
		RegisterState::bits mayChange;
		inline void operator+=(Instruction*w){assert(w);ins+=w;}
		void compile();
		void inlineInto(CompileState&parent);//basically "compile" except it's forcibly inlined straight into the buffer with all troubly stuff moved to parent
		void eval(RegisterState::regT regs[sizeof(int*)<<1]);//Yes, it's a pretty fake "eval"(swap registers,call,swap registers). ebp gets overwritten and it's not meant to be used as is
		inline uint32_t getSize(){if(!_compiled)return 0;return*(uint32_t*)(uintptr_t(_compiled)-4);}
		union{
			void*_compiled;
			//UNFORTUNATELY IT DOESN'T CARE ABOUT WHAT IT SHOULD DO AND SOMETIMES MAKES THESE POINTERS DIFFERENT
			// void(*call)();//use it like a normal function
		};
		inline Instructionary(Environment*e){insData=0;inliny=0;_compiled=0;env=e;}
		List<uint16_t>lns;inline void ln(){lns.append(ins.count);}
		InstructionaryExtraS___;
		inline~Instructionary(){if(env)env->exeMem.free(_compiled);for(uint16_t i=0;i<ins.count;i++)if(!ins[i]->constant)delete ins[i];}

		static Instructionary&EVALER();//That's what "eval" uses... args=(RegisterState::regT*) where ebp is what to "eval"
		template<typename T>inline T*allo(){if(!insData){return new T();}T*_=(T*)insData->valloc(sizeof(T));new(_)T();_->constant=1;return _;}
		template<typename T>inline void add(){(*this)+=allo<T>();}
		template<typename T,typename A>inline T*allo(A a){if(!insData){return new T(a);}T*_=(T*)insData->valloc(sizeof(T));new(_)T(a);_->constant=1;return _;}
		template<typename T,typename A>inline void add(A a){(*this)+=allo<T>(a);}
		template<typename T,typename A,typename B>inline T*allo(A a,B b){if(!insData){return new T(a,b);}T*_=(T*)insData->valloc(sizeof(T));new(_)T(a,b);_->constant=1;return _;}
		template<typename T,typename A,typename B>inline void add(A a,B b){(*this)+=allo<T>(a,b);}
		template<typename T,typename A,typename B,typename C>inline T*allo(A a,B b,C c){if(!insData){return new T(a,b,c);}T*_=(T*)insData->valloc(sizeof(T));new(_)T(a,b,c);_->constant=1;return _;}
		template<typename T,typename A,typename B,typename C>inline void add(A a,B b,C c){(*this)+=allo<T>(a,b,c);}
		template<typename T,typename A,typename B,typename C,typename D>inline T*allo(A a,B b,C c,D d){if(!insData){return new T(a,b,c,d);}T*_=(T*)insData->valloc(sizeof(T));new(_)T(a,b,c,d);_->constant=1;return _;}
		template<typename T,typename A,typename B,typename C,typename D>inline void add(A a,B b,C c,D d){(*this)+=allo<T>(a,b,c,d);}
		inline void add(Instruction*i){(*this)+=i;}
		inline void pasteTo(Instructionary*i)const{assert(!_compiled);i->ins.movFrom(ins);}
		Simpler::insLoc operator[](Location l);
	};
	template<>inline void Instructionary::add<Instructions::Return0>(){(*this)+=&Instructions::ret;}
	template<>inline void Instructionary::add<Instructions::Noth>(){(*this)+=&Instructions::noth;}
	template<>inline void Instructionary::add<Instructions::StackPURGE>(){(*this)+=&Instructions::stackPURGE;}
	template<>inline void Instructionary::add<Instructions::TransitionTo>(){(*this)+=&Instructions::transitTo;}
	template<>inline void Instructionary::add<Instructions::TransitionFrom>(){(*this)+=&Instructions::transitFrom;}
	struct DefaultableVariable:_Variable{
		Instructionary*deflt;//if you're not gonna init the variable then use <deflt> if it's not 0
		inline void operator()(Instructionary*ins)const{if(deflt)deflt->pasteTo(ins);}
		inline DefaultableVariable(Type*a,Location b):_Variable(a,b){deflt=0;}
		inline DefaultableVariable(_Variable w):_Variable(w){deflt=0;}
		inline DefaultableVariable(Type*a,Location b,Instructionary*c):_Variable(a,b){deflt=c;}
	};
	struct CompileState{
		Instructionary*origin;Environment*e;bool inlining;
		uint16_t i,line;//the current instruction and the current line
		List<uint32_t>lns;//indecies of the first bytes each(except the last one) line doesn't take, do fix 'em all if manually editing "BBuf"
		BBuf*b;
		uint16_t stackClaimed,stackUsed;
		// inline Location stackClaim(uint16_t w){stacked+=w;Instructions::StackClaim(w).compile(*this);return Location::loca(stacked);}
		// inline Location stackClaimed(uint16_t w){stacked+=w;return Location::loca(stacked);}
		// inline void stackUnclaim(uint16_t w){stacked-=w;Instructions::StackUnclaim(w).compile(*this);}
		// inline void stackUnclaimed(uint16_t w){stacked-=w;}
		inline void ln(uint32_t i){lns.append(i);}
		List<pair<pair<uint32_t,void*>,bool(*)(CompileState&,uint32_t,void*)>>posts;Allocery::Linear postsData;void post();
		inline~CompileState(){posts.death();lns.death();}

		//My logic kinda became dead inside when I realized how troublesome all "jmp" s--- will make it...
		//KnownStateR regs;//For optimizing. You Do Realise That Threads Don't Just Use Same Registers, right?
		//KnownStateF flgs;//For optimizing. You Do Realise That Threads Don't Just Use Same Status Flags, right?.. How troublesome... I'll have to SOMEHOW update 'em myself...
		////KnownStateM mem;//THAT probably shouldn't be done/used because of threads and sh--... Will maybe end up doing so though...
		
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

		inline CompileState(){stackRegister=4;line=i=0;stackClaimed=stackUsed=0;}
		inline CompileState(Environment*env,Instructionary*orig,BBuf*buf,bool inl=0):CompileState(){e=env;origin=orig;inlining=inl;b=buf;}
		uint8_t stackRegister;
	};
	// namespace Instructions{struct Paste:Instruction{
	// 	Instructionary r;
	// 	inline Paste(Instructionary z){r=z;}
	// 	bool compile(CompileState&)override;
	// 	~Paste()override;
	// };}
}
