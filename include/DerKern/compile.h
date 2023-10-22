#pragma once
#include"parse.h"
namespace DerKern{
	struct Environment;struct ParseResult;
	typedef void(__stdcall*CompileResult)(RegisterState::regT[sizeof(int*)<<1]);
	struct CompileState{
		bool usedRegisters[sizeof(int*)<<1];
		inline CompileState(){for(uint8_t i=0;i<4;i++)usedRegisters[i]=0;for(uint8_t i=4;i<6;i++)usedRegisters[i]=1;for(uint8_t i=6;i<(sizeof(int*)<<1);i++)usedRegisters[i]=0;}
		ParseResult*origin;Environment*e;
		inline CompileState(Environment*env,ParseResult*orig):CompileState(){e=env;origin=orig;}
		uint16_t i,line;//the current instruction and the current line
		List<pair<void*,void(*)(void*)>>posts;
		BBuf*b;
		List<uint64_t>lns;//indecies of the first bytes each(except the last one) line doesn't take, do fix 'em all if manually editing "BBuf"
		inline void ln(uint64_t i){lns.append(i);}
		inline bool UnusedRegister(uint8_t*r){for(uint8_t i=0;i<(sizeof(int*)<<1);i++)if(!usedRegisters[i]){*r=i;return 1;}return 0;}
		void post();
		inline~CompileState(){posts.death();lns.death();}
		CompileResult toExec();
	};
}