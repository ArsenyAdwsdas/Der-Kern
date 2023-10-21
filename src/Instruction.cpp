#pragma once
#include"../include/DerKern/platformie/x86.h"
#include"platformie/x86.cpp"
DerKern::Instruction::~Instruction(){}void DerKern::Instruction::eval(EvalState&s){s.i=s.origin->ins.count;}
namespace DerKern::Instructions{
	Instruction ret0{1};
	Pong pong{1};
	Return1::~Return1(){bef->~Return0();free(bef);}
	void Return1::eval(EvalState&s){s.i=s.origin->ins.count;}


	void Pong::eval(EvalState&s){printf("pong at %hu\n",s.line+1);}
	void jmpLine::eval(EvalState&s){s.line+=lineOffset;s.i=s.origin->lns[s.line-1]-1;}
	void NopO::compile(CompileState&s){}void NopO::eval(EvalState&s){}
}