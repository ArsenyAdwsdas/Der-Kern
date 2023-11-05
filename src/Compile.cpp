#pragma once
#include"../include/DerKern/Compile.h"
#include"Environment.cpp"
#include"Instruction.cpp"
#include"StandloExeMem.cpp"
namespace DerKern{
	bool CompileState::compile(){
		// for(uint16_t i=0;i<lns[0];i++)ins[i]->compile(c);
		// c.ln(b->count);c.line++;
		// while(lns.count>c.line){
		// 	for(uint16_t i=lns[c.line-1];i<lns[c.line];i++)ins[i]->compile(c);
		// 	c.ln(b->count);c.line++;
		// }
		for(;i<origin->ins.count;i++){
			while(i>=origin->lns[line])line++;
			auto I=origin->ins[i];
			if(!I->compile(*this))return 0;
		}
		return 1;
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
			}else throw std::exception();
		}
	}
	// Instructions::Paste::~Paste(){r.death();}
	// bool Instructions::Paste::compile(CompileState&s){
	// 	throw std::exception();//It's not ready.
	// 	//r.compile(CompileState{s.e,s.origin,s.b});
	// 	return 0;
	// }
	void Instructionary::inlineInto(BBuf*b,CompileState&c){
		if(_compiled)env->exeMem.free(_compiled);
		CompileState co(env,this,inliny);co.b=b;co.i=0;co.line=0;
		if(co.compile())throw std::exception();
		//auto w=__b.raw;__b.raw=env->exeMem.alloc(1+__b.count);memcpy(__b.raw,__b.count,w);__b.size=0;free(w);
		co.post();
		c.posts.cpyFrom(co.posts);
		c.backpawchy.cpyFrom(co.backpawchy);
	}
	void Instructionary::compile(){
		if(_compiled)env->exeMem.free(_compiled);
		CompileState co(env,this,inliny);BBuf __b;co.b=&__b;co.i=0;co.line=0;
		if(!co.compile())throw std::exception();
		if(inliny)__b.ensure(1);
		uint32_t siz=__b.count;
		auto w=__b.raw;__b.raw=(uint8_t*)env->exeMem.alloc(inliny+siz);memcpy(__b.raw,w,siz);__b.size=0;free(w);co.post();//finishing touches: replace buffer with allocated executable memory, disable BBuf cleanup(size=0), post-process s---("post()")
		if(inliny)__b.raw[__b.count]=0xc3;//a proper "ret" just in case some dummy actually uses "call" on it... also handy C++-like inline
		if(siz+inliny>__b.count)env->exeMem.shrink(__b.raw,__b.count+inliny);
		_compiled=__b.raw;
		printf("Compiled: ");for(uint32_t i=0;i<__b.count;i++)printf("%.2x",__b[i]);printf("\n");
	}
	static Instructionary _EVALER(0);
	void Instructionary::eval(RegisterState::regT regs[sizeof(int*)<<1]){regs[5]=(RegisterState::regT)_compiled;((void(__stdcall*)(RegisterState::regT*))_EVALER._compiled)(regs);}//Kinda I guess can be called dark magic... Meh, 'tis fine.
	namespace __COMPILE{
		static void finalize(void){_EVALER._compiled=0;}
		INITIALIZER(init){
			atexit(finalize);
			_EVALER.ins+=&Instructions::transitTo;
			static Instructions::call _c({(uint8_t)5,sizeof(int*)*5});_c.constant=1;_EVALER.ins+=&_c;//this one "saved register" can't really be properly saved/restored anyway
			_EVALER.ins+=&Instructions::transitFrom;
			_EVALER.ins+=&Instructions::ret;

			_EVALER.ln();

			CompileState co(0,&_EVALER,_EVALER.inliny);BBuf b;co.b=&b;co.i=0;co.line=0;
			if(!co.compile())throw std::exception();
			uint32_t siz=b.count;
			auto w=b.raw;_EVALER._compiled=b.raw=(uint8_t*)ExeMem.alloCpy(b.raw,_EVALER.inliny+siz);b.size=0;free(w);co.post();//basically a shorter version of a line in Instructionary::compile for the "Environment"-less environment of "global construction"
			ExeMem.count-=siz-b.count;//Just in case "post()" ends up shrinking it with some dark magic or s--- like that...

			_EVALER.inliny=1;
			for(uint8_t i=0;i<(sizeof(int*)>>2);i++)_EVALER.preserves.raw[i]=-1;
		}
	}
	Instructionary&Instructionary::EVALER(){return _EVALER;}
}
