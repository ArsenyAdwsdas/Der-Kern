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
			inte,//u8,u16,u32,u64,i8,i16,i32,i64; I suggest not setting this TypeID because it's troublesome though...
			flot,//float,double; I suggest not setting this TypeID because it's troublesome.
			IDK_MODIFIER,//Basically unknown.
			//Modifiers
			Const,ptr1,ptr2,ptr3,
			//SpecialTypes
			array,func,
			IDK//Is one hell of a problem. The real type is (Type*)IDK.location.eval()
			ENUM_END
		IDs::T typeID;//would've written  except then I'm not allowed to just do =0...
		bool isFinal;//1 means *, &, &&, [] aren't allowed
		bool stacky;//0 means  objects of this type are allowed to live in registers
		Type*parent;string name;uint32_t size;
		struct Memba{
			ENUM(Protectio,uint8_t)Public,Protected,Private ENUM_END Protectio::T prot;
			ENUM(Types,uint8_t)
				invalid,
				normal,//C style struct member
				gset,//getters-setters
				stat//static
				ENUM_END Types::T type;
			union{struct{uint16_t off;Type*t;};/*gset*/_Variable sta;};
			inline Memba(){type=Types::invalid;}
			inline Memba(uint16_t o,Type*T,Protectio::T pro=0){type=1;off=o;t=T;prot=pro;}
			//inline Memba(
			inline Memba(_Variable v,Protectio::T pro=0){type=3;sta=v;prot=pro;}
			inline Memba(const Memba&m){type=m.type;if(type==1){off=m.off;t=m.t;assert(t);}/*gset*/if(type==3){new(&sta)_Variable(m.sta);assert(sta.a);}prot=m.prot;}
			inline~Memba(){
				//gset
			}
			inline Memba&operator=(const Memba&m){if(this!=&m){this->~Memba();new(this)Memba(m);}}
		};
		Dicto<string,Memba,uint16_t,8,&cmp>	membas;inline Memba*memba(string n)const{auto z=membas.get(n);if(z)return z;assert(parent!=this);return parent?parent->memba(n):0;}
		inline Type(string n,uint32_t s){typeID=IDs::normal;name=n;size=s;isFinal=0;parent=0;}
		inline Type(Type*p,string n,uint32_t extraSize=0):Type(n,p->size+extraSize){parent=p;}
		inline Type():Type("???",0){}
		inline bool memOnly(){return size>sizeof(int*)||memba("+this+");}
		
		ENUM(UOps,uint8_t)
			Spawn=0,//type.statics["$this"](value) or nothing		a default constructor
			Mitosis=1,//type.statics["+this+"](value) or nothing	a copy constructor
			Despawn=2,//type.statics["~this~"](value) or nothing	a destructor
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
		static Type i8,		u8;
		static Type i16,	u16;
		static Type i32,	u32;
		static Type i64,	u64;
		static Type f,d;//float/double
		static Type str,VOID,BOOL;

		constexpr static uint8_t C_Count=12;//without _type, idk[16], containers(5)

		static Type _type;//type of "Type"s
		//OverloadedFunction, FuncType, "const", "array", "*", "&", "&&"		are basically unused except as operator and variable data(like names and where to find) containers.
		static Type overloadery,func_container,const_container,array_container;
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
	struct TypeModifier:Type{Type*orig;inline TypeModifier(string n,Type*t,uint32_t siz):Type(n,siz){orig=t;typeID=IDs::IDK_MODIFIER;}};
	struct ConstType:TypeModifier{
		inline static string NAME(Type*t){string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());b.append((uint8_t*)"const",5);return string((char*)b.raw,b.count);}
		inline ConstType(string n,Type*t):TypeModifier(n,t,t->size){typeID=IDs::Const;}inline ConstType(Type*t):ConstType(NAME(t),t){}
	};
	struct Ptr1Type:TypeModifier{
		inline static string NAME(Type*t){string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());b+='*';return string((char*)b.raw,b.count);}
		inline Ptr1Type(string n,Type*t):TypeModifier(n,t,sizeof(int*)){typeID=IDs::ptr1;}inline Ptr1Type(Type*t):Ptr1Type(NAME(t),t){}
	};
	struct Ptr2Type:TypeModifier{
		inline static string NAME(Type*t){string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());b+='&';return string((char*)b.raw,b.count);}
		inline Ptr2Type(string n,Type*t):TypeModifier(n,t,sizeof(int*)){isFinal=1;typeID=IDs::ptr2;}inline Ptr2Type(Type*t):Ptr2Type(NAME(t),t){}
	};
	struct Ptr3Type:TypeModifier{
		inline static string NAME(Type*t){string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());b.append((uint8_t*)"&&",2);return string((char*)b.raw,b.count);}
		inline Ptr3Type(string n,Type*t):TypeModifier(n,t,sizeof(int*)){isFinal=1;typeID=IDs::ptr3;}inline Ptr3Type(Type*t):Ptr3Type(NAME(t),t){}
	};
	struct ArrayType:TypeModifier{uint32_t count;
		inline static string NAME(Type*t,uint32_t count){
			string _;BBuf b;b.append((uint8_t*)t->name.c_str(),t->name.size());
			b+='[';char _c[9];sprintf(_c,"%x",count);b.append((uint8_t*)_c,strlen(_c));b+=']';
			return string((char*)b.raw,b.count);
		}
		inline ArrayType(string n,Type*t,uint32_t c):TypeModifier(n,t,t->size*c){orig=t;count=c;typeID=IDs::array;}inline ArrayType(Type*t,uint32_t count):ArrayType(NAME(t,count),t,count){}
	};
}