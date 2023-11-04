#pragma once
#include"../include/DerKern/Instruction.h"
#if defined(__x86_64__) || defined(_M_X64) || defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
	#include"../include/DerKern/platformie/x86.h"
	#include"platformie/x86.cpp"
#endif
DerKern::Instruction::~Instruction(){}
namespace DerKern::Instructions{
	Instruction ret{1};
	Pong pong{1};
	Noth noth{1};
	TransitionTo transitTo{1};
	TransitionFrom transitFrom{1};

	Return1::~Return1(){bef->~Return0();free(bef);}

	bool Noth::compile(CompileState&s){return 1;}
}