#pragma once
#include"parse.h"
namespace DerKern{
	struct CompileState;
	struct EvalState{
		inline EvalState(){}
		inline EvalState(Environment*env,ParseResult*orig=0){e=env;origin=orig;}
		EvalState(const CompileState&)=delete;
		ParseResult*origin;Environment*e;
		uint16_t i,line;//the current instruction and the current line
		List<pair<void*,void(*)(void*)>>posts;//pairs of A,F to do F(A) after everything else

		void post();
		inline ~EvalState(){posts.death();}
	};
	void Eval(uint64_t*sCurrent,string s,Environment*);
}