#pragma once
#include"../include/DerKern/parse.h"
#include"Environment.cpp"
namespace DerKern{
	void Parse(ParseResult*r,uint64_t*sCurrent,string s){
		uint64_t _i;if(!sCurrent){_i=0;sCurrent=&_i;}
		while(s.size()>*sCurrent){
			if(!Parse1(sCurrent,s,r)){
				printf("| !!!Can't continue parsing at ```\n%s\n```\n",s.c_str()+*sCurrent);
				throw std::exception();
			}
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
	bool Instructions::Paste::compile(CompileState&s){
		throw std::exception();//It's not ready.
		//r.compile(CompileState{s.e,s.origin,s.b});
		return 0;
	}
	Instructions::Paste::~Paste(){r.death();}
}