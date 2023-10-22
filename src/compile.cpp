#pragma once
#include"../include/DerKern/compile.h"
#include"parse.cpp"
namespace DerKern{
	void DerKern::ParseResult::compile(CompileState&st){
		BBuf _b;if(!st.b)st.b=&_b;
		CompileState c;c.origin=this;c.b=st.b;c.e=st.e;c.line=0;
		/*for(uint16_t i=0;i<lns[0];i++)ins[i]->compile(c);
		c.ln(b->count);c.line++;
		while(lns.count>c.line){
			for(uint16_t i=lns[c.line-1];i<lns[c.line];i++)ins[i]->compile(c);
			c.ln(b->count);c.line++;
		}*/
		for(c.i=0;c.i<ins.count;c.i++){
			while(c.i>=lns[c.line])c.line++;
			auto I=ins[c.i];
			I->compile(c);
		}
		c.post();
	}
	void DerKern::CompileState::post(){
		for(uint16_t i=0;i<posts.count;i++)posts[i].second(posts[i].first);
	}
}
#include<windows.h>
DerKern::CompileResult DerKern::CompileState::toExec(){
	DWORD dummy;
	auto _=VirtualAlloc(0, b->size, MEM_COMMIT, PAGE_READWRITE);
	memcpy(_,b->raw,b->size);
	if(!VirtualProtect(_, b->size, PAGE_EXECUTE_READWRITE, &dummy))printf("FAIL\n");
	return (CompileResult)_;
}