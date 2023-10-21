#pragma once
#include"../include/DerKern/eval.h"
#include"parse.cpp"
namespace DerKern{
	void ParseResult::eval(Environment*e){
		EvalState s;s.origin=this;s.e=e;s.line=0;eval(&s);s.post();
	}
	void ParseResult::eval(EvalState*e){
		for(e->i=0;e->i<ins.count;e->i++){
			while(e->i>=lns[e->line])e->line++;
			auto I=ins[e->i];
			/*switch(I->type){
				case 0:
				break;case 1:
				break;default:
			}*/
			I->eval(*e);
		}
	}
	void EvalState::post(){
		for(uint16_t i=0;i<posts.count;i++)posts[i].second(posts[i].first);
	}
	void Eval(uint64_t*sCurrent,string s,Environment*e){ParseResult p=Parse(sCurrent,s);p.eval(e);p.death();}
}