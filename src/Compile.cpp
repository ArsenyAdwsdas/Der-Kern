#pragma once
#include"../include/DerKern/Compile.h"
#include"Environment.cpp"
#include"Instruction.cpp"
#include"StandloExeMem.cpp"
namespace DerKern{
	bool CompileState::compile(){
		//v1... I don't want to deal with it... Mostly...
		// for(uint16_t i=0;i<lns[0];i++)ins[i]->compile(c);
		// c.ln(b->count);c.line++;
		// while(lns.count>c.line){
		// 	for(uint16_t i=lns[c.line-1];i<lns[c.line];i++)ins[i]->compile(c);
		// 	c.ln(b->count);c.line++;
		// }

		#define NO_PRESERVATION
		#ifdef NO_PRESERVATION//v2 compilation
			for(;i<origin->ins.count;i++){
				while(i>=origin->lns[line])line++;
				auto I=origin->ins[i];
				if(!I->compile(*this)){
					printf("%u/%u\n",unsigned(i+1),unsigned(origin->ins.count));
					Error("FAILED: CompileState::compile");
					return 0;
				}
			}
			return 1;
		#else//v3: Extreme Spaghetti if I understand correctly how to call it...
			/*struct Rang{
				uint_t start,last;
				struct El{
					uint_t at;bool changed;El*next;
					inline El(decltype(at)a,bool b,El*c){at=a;changed=b;next=c;}inline El(decltype(at)a,bool b){at=a;changed=b;}
				};
				El*inp;uint16_t inpC;
				RegisterState::bits available;
				uint8_t reg;bool useful;
				inline Rang(decltype(last)st,decltype(last)la,El*arr,uint16_t co){start=st;last=la;inp=arr;inpC=co;chosen=0;}
			};int cmp(const Rang&a,const Rang&b){return a.start>b.last?1:a.last<b.start?-1:0;}int cmp(const Rang&a,const uint_t&b){return a.start>b?1:a.last<b?-1:0;}
			List<Rang,uint16_t,16>ranges[sizeof(int*)*2]
			RegisterState::bits REG_UNAVAIL=(1<<stackRegister);
			Allocery::Linear<>alo;
			for(uint8_t r=0;r<sizeof(int*)*2;r++){RegisterState::bits R=1<<r;if(!(REG_UNAVAIL&R)){
				Allocery::AlloclessLinear<>_alo;
				uint_t i=0;Rang rang;rang.start=0;rang.inpC=1;rang.reg=r;auto rava=rang.available=~(R|REG_UNAVAIL);List<Rang,uint16_t,16>&rangs=ranges[r];El*las;
				singl:rang.inpC=1;bool un=0;for(;i<origin->ins.count;i++){
					auto in=origin->ins[i]->inputs(),ou=origin->ins[i]->outputs(),uns=origin->ins[i]->undesired();
					rang.available=rang.available&~(uns|in|ou);
					if(in&R){
						rang.last=i;un=0;
						if(ou&R){rang.inp=_alo.make(i,un,0);rangs+=rang;rang.available=rava&~(uns|in|ou);rang.start=i;un=0;continue;}
						las=rang.inp=_alo.make(i,un);i++;goto multi;
					}
					if(ou&R){rang.available=rava&~(uns|in|ou);rang.start=i;un=0;continue;}un|=uns&R;rang.useful|=un;
				}goto rebuild;
				multi:for(;i<origin->ins.count;i++){
					auto in=origin->ins[i]->inputs(),ou=origin->ins[i]->outputs(),uns=origin->ins[i]->undesired();
					rang.available=(rang.available)&~(uns|in|ou);
					if(in&R){las=las->next=_alo.make(i,un);rang.last=i;un=0;rang.inpC++;}
					if(ou&R){las->next=0;rangs+=rang;rang.start=i;rang.available=rava&~(uns|in|ou);i++;goto singl;}un|=uns&R;rang.useful|=un;
				}rebuild:
				for(i=0;i<rangs.count;i++){//can take this loop out and thread everything else in the "for" "r"
					El*cur=rangs[i].inp,dst;dst=rangs[i].inp=alo.alloc(sizeof(El)*rangs[i].inpC);
					while(cur){*dst=*cur;dst->next=0;dst++;cur=cur->next;}
				}
			}}*/
			struct Ine{uint_t at;bool recover;Rang*next;inline Ine(){}inline Ine(uint_t a,bool r){at=a;recover=r;}};
			RegisterState::bits REG_UNAVAIL=(1<<stackRegister),stat=0;
			List<Ine>ines;
			{
				Ine _ines[sizeof(int*)*2]={0},cur[sizeof(int*)*2];for(uint8_t r=0;r<sizeof(int*)*2;r++)cur[r]=&_ines[r];
				Allocery::AlloclessLinear<>alo;
				for(;i<origin->ins.count;i++){
					auto in=ins[i]->inputs(*this)&stat,un=ins[i]->undesired(*this)&~stat;//unfiltered ones aren't necessary
					if(in){for(uint8_t r=0;r<sizeof(int*)*2;r++)if(in&(1<<r))cur[r]=cur[r]->next=alo.make<Ine>(i,1);stat&=~in;}//recovers
					stat&=~ins[i]->outputs(*this);//stops saving if needed
					if(un){for(uint8_t r=0;r<sizeof(int*)*2;r++)if(un&(1<<r))cur[r]=cur[r]->next=alo.make<Ine>(i,0);stat|=un;}//saves
				}
				uint_t _i=-1;
				for(uint8_t r=0;r<sizeof(int*)*2;r++)if(_i>_ines[r].at)_i=_ines[r].at;
				for(uint8_t r=sizeof(int*)*2-1;r!=255;r--)if(_i==_ines[r].at){
					ines.add(_ines[r]);_ines[r]=*_ines[r].next;
				}
			}
			RegisterState::bits NoPreserve=REG_UNAVAIL;
			for(uint8_t r=0;r<sizeof(int*)*2;r++){if(cur[r]==&ines[r])NoPreserve|=1<<r;else cur[r]->next=0;}

			uint_t next=-1;RegisterState::bits claimed;//next register preservation index and 
			struct{uint16_t i,inp;Rang::El*cach;bool active;Location reloc;Location stack;}rStates[sizeof(int*)*2];//register preservation state
			for(uint8_t r=0;r<sizeof(int*)*2;r++)if(!(NoPreserve&(1<<r))){
				rStates[r].cach=&ines[r].inp[0];
				auto c=rStates[r].cach->start;if(next<c)c=next;//looks for the first register preservation that appears
			}
			//prepares all and activates the ones at that index
			for(uint8_t r=0;r<sizeof(int*)*2;r++)if(!(NoPreserve&(1<<r))){Instructions::StackAllo(&rStates[r].stack).compile(*this);rStates[r].i=0;rStates[r].active=rStates[r].cach->start==next;}
			for(i=0;i<origin->ins.count;i++){
				while(i>=origin->lns[line])line++;
				if(i==next){
					next=-1;//needed to search again
					//restores registers
					for(uint8_t r=0;r<sizeof(int*)*2;r++)if(!(NoPreserve&(1<<r)&ins[i]->inputs())&&rStates[r].active)Instructions::Set(r,rStates[r].reloc,sizeof(int*)).compile(*this);
					//saves registers
					for(uint8_t r=0;r<sizeof(int*)*2;r++){auto&sta=rStates[r];RegisterState::bits R=1<<r;if(!(NoPreserve&R)&&sta.active){
						if(rStates[r].cach.start==i){
							claimed&=~sta.reloc.regDireBit();
							auto _=highBitI(rang.available&~(REG_UNAVAIL|claimed));if(_){sta.reloc=_;claimed|=1<<_;}else sta.reloc=sta[r].stack;
							Instructions::Set(sta.reloc,r,sizeof(int*)).compile(*this)
						}
						if(sta.inp++==sta.cach->inpC){//responsible for going through preservations
							sta.inp=0;sta.i++;
							if(sta.i==ines[r].count){sta.active=0;continue;}//if no more then it disables
							sta.cach=&ines[r].inp[sta.i];//cache the pointer there of course
						}
						if(next<sta.cach->inp[sta.i]->start)sta.cach->inp[sta.i]->start=next;//searches for the next one
					}}
					//updates registers
					for(uint8_t r=0;r<sizeof(int*)*2;r++)if(!(NoPreserve&(1<<r))&&ines[r].count>sta.i){rStates[r].active=rStates[r].cach->start==next;}
				}
				origin->ins[i].compile(*this);
			}
		#endif
	}
	void CompileState::post(){
		for(uint16_t i=0;i<posts.count;)if(posts[i].second(*this,posts[i].a.a,posts[i].a.b))i++;else posts.remove(i);
		for(uint16_t i=0;i<backpawchy.count;i++){
			if(backpawchy[i].b.type==2){
				backpawchy[i].b=b->count-lns[backpawchy[i].b.line];
			}else if(backpawchy[i].b.type==3){
				backpawchy[i].b=b->count-labls[*backpawchy[i].b.labl];
			}else if(backpawchy[i].b.type==4){
				backpawchy[i].b=b->count-backpawchy[i].a-5;
			}
			if(backpawchy[i].b.type==0){
				*(int32_t*)(b->raw+backpawchy[i].a+1)=backpawchy[i].b.abs;
			}else if(backpawchy[i].b.type==1){

				backpawchy.remove(i--);
				printf("!!!Backpatching: unimplemented case!!!\n");throw std::exception();
			}else{printf("CompileState::post\n");throw std::exception();}
		}
	}
	// Instructions::Paste::~Paste(){r.death();}
	// bool Instructions::Paste::compile(CompileState&s){
	// 	throw std::exception();//It's not ready.
	// 	//r.compile(CompileState{s.e,s.origin,s.b});
	// 	return 0;
	// }
	void Instructionary::inlineInto(CompileState&c){
		CompileState co(env,this,c.b,inliny);
		if(!co.compile()){printf("Instructionary::inline\n");throw std::exception();}
		Instructions::StackUnRequire(co.stackClaimed).compile(co);
		//auto w=__b.raw;__b.raw=env->exeMem.alloc(1+__b.count);memcpy(__b.raw,__b.count,w);__b.size=0;free(w);
		co.post();
		co.posts.movFrom(co.posts);
		co.backpawchy.movFrom(co.backpawchy);
	}
	void Instructionary::compile(){
		auto ExeMem_count=ExeMem.count;
		if(_compiled){Error("UNIMPLEMENTED: Instructionary RECOMPILATION");return;}
		if(!lns.count||lns.last()!=ins.count)ln();
		BBuf __b;CompileState co(env,this,&__b,inliny);
		if(!co.compile()){printf("Instructionary::compile()=0\n");throw std::exception();}
		//if(inliny)__b.ensure(Instructions::retSize);//why did I write this again?
		uint32_t siz=__b.count+(inliny?Instructions::retSize+Instructions::stackUnclaimMaxSize:0);

		auto w=__b.raw;__b.raw=(uint8_t*)(env?env->exeMem.alloc(siz):ExeMem.alloc(siz));__b.size=siz;memcpy(__b.raw,w,__b.count);free(w);//replace the buffer with allocated executable memory
		co.post();//post-process s---

		if(inliny){co.inlining=0;Instructions::StackPURGE(co.stackClaimed).compile(co);Instructions::ret.compile(co);}//a proper "ret" just in case some dummy actually uses "call" on it... also handy C++-like inline
		if(!env)ExeMem.count-=siz-__b.count;else if(siz>__b.count)env->exeMem.shrink(__b.raw,__b.count);_compiled=__b.raw;//finale
		printf("Compiled(%u): ",unsigned(env?*(uint32_t*)(uintptr_t(_compiled)-4):ExeMem.count-ExeMem_count));for(uint32_t i=0;i<__b.count;i++)printf("%.2x",__b[i]);printf("\n");
		__b.raw=0;//neutralize BBuf cleanup or else it'll UB, likely to death
	}
	static Instructionary _EVALER(0);
	void Instructionary::eval(RegisterState::regT regs[sizeof(int*)<<1]){// https://devblogs.microsoft.com/oldnewthing/20080215-00/?p=23443 ...
		regs[5]=(RegisterState::regT)_compiled;
		((void(*)(RegisterState::regT*))_EVALER._compiled)(regs);
	}//Kinda I guess can be called dark magic... Meh, 'tis fine.
	namespace __COMPILE{
		INITIALIZER(init){
			_EVALER.ins+=&Instructions::transitTo;
			static Instructions::call _c({(uint8_t)5,sizeof(int*)*5});_c.constant=1;_EVALER.ins+=&_c;//this one "saved register" can't really be properly saved/restored anyway
			_EVALER.ins+=&Instructions::transitFrom;

			_EVALER.ln();BBuf b;CompileState co(0,&_EVALER,&b);//just all the s--- to have it compile
			if(!co.compile()){printf("__COMPILE::init\n");throw std::exception();}Instructions::ret.compile(co);
			//printf("Compiled(%u): ",unsigned(b.count));for(uint32_t i=0;i<b.count;i++)printf("%.2x",b.raw[i]);printf("\n");
			uint32_t siz=b.count;
			auto w=b.raw;_EVALER._compiled=b.raw=(uint8_t*)ExeMem.alloCpy(b.raw,siz);free(w);co.post();//basically a shorter version of 2 lines in Instructionary::compile
			ExeMem.count-=siz-b.count;//Just in case "post()" ends up shrinking it with some dark magic or s--- like that...

			_EVALER.inliny=1;//because this can be inlined.
			for(uint8_t i=0;i<(sizeof(int*)>>2);i++)_EVALER.mayChange&=~(1<<i);
			b.raw=0;//neutralize BBuf cleanup or else it'll UB, likely to death
		}
	}
	Instructionary&Instructionary::EVALER(){return _EVALER;}
}
