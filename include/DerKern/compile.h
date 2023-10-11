#pragma once
#include"eval.h"
namespace DerKern{
	struct CompileState:EvalState{
		BBuf*b;
		List<uint64_t>lns;//indecies of the first bytes each(except the last one) line doesn't take, do fix 'em all if manually editing "BBuf"
		inline void ln(uint64_t i){lns.append(i);}

		void post();
		inline~CompileState(){lns.death();}
		operator EvalState()=delete;
	};
}