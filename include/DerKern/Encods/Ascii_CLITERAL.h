#pragma once
#include"Ascii.h"
#include"../Type.h"
namespace DerKern{
	namespace ASCII{
		struct CLITERAL_result{
			enum class Type{
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
			inline CLITERAL_result(int8_t v){type=5;u8=v;}inline CLITERAL_result(int16_t v){type=6;u16=v;}
			inline CLITERAL_result(int32_t v){type=7;u32=v;}inline CLITERAL_result(int64_t v){type=8;u64=v;}
			inline CLITERAL_result(float v){type=9;f=v;}inline CLITERAL_result(double v){type=10;d=v;}
			inline CLITERAL_result(string v,bool name){type=11+name;new(&s)T(v);}
			inline operator bool(){return type;}
			inline bool operator!(){return!type;}
			inline~CLITERAL_result(){if(type>10)s.~string();}
			template<typename t>inline operator t(){
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
		CLITERAL_result CLITERAL(Inputo::Universa*);//doesn't go back on fail, only supports numbers, strings, names.
	}
}