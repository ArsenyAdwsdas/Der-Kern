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
	/*void Add::eval(EvalState&s){
		if(
			(
				in.a!=&Type::u8&&in.a!=&Type::i8&&in.a!=&Type::u16&&in.a!=&Type::i16
				&&in.a!=&Type::u32&&in.a!=&Type::i32&&in.a!=&Type::u64&&in.a!=&Type::i64
				&&in.a!=&Type::f&&in.a!=&Type::d
			)||out.b.resolve().type==0||in.b.resolve().type==0||in2.b.resolve().type==0)
			throw std::exception();

		if(in.a==&Type::u8||in.a==&Type::i8){
			auto&a=*(uint8_t*)in.b.resolve().eval();
			auto&b=*(uint8_t*)in2.b.resolve().eval();
			uint8_t c=a+b;
			*(uint8_t*)out.b.resolve().eval()=c;
			uint8_t&f=s.e->flags;
			f=(f&~23)|bool(a>c||b>c)|((c^(a&0x80))?2:0)|((c&0x80)?4:0)|(c?0:16);
		}else if(in.a==&Type::u16||in.a==&Type::i16){
			auto&a=*(uint16_t*)in.b.resolve().eval();
			auto&b=*(uint16_t*)in2.b.resolve().eval();
			uint16_t c=a+b;
			*(uint8_t*)out.b.resolve().eval()=c;
			uint8_t&f=s.e->flags;
			f=(f&~23)|bool(a>c||b>c)|((c^(a&0x8000))?2:0)|((c&0x8000)?4:0)|(c?0:16);
		}else if(in.a==&Type::u32||in.a==&Type::i32){
			auto&a=*(uint32_t*)in.b.resolve().eval();
			auto&b=*(uint32_t*)in2.b.resolve().eval();
			uint32_t c=a+b;
			*(uint8_t*)out.b.resolve().eval()=c;
			uint8_t&f=s.e->flags;
			f=(f&~23)|bool(a>c||b>c)|((c^(a&0x8000))?2:0)|((c&0x8000)?4:0)|(c?0:16);
		}else if(in.a==&Type::u64||in.a==&Type::i64){
			auto&a=*(uint64_t*)in.b.resolve().eval();
			auto&b=*(uint64_t*)in2.b.resolve().eval();
			uint64_t c=a+b;
			*(uint8_t*)out.b.resolve().eval()=c;
			uint8_t&f=s.e->flags;
			f=(f&~23)|bool(a>c||b>c)|((c^(a&0x8000))?2:0)|((c&0x8000)?4:0)|(c?0:16);
		}
	}*/
}