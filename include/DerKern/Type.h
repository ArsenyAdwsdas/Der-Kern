#pragma once
#include<string.h>
#include"Values.h"
#ifndef TypeExtraS___
#define TypeExtraS___
#endif
namespace DerKern{
	struct Type;int cmp(Type*const&,Type*const&);int cmp(Type*const&a,const string&b);
	struct TypeModifier;struct ConstType;struct Ptr1Type;struct Ptr2Type;struct Ptr3Type;
	struct ArrayType;
	struct Type{
		ENUM(IDs,uint8_t)
			normal,
			IDK_MODIFIER,//Basically unknown.
			//Modifiers
			Const,ptr1,ptr2,ptr3,
			//SpecialTypes
			array,func,
			IDK//Is one hell of a problem. The real type is (Type*)IDK.location.eval()
			ENUM_END
		IDs::T typeID;//would've written  except then I'm not allowed to just do =0...
		bool isFinal;//1 means *, &, &&, [] aren't allowed
		Type*parent;string name;uint32_t size;
		inline Type(string n,uint32_t s){typeID=0;name=n;size=s;isFinal=0;parent=0;}
		inline Type(Type*p,string n,uint32_t extraSize=0){typeID=0;name=n;size=p->size+extraSize;isFinal=0;parent=0;}
		inline Type():Type("???",0){typeID=0;}
		Dicto<string,_Variable,uint16_t,8,&cmp>Statics;inline _Variable*Static(string n)const{auto z=Statics.get(n);if(z)return z;return parent?parent->Static(n):0;}
		Dicto<string,uint32_t,uint16_t,8,&cmp>	Membas;inline 	uint32_t*Memba(string n)const{auto z=Membas.get(n);if(z)return z;return parent?parent->Memba(n):0;}//u32 is basically MemberOffsetFrom0AtThis
		ENUM(UOps,uint8_t)
			Equ,//=
			Negate,Plusate,//-,+
			Not,BNot,//!,~
			Star,//*
			Dolla,Hash//$,#
			ENUM_END
		ENUM(Ops,uint8_t)
			Set,//=
			Plus,Minus,Mul,Div,//+,-,*,/
			PlusE,MinusE,MulE,DivE,//+=,-=,*=,/=

			BOr,BAnd,BXor,Mod,//|,&,^,%
			BOrE,BAndE,BXorE,ModE,//|=,&=,^=,%=

			Left,BLeft,BRight,Right,//<,<<,>>,>
			LeftE,BLeftE,BRightE,RightE,//<=,<<=,>>=,>=

			Or,And,Eq,NEq//||,&&,==,!=
			ENUM_END
		TypeExtraS___
		static Type i8;static Type u8;
		static Type i16;static Type u16;
		static Type i32;static Type u32;
		static Type i64;static Type u64;
		static Type f;static Type d;//float/double
		static Type str;static Type VOID;

		constexpr static uint8_t C_Count=12;//without _type, idk[16], containers(5)

		static Type _type;//type of "Type"s
		static Type overloadery;//type of "OverloadedFunction"s
		//FuncType, "const", "array", "*", "&", "&&"		are basically unused except as operator and variable data(like names and where to find) containers.
		static Type func_container;
		static Type const_container;
		static Type array_container;
		constexpr static uint8_t PtrSize=sizeof(int*);
		static Ptr1Type*ptr1_container;
		static Ptr2Type*ptr2_container;
		static Ptr3Type*ptr3_container;

		Ptr1Type*pointer();Ptr2Type*pointer2();Ptr3Type*pointer3();//*, &, &&
		ConstType*Const();//make it "const"
		ArrayType*array(uint32_t count);
		private:
		ConstType*_const=0;Ptr1Type*_pointer=0;Ptr2Type*_pointer2=0;Ptr3Type*_pointer3=0;
		SortList<Type*,uint16_t,16,&cmp>_arrays;
	};
	struct TypeModifier:Type{Type*orig;inline TypeModifier(string n,Type*t,uint32_t siz):Type(n,siz){orig=t;typeID=1;}};
	struct ConstType:TypeModifier{
		inline static string NAME(Type*t){string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());b.append((uint8_t*)"const",5);return string((char*)b.raw,b.count);}
		inline ConstType(string n,Type*t):TypeModifier(n,t,t->size){typeID=2;}inline ConstType(Type*t):ConstType(NAME(t),t){}
	};
	struct Ptr1Type:TypeModifier{
		inline static string NAME(Type*t){string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());b+='*';return string((char*)b.raw,b.count);}
		inline Ptr1Type(string n,Type*t):TypeModifier(n,t,Type::PtrSize){typeID=3;}inline Ptr1Type(Type*t):Ptr1Type(NAME(t),t){}
	};
	struct Ptr2Type:TypeModifier{
		inline static string NAME(Type*t){string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());b+='&';return string((char*)b.raw,b.count);}
		inline Ptr2Type(string n,Type*t):TypeModifier(n,t,Type::PtrSize){isFinal=1;typeID=4;}inline Ptr2Type(Type*t):Ptr2Type(NAME(t),t){}
	};
	struct Ptr3Type:TypeModifier{
		inline static string NAME(Type*t){string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());b.append((uint8_t*)"&&",2);return string((char*)b.raw,b.count);}
		inline Ptr3Type(string n,Type*t):TypeModifier(n,t,Type::PtrSize){isFinal=1;typeID=5;}inline Ptr3Type(Type*t):Ptr3Type(NAME(t),t){}
	};
	struct ArrayType:TypeModifier{uint32_t count;
		inline static string NAME(Type*t,uint32_t count){
			string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());
			b+='[';char _c[9];sprintf(_c,"%x",count);b.append((uint8_t*)_c,strlen(_c));b+=']';
			return string((char*)b.raw,b.count);
		}
		inline ArrayType(string n,Type*t,uint32_t c):TypeModifier(n,t,t->size*c){orig=t;count=c;typeID=6;}inline ArrayType(Type*t,uint32_t count):ArrayType(NAME(t,count),t,count){}
	};



}