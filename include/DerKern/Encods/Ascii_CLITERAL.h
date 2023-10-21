#pragma once
#include"Ascii.h"
#include"../Values.h"
#include"../Type.h"
namespace DerKern{
	namespace ASCII{
		struct CLITERAL_result{//I call it that because I remember "CLITERAL" was in raylib
			enum class Types{
				FAILED=0,
				u8,u16,u32,u64,
				i8,i16,i32,i64,
				flot,dobl,
				strng,name
			};
			uint8_t type;
			union{
				 uint8_t  u8; int8_t  i8;
				uint16_t u16;int16_t i16;
				uint32_t u32;int32_t i32;
				uint64_t u64;int64_t i64;
				float f;double d;
				string s;
			};
			inline CLITERAL_result(){type=0;}
			inline CLITERAL_result(uint8_t v){type=1;u8=v;}inline CLITERAL_result(uint16_t v){type=2;u16=v;}
			inline CLITERAL_result(uint32_t v){type=3;u32=v;}inline CLITERAL_result(uint64_t v){type=4;u64=v;}
			inline CLITERAL_result(int8_t v){type=5;i8=v;}inline CLITERAL_result(int16_t v){type=6;i16=v;}
			inline CLITERAL_result(int32_t v){type=7;i32=v;}inline CLITERAL_result(int64_t v){type=8;i64=v;}
			inline CLITERAL_result(float v){type=9;f=v;}inline CLITERAL_result(double v){type=10;d=v;}
			inline CLITERAL_result(string v,bool name){type=11+name;new(&s)string(v);}
			inline operator bool()const{return type;}
			inline bool operator!()const{return!type;}
			inline~CLITERAL_result(){if(type>10)s.~string();}
			inline _Variable toTV(const Scope*scop){
				if(type==1)return{Type::u8.Const(),Location(&u8)};
				if(type==2)return{Type::u16.Const(),Location(&u16)};
				if(type==3)return{Type::u32.Const(),Location(&u32)};
				if(type==4)return{Type::u64.Const(),Location(&u64)};
				if(type==5)return{Type::i8.Const(),Location(&i8)};
				if(type==6)return{Type::i16.Const(),&i16};
				if(type==7)return{Type::i32.Const(),Location(&i32)};
				if(type==8)return{Type::i64.Const(),Location(&i64)};
				if(type==9)return{Type::f.Const(),Location(&f)};
				if(type==10)return{Type::d.Const(),Location(&d)};
				if(type==11)return{Type::str.Const(),Location(&s)};
				if(type==12&&scop){auto _=*scop->get(s);_.val=_.val.resolve();return _;}
				return{0,{}};
			}
			template<typename t>inline operator t()const{
				if(type==1)return(t)u8;
				if(type==2)return(t)u16;
				if(type==3)return(t)u32;
				if(type==4)return(t)u64;
				if(type==5)return(t)i8;
				if(type==6)return(t)i16;
				if(type==7)return(t)i32;
				if(type==8)return(t)i64;
				if(type==9)return(t)f;
				if(type==10)return(t)d;
				return(t)0xcdcdcdcdcdcdcdcd;
			}
		};
		//I call it that because I remember "CLITERAL" was in raylib
		//doesn't go back on fail, only supports numbers, strings, names.
		CLITERAL_result CLITERAL(Inputo::Universa*);
	}
}