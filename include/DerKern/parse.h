#pragma once
#include"compile.h"
#include"Encods/Ascii.h"
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
	void Parse(ParseResult*r,Inputo::Universa*);
	ParseResult Parse(Inputo::Universa*);
	bool Parse1(Inputo::Universa*,ParseResult*);//return 0 on error, define it yourself.
}