#pragma once
#include"../include/DerKern/parse.h"
#include"compile.cpp"
#include"Encods/Ascii.cpp"
namespace DerKern{
	void Parse(ParseResult*r,Inputo::Universa*I){
		while(I->ensure(1)){
			if(!Parse1(I,r)){
				printf("| !!!Can't continue parsing at ```\n%llu(%c/%d)\n```\n",(uint64_t)I->getI(),I->peekC(),int(I->peekC()));
				throw std::exception();
			}
		}
		if(!r->target->inliny&&(!r->target->ins.count||r->target->ins.last()!=&Instructions::ret))r->target->ins+=&Instructions::ret;
		r->target->ln();
	}
	ParseResult Parse(Inputo::Universa*I){
		ParseResult r;
		Parse(&r,I);
		return r;
	}
	/*void ParseResult::death(){
		for(uint16_t i=0;i<ins.count;i++){
			auto z=ins[i];
			if(!z->constant)delete z;
		}
		free(ins.raw);ins.size=ins.count=0;
		lns.death();
	}*/
}