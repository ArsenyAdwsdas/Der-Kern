#pragma once
#include"Instruction.h"
#include"Environment.h"
namespace DerKern{
	struct CompileState;
	struct ParseResult{//AKA Scope?
		List<Instruction*>ins;inline void operator+=(Instruction*i){ins.append(i);}
		List<uint16_t>lns;inline void ln(){lns.append(ins.count);}
		Scope vars;
		Allocery::Linear extra;
		inline ParseResult(){}
		
		void DoubleQuote_eval_DoubleQuote(Environment*);//it's basically "compile, run what you just compiled, throw away what you just compiled." because I ended up erasing "eval".
		inline void eval(Environment*e){DoubleQuote_eval_DoubleQuote(e);}
		void compile(CompileState&);
		void death();
	};
	namespace Instructions{struct Paste:Instruction{
		ParseResult r;
		inline Paste(ParseResult z){/*type=2;*/r=z;}
		bool compile(CompileState&)override;
		~Paste()override;
	};}
	void Parse(ParseResult*r,uint64_t*sCurrent,string s);
	ParseResult Parse(uint64_t*sCurrent,string s);
	bool Parse1(uint64_t*sCurrent,string s,ParseResult*);//return 0 on error, define it yourself.
}