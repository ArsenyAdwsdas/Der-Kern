#pragma once
#include"Type.h"
//#include"compile.h"
namespace DerKern{
	struct CompileState;struct Function;
	struct Instruction{
		bool constant=0;//if 1 then don't even think of destroying it.
		inline Instruction(int){constant=1;}
		inline Instruction(){}
		//static constexpr Return0*Error=(Return0*)(uint64_t)-1;	//Я БЫ С РАДОСТЬЮ, НО НЕ ПОЗВОЛЯЮТ.
		virtual bool compile(CompileState&);
		virtual~Instruction();
	};
	namespace Instructions{
		// VERY unsure of whether it's needed...
		// ENUM(IDs,uint16_t)
		// 	MoreComplicated=0x8000//before that is x86
		// 	Re
		// 	ENUM_END
		typedef Instruction Return0;
		extern Return0 ret;
		struct Return1:Return0{
			Return0*bef;
			inline Return1(Return0*b){bef=b;}
			bool compile(CompileState&)override;
			~Return1()override;
		};
		inline Return0*Return(Return0*b){if(b)return new Return1(b);else return&ret;}
		
		struct jmpLine:Return0{
			int16_t lineOffset;
			inline jmpLine(int16_t o){lineOffset=o;}
			bool compile(CompileState&)override;
		};
		struct Pong:Return0{
			inline Pong(int){constant=1;}
			bool compile(CompileState&)override;
		};extern Pong pong;
		struct Paste;
		struct SpawnVar:Return0{
			Variable v;
			inline SpawnVar(string n,Location v,Type*t):v(n,{t,v}){}
			bool compile(CompileState&)override;
		};
		struct call:Return0{Location loc;inline call(Location l){loc=l;}bool compile(CompileState&)override;};
		struct CallArged:Return0{
			Location ret,*argv;uint8_t argc;
			Function*func;
			inline CallArged(Function*f,Location r,Location*av,uint8_t ac){func=f;ret=r;argv=av;argc=ac;}
			bool compile(CompileState&)override;
		};
		
		struct Noth:Return0{inline Noth(){}inline Noth(int){constant=1;}bool compile(CompileState&)override;};extern Noth noth;//basically does nothing.

		struct NopO:Noth{pair<Type*,Location>out;inline NopO(pair<Type*,Location>ou):out(ou){}};//a base instruction that "outputs" a value
		struct NopIO:NopO{pair<Type*,Location>in;inline NopIO(pair<Type*,Location>inp,pair<Type*,Location>out):NopO(out),in(inp){}};//a base instruction that "inputs" and "outputs" a value
		struct NopIIO:NopIO{pair<Type*,Location>in2;inline NopIIO(pair<Type*,Location>inp,pair<Type*,Location>inp2,pair<Type*,Location>out):NopIO(inp,out),in2(inp2){}};//a base instruction that "inputs" twice and "outputs" a value
		struct Init:NopO{pair<Type*,Location>v;bool compile(CompileState&)override;};//calls default constructor

		struct Set8:Noth{Location to;uint8_t val;inline Set8(Location t,uint8_t v){to=t;val=v;}bool compile(CompileState&)override;};
		struct Set16:Noth{Location to;uint16_t val;inline Set16(Location t,uint16_t v){to=t;val=v;}bool compile(CompileState&)override;};
		struct Set32:Noth{Location to;uint32_t val;inline Set32(Location t,uint32_t v){to=t;val=v;}bool compile(CompileState&)override;};
		struct Set64:Noth{Location to;uint64_t val;inline Set64(Location t,uint64_t v){to=t;val=v;}bool compile(CompileState&)override;};
		struct Set:Noth{Location to,from;uint8_t size;inline Set(Location t,Location f,uint8_t s){to=t;from=f;size=s;}bool compile(CompileState&)override;};
		struct Swap:Noth{Location to,from;uint8_t size;inline Swap(Location t,Location f,uint8_t s){to=t;from=f;size=s;}bool compile(CompileState&)override;};

		struct Add8_1:Noth{Location to;uint8_t val;inline Add8_1(Location t,uint8_t v){to=t;val=v;}bool compile(CompileState&)override;};
		struct Add16_1:Noth{Location to;uint16_t val;inline Add16_1(Location t,uint16_t v){to=t;val=v;}bool compile(CompileState&)override;};
		struct Add32_1:Noth{Location to;int32_t val;inline Add32_1(Location t,uint32_t v){to=t;val=v;}bool compile(CompileState&)override;};
		struct Add_1:Noth{Location to,from;uint8_t size;inline Add_1(Location t,Location f,uint8_t s){to=t;from=f;size=s;}bool compile(CompileState&)override;};

		
		struct TransitionTo:Return0{//ebp doesn't matter and it's not meant to be used as is
			inline TransitionTo(){}inline TransitionTo(int){constant=1;}
			bool compile(CompileState&)override;
		};extern TransitionTo transitTo;
		struct TransitionFrom:Return0{
			inline TransitionFrom(){}inline TransitionFrom(int){constant=1;}
			bool compile(CompileState&)override;
		};extern TransitionFrom transitFrom;
		bool _set(CompileState&,Location,RegisterState::regT v);
		bool _call(CompileState&,Location);
	}
	typedef Instructions::NopO InstructionWithO;
	typedef Instructions::NopIO InstructionWithIO;
}
#if defined(__x86_64__) || defined(_M_X64) || defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)//https://stackoverflow.com/a/66249936
	#include"platformie/x86.h"
#endif