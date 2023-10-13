#pragma once
#include"Allocery.h"
namespace DerKern{
	struct Type;int cmp(const Type*&,const Type*&);int cmp(const Type*&a,const string&b);
	struct FuncType;struct Ptr1Type;struct Ptr2Type;struct Ptr3Type;
	struct Type{
		bool ptr1Valid,ptr2Valid,ptr3Valid;//whether *, &, && are allowed
		string name;
		uint32_t size;
		inline Type(string n,uint32_t s){name=n;size=s;ptr1Valid=1;ptr2Valid=ptr3Valid=s;}
		inline Type():Type("???",0){}
		Dicto<string,_Variable,uint16_t,8>vars;
		enum UOps:uint8_t{
			negate=0,plus,//-, +
			not,bNot,//!, ~
			star,//*
			dollar,hash,equals,//$, #, =
			Count
		};
		enum Ops:uint8_t{
			plus=0,minus,//+, -
			plusE,minusE,//+=, -=
			mul,div,//*, /
			mulE,divE,//*=, /=
			bAnd,bOr,//&, |
			bAnd,bOrE,//&=, |=
			mod,bXor,//%, ^
			modE,bXorE,//%=, ^=
			bLeft,bRight//<<, >>
			bLeftE,bRightE//<<=, >>=
			eq,neq,//==, !=
			and,or,//&&, ||
			left,right//<, >
			leftE,rightE//<=, >=
			Count
		};
		#ifdef FASTER
			Type*_uops[UOps::Count];
			Type*_ops[Ops::Count];
		#endif
		extern static Type i8;extern static Type u8;
		extern static Type i16;extern static Type u16;
		extern static Type i32;extern static Type u32;
		extern static Type i64;extern static Type u64;
		extern static Type f;extern static Type d;//float/double
		extern static Type str;extern static Type _type;
		extern static Type idk[16];//idk*(this pointer's type ain't resolvable until runtime)[register with type id]

		constexpr static uint8_t PtrSize=sizeof(int*);

		//"*", "&", *FuncHead		are basically unused except as operator and variable data(like names and where to find) containers.
		extern static Ptr1Type ptr1;
		extern static Ptr2Type ptr2;
		extern static Ptr3Type ptr3;
		extern static Type func;

		Ptr1Type*pointer();Ptr2Type*pointer2();Ptr3Type*pointer3();//*, &, &&
		Type*array(uint32_t count);
		Type*func(Type**argv,uint8_t argc);
		private:Type*_pointer,_pointer2;SortList<Type*>_arrays;SortList<FuncType*>_funcs;
	};
	struct FuncType:Type{
		Type rets;Type**argv;uint8_t argc;
		inline static string NAME(Type*rets,Type**argv,uint8_t argc){
			string _;BBuf b;
			b.append(rets->name.c_str(),rets.name.size());
			b+='(';if(argc)b.append(argv[0]->name.c_str(),argv[0]->name.size);for(uint8_t i=1;i<argc;i++){b+=',';b.append(argv[i]->name.c_str(),argv[i]->name.size);}b+=')';
			return string(b.raw,b.count);
		}
		inline FuncType(string n,Type*_rets,Type**_argv,uint8_t _argc):Type(n,0){rets=_rets;argv=mallocCpy(_argv,_argc);argc=_argc;}
		inline FuncType(Type*_rets,Type**_argv,uint8_t _argc):FuncType(NAME(_rets,_argv,_argc),_rets,_argv,_argc){}
	};
	struct Ptr1Type:Type{Type*orig;
		inline static string NAME(Type*t){string _;BBuf b;b.append(t->name.c_str(),t.name.size());b+='*';return string(b.raw,b.count);}
		inline Ptr1Type(string n,Type*t):Type(n,Type::PtrSize){orig=t;}inline Ptr1Type(Type*t):Ptr1Type(NAME(t),t){}
	};
	struct Ptr2Type:Type{Type*orig;
		inline static string NAME(Type*t){string _;BBuf b;b.append(t->name.c_str(),t.name.size());b+='&';return string(b.raw,b.count);}
		inline Ptr2Type(string n,Type*t):Type(n,Type::PtrSize){ptr3Valid=ptr2Valid=0;orig=t;}inline Ptr2Type(Type*t):Ptr2Type(NAME(t),t){}
	};
	struct Ptr3Type:Type{Type*orig;
		inline static string NAME(Type*t){string _;BBuf b;b.append(t->name.c_str(),t.name.size());b+="&&";return string(b.raw,b.count);}
		inline Ptr3Type(string n,Type*t):Type(n,Type::PtrSize){ptr3Valid=ptr2Valid=0;orig=t;}inline Ptr3Type(Type*t):Ptr3Type(NAME(t),t){}
	};
}