#pragma once
#include"../include/DerKern/platformie/x86.h"
#include"platformie/x86.cpp"
DerKern::Instruction::~Instruction(){}
namespace DerKern::Instructions{
	Instruction ret0{1};
	Pong pong{1};
	Noth noth{1};
	TransitionTo transitTo{1};
	TransitionFrom transitFrom{1};

	Return1::~Return1(){bef->~Return0();free(bef);}

	bool Noth::compile(CompileState&s){return 1;}
}