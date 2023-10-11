#include"../include/DerKern/DerKern.h"
namespace DerKern{
	void Parse(ParseResult*r,uint64_t*sCurrent,string s){
		uint64_t _i;if(!sCurrent){_i=0;sCurrent=&_i;}
		while(s.size()>*sCurrent){
			Instruction*I=Parse1(sCurrent,s,r);if(I==0)continue;
			if(I==(Instruction*)(uint64_t)-1){
				printf("| !!!Can't continue parsing at ```\n%s\n```\n",s.c_str()+*sCurrent);
				throw std::exception();
			}
			*r+=I;
		}r->ln();
	}
	ParseResult Parse(uint64_t*i,string s){
		uint64_t _i;if(!i){_i=0;i=&_i;}
		ParseResult r;
		Parse(&r,i,s);
		return r;
	}
	void ParseResult::death(){
		for(uint16_t i=0;i<ins.count;i++){
			auto z=ins[i];
			if(!z->constant)delete z;
		}
		free(ins.raw);ins.size=ins.count=0;
		lns.death();
	}
	void Instructions::Paste::compile(CompileState&s){r.compile(s.b,s.e);}
	void Instructions::Paste::eval(EvalState&s){r.eval(s.e);}
	Instructions::Paste::~Paste(){r.death();}
}