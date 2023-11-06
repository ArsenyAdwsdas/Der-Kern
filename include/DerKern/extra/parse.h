#pragma once//This file's quite useless.
#include"../Compile.h"
#include"../Encods/Ascii.h"
namespace DerKern{
	struct CompileState;
	struct ParseResult{//AKA CompilationScope?
		Scope vars;
		Allocery::Linear extra;
		Instructionary*target;
		inline ParseResult(){target=0;}

		/*List<Instruction*>ins;inline void operator+=(Instruction*i){ins.append(i);}
		List<uint16_t>lns;inline void ln(){lns.append(ins.count);}
		inline ParseResult(){}
		
		void DoubleQuote_eval_DoubleQuote(Environment*);//it's basically "compile, run what you just compiled, throw away what you just compiled." because I ended up erasing "eval".
		inline void eval(Environment*e){DoubleQuote_eval_DoubleQuote(e);}
		void compile(CompileState&);
		void death();*/
	};
	//in parse1: return 0 on error
	template<bool(*parse1)(Inputo::Universa*,ParseResult*)>inline void Parse(ParseResult*r,Inputo::Universa*I){
		while(I->ensure(1)){
			if(!parse1(I,r)){
				printf("| !!!Can't continue parsing at ```\n%llu(%c/%d)\n```\n",(uint64_t)I->getI(),I->peekC(),int(I->peekC()));
				throw std::exception();
			}
		}
		if(!r->target->inliny&&(!r->target->ins.count||r->target->ins.last()!=&Instructions::ret))r->target->ins+=&Instructions::ret;//just to make sure it returns instead of dying
	}
	/*template<bool(*parse1)(Inputo::Universa*,ParseResult*)>inline ParseResult Parse(Inputo::Universa*I){
		ParseResult r;
		Parse<parse1>(&r,I);
		return r;
	}*/
}
