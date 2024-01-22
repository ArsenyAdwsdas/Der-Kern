#pragma once
#include"../../include/DerKern/extra/type_stuff.h"
#include"../Type.cpp"
#include"../Compile.cpp"
#include"../Functions.cpp"
namespace DerKern{
	namespace Instructions{
		bool MovM::compile(CompileState&c){
			assert(m->type==1);
			auto f=m->t->memba("=");
			if(!f)return memecpy(out.resolve()+m->off,in,m->t->size).compile(c);
			throw std::exception();
		}
		bool GetM::compile(CompileState&c){
			assert(m->type==1);
			auto f=m->t->memba("=");
			if(!f)return memecpy(out.resolve(),in+m->off,m->t->size).compile(c);
			throw std::exception();
		}

//Maybe broken

		bool Constry::compile(CompileState&c){
			auto f=v.a->memba("$this");if(!f)return 1;
			Type*t=v.a->pointer();
			assert(f->type==3&&f->sta.a==FuncType::Get(&Type::VOID,&t,1));
			Instructions::NopO _={v.b,sizeof(int*)},*__=&_;
			if(!((Function*)f->sta.b)->_call(c,{},&__,1))return 0;
			return 1;
		}
		bool Assassinate::compile(CompileState&c){
			auto f=v.a->memba("~this~");if(!f)return 1;
			Type*t=v.a->pointer();
			assert(f->type==3&&f->sta.a==FuncType::Get(&Type::VOID,&t,1));
			Instructions::NopO _={v.b,sizeof(int*)},*__=&_;
			if(!((Function*)f->sta.b)->_call(c,{},&__,1))return 0;
			return 1;
		}
		bool Cpy::compile(CompileState&c){
			auto f=outType->memba("+this+");if(!f)return memecpy(out,orig,outSize).compile(c);
			Type*t=outType->pointer();
			assert(f->type==3&&f->sta.a==FuncType::Get(&Type::VOID,&t,1));
			Instructions::NopO _[]={{out,sizeof(int*)},{orig,sizeof(int*)}};
			Instructions::NopO*__[]={_,_+1};
			if(!((Function*)f->sta.b)->_call(c,Location(),__,2))return 0;
			return 1;
		}

//Likely broken

		bool MovT::compile(CompileState&c){
			auto f=outType->memba("=");
			printf(".\n");
			if(!f)return memecpy(out,in,outSize).compile(c);
			printf("?\n");
			Type*t[]={outType->pointer(),outType};
			assert(f->type==3&&f->sta.a==FuncType::Get(&Type::VOID,t,2));
			Instructions::NopO _[]={{out,sizeof(int*)},{in,sizeof(int*)}};
			Instructions::NopO*__[]={_,_+1};
			if(!((Function*)f->sta.b)->_call(c,Location(),__,2))return 0;
			return 1;
		}
		/*NopOt*GetM(Location v,Type::Memba*m,Instructionary*i){
			if(!m)return 0;
			if(m->type==1){assert(v.isDReg());return i?i->allo<NopOt>(v+m->off,m->t):new NopOt(v+m->off,m->t);}
			assert(m->type!=2);
			if(m->type==3)return i?i->allo<NopOt>(m->sta.b,m->sta.a):new NopOt(m->sta.b,m->sta.a);
			printf("!!! GetM got a weird Memba\n");
			throw std::exception();
		}*/
		/*NopOt*SetM(Location o,Memba*m,_Variable v,Instructionary*i=0){
			if(!m)return 0;
			if(m->type==1&&v.type.isDReg())return i?i->allo<SetT>({m->t,o+m->off},v):new SetT({m->t,o+m->off},v);
			assert(m->type!=2);
			if(m->type==3)return i?i->allo<SetT>(m->sta,v):new SetT(m->sta,v);
			return 0;
		}*/
		NopOt*SetM(Location o,Type::Memba*m,Location v,Instructionary*i){
			if(!m)return 0;
			if(m->type==1){assert(o.isDReg()||o.type==5);return i?i->allo<MovT>(_Variable{m->t,o+m->off},v):new MovT({m->t,o+m->off},v);}
			assert(m->type!=2);
			if(m->type==3)return i?i->allo<MovT>(m->sta,v):new MovT(m->sta,v);
			return 0;
		}
	}
}