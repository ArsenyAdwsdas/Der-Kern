#include"../include/DerKern/platformie/x86.h"
namespace DerKern::Instructions{
	Return0 ret0{1};
	Pong pong{1};
	Return0::~Return0(){}
	void Return0::eval(EvalState&s){s.i=s.origin->ins.count;}
	Return1::~Return1(){bef->~Return0();free(bef);}
	void Return1::eval(EvalState&s){s.i=s.origin->ins.count;}


	void Pong::eval(EvalState&s){printf("pong at %hu\n",s.line+1);}
	void jmpLine::eval(EvalState&s){s.line+=lineOffset;s.i=s.origin->lns[s.line-1]-1;}
}