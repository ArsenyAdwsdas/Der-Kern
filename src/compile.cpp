#include"../include/DerKern/DerKern.h"
namespace DerKern{
	void DerKern::ParseResult::compile(BBuf*b,Environment*e){
		BBuf _b;if(!b)b=&_b;
		CompileState c;c.origin=this;c.b=b;c.e=e;c.line=0;
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