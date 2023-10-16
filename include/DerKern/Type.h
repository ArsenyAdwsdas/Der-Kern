#pragma once
#include<string.h>
#include"Allocery.h"
namespace DerKern{
	struct Type;int cmp(Type*const&,Type*const&);int cmp(Type*const&a,const string&b);
	struct FuncType;struct Ptr1Type;struct Ptr2Type;struct Ptr3Type;struct ArrayType;
	struct Type{
		bool ptr1Valid,ptr2Valid,ptr3Valid;//whether *, &, && are allowed
		string name;uint32_t size;
		inline Type(string n,uint32_t s){name=n;size=s;ptr1Valid=1;ptr2Valid=ptr3Valid=s;}
		inline Type():Type("???",0){}
		Dicto<string,_Variable,uint16_t,8,&cmp>vars;
		enum class UOps:uint8_t{
			Equ=0,//=
			Negate,Plusate,//-,+
			Not,BNot,//!,~
			Star,//*
			Dolla,Hash,//$,#
			Count
		};
		enum class Ops:uint8_t{
			Set=0,//=
			Plus,Minus,Mul,Div,//+,-,*,/
			PlusE,MinusE,MulE,DivE,//+=,-=,*=,/=

			BOr,BAnd,BXor,Mod,//|,&,^,%
			BOrE,BAndE,BXorE,ModE,//|=,&=,^=,%=

			Left,BLeft,BRight,Right,//<,<<,>>,>
			LeftE,BLeftE,BRightE,RightE,//<=,<<=,>>=,>=

			Or,And,Eq,NEq,//||,&&,==,!=
			Count
		};
		#ifdef FASTER
			FuncHead _uops[UOps::Count];
			FuncHead _ops[Ops::Count];
		#endif
		static Type i8;static Type u8;
		static Type i16;static Type u16;
		static Type i32;static Type u32;
		static Type i64;static Type u64;
		static Type f;static Type d;//float/double
		static Type str;static Type _type;
		static Type idk[16];//idk*(this pointer's type ain't resolvable until runtime)[register with type id]

		constexpr static uint8_t PtrSize=sizeof(int*);

		static Type VOID;
		//"*", "&", "&&", *FuncHead		are basically unused except as operator and variable data(like names and where to find) containers.
		static Ptr1Type*ptr1_container;
		static Ptr2Type*ptr2_container;
		static Ptr3Type*ptr3_container;
		static Type func_container;

		Ptr1Type*pointer();Ptr2Type*pointer2();Ptr3Type*pointer3();//*, &, &&
		ArrayType*array(uint32_t count);
		FuncType*func(Type**argv,uint8_t argc);
		private:Ptr1Type*_pointer;Ptr2Type*_pointer2;Ptr3Type*_pointer3;SortList<Type*,uint16_t,16,&cmp>_arrays;SortList<Type*,uint16_t,16,&cmp>_funcs;
	};
	struct FuncType:Type{
		Type*rets;Type**argv;uint8_t argc;
		inline static string NAME(Type*rets,Type**argv,uint8_t argc){
			string _;BBuf b;
			b.append((uint8_t*)rets->name.c_str(),rets->name.size());
			b+='(';if(argc)b.append((uint8_t*)argv[0]->name.c_str(),argv[0]->name.size());for(uint8_t i=1;i<argc;i++){b+=',';b.append((uint8_t*)argv[i]->name.c_str(),argv[i]->name.size());}b+=')';
			return string((char*)b.raw,b.count);
		}
		inline FuncType(string n,Type*_rets,Type**_argv,uint8_t _argc):Type(n,0){rets=_rets;argv=mallocCpy(_argv,_argc);argc=_argc;}
		inline FuncType(Type*_rets,Type**_argv,uint8_t _argc):FuncType(NAME(_rets,_argv,_argc),_rets,_argv,_argc){}
	};
	struct Ptr1Type:Type{Type*orig;
		inline static string NAME(Type*t){string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());b+='*';return string((char*)b.raw,b.count);}
		inline Ptr1Type(string n,Type*t):Type(n,Type::PtrSize){orig=t;}inline Ptr1Type(Type*t):Ptr1Type(NAME(t),t){}
	};
	struct Ptr2Type:Type{Type*orig;
		inline static string NAME(Type*t){string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());b+='&';return string((char*)b.raw,b.count);}
		inline Ptr2Type(string n,Type*t):Type(n,Type::PtrSize){ptr3Valid=ptr2Valid=0;orig=t;}inline Ptr2Type(Type*t):Ptr2Type(NAME(t),t){}
	};
	struct Ptr3Type:Type{Type*orig;
		inline static string NAME(Type*t){string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());b.append((uint8_t*)"&&",2);return string((char*)b.raw,b.count);}
		inline Ptr3Type(string n,Type*t):Type(n,Type::PtrSize){ptr3Valid=ptr2Valid=0;orig=t;}inline Ptr3Type(Type*t):Ptr3Type(NAME(t),t){}
	};
	struct ArrayType:Type{Type*orig;uint32_t count;
		inline static string NAME(Type*t,uint32_t count){
			string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());
			b+='[';char _c[9];sprintf(_c,"%x",count);b.append((uint8_t*)_c,strlen(_c));b+=']';
			return string((char*)b.raw,b.count);
		}
		inline ArrayType(string n,Type*t,uint32_t c):Type(n,t->size*c){orig=t;count=c;}inline ArrayType(Type*t,uint32_t count):ArrayType(NAME(t,count),t,count){}
	};
}