#pragma once
#define type_stuff___H
#include"../Compile.h"
#include"../Functions.h"
namespace DerKern{
	int32_t&envPointTo(string name,Type*,void*location);//basically makes a variable that's placed at "location"
	namespace Instructions{
		struct NopOt:NopO{Type*outType;inline NopOt(Location ou,Type*t):NopO(ou,t->size){outType=t;}inline NopOt(_Variable _):NopO(_.b,_.a->size){outType=_.a;}};//NopO except with a Type
		struct MovM:Noth{Type::Memba*m;Location in;Location out;inline MovM(Location dst,Type::Memba*M,Location inp){out=dst;in=inp;m=M;}bool compile(CompileState&)override;};//calls a move constructor or moves manually
		struct GetM:NopOt{Type::Memba*m;Location in;inline GetM(Location dst,Type::Memba*M,Location inp):NopOt(dst,M->t){in=inp;m=M;}bool compile(CompileState&)override;};//calls a move constructor or moves manually
		
		struct Constry:Noth{_Variable v;inline Constry(_Variable _):v(_){}bool compile(CompileState&)override;};//calls a default constructor if possible
		struct Assassinate:Noth{_Variable v;inline Assassinate(_Variable _):v(_){}bool compile(CompileState&)override;};//calls a destructor if possible
		struct Cpy:NopOt{Location orig;inline Cpy(_Variable dst,Location src):NopOt(dst){orig=src;}bool compile(CompileState&)override;};//calls a copy constructor if possible
		struct MovT:NopOt{Location in;inline MovT(_Variable dst,Location inp):NopOt(dst){in=inp;}bool compile(CompileState&)override;};//calls a move constructor or moves manually
		struct SetT:NopOt{_Variable f;inline SetT(_Variable dst,_Variable inp):NopOt(dst),f(inp){}bool compile(CompileState&)override;};//calls a move constructor or moves manually
		//NopOt*GetM(Location v,Type::Memba*,Instructionary*i=0);
		//inline NopOt*GetM(Location dst,Location v,Type::Memba*m,Instructionary*i=0){auto _=GetM(v,m,i);if(_->out==dst)return _;return i?i->allo<MovT>(_Variable{_->outType,dst},_->out):new MovT(_Variable{_->outType,dst},_->out);}
		NopOt*SetM(Location obj,Type::Memba*,_Variable v,Instructionary*i=0);
		NopOt*SetM(Location obj,Type::Memba*,Location v,Instructionary*i=0);
	}
}