#pragma once
#include"Instruction.h"
#include"Environment.h"
namespace DerKern{
	struct ParseResult{//AKA Scope?
		List<Instruction*>ins;inline void operator+=(Instruction*i){ins.append(i);}
		List<uint16_t>lns;inline void ln(){lns.append(ins.count);}
		Dicto<string,pair<Type*,Location>>vars;
		Allocery::Linear extra;
		inline ParseResult(){}
		
		void eval(Environment*);
		void compile(BBuf*,Environment*);
		void eval(EvalState*);
		void death();
	};
	struct Instructions::Paste:Instruction{
		ParseResult r;
		inline Paste(ParseResult z){/*type=2;*/r=z;}
		void compile(CompileState&)override;
		void eval(EvalState&)override;
		~Paste()override;
	};
	void Parse(ParseResult*r,uint64_t*sCurrent,string s);
	ParseResult Parse(uint64_t*sCurrent,string s);
	bool Parse1(uint64_t*sCurrent,string s,ParseResult*);//return 0 on error, define it yourself.
}