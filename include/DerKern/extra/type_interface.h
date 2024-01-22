#pragma once
#include"../Type.h"
namespace DerKern{
	template<typename A,typename B>Type::Memba*ExposeMemba(Type*T,Type*exposeAs,A B::*Memba,string exposeName){return&T->membas.set(exposeName,{(uint16_t)(uint64_t)&(((B*)0)->*Memba),exposeAs});}
	template<typename B>Type::Memba*ExposeMemba(Type*T,bool B::*m,string e){return ExposeMemba<bool,B>(T,&Type::BOOL,m,e);}
	template<typename B>Type::Memba*ExposeMemba(Type*T,uint8_t B::*m,string e){return ExposeMemba<uint8_t,B>(T,&Type::u8,m,e);}
	template<typename B>Type::Memba*ExposeMemba(Type*T,uint16_t B::*m,string e){return ExposeMemba<uint16_t,B>(T,&Type::u16,m,e);}
	template<typename B>Type::Memba*ExposeMemba(Type*T,uint32_t B::*m,string e){return ExposeMemba<uint32_t,B>(T,&Type::u32,m,e);}
	template<typename B>Type::Memba*ExposeMemba(Type*T,uint64_t B::*m,string e){return ExposeMemba<uint64_t,B>(T,&Type::u64,m,e);}
	template<typename B>Type::Memba*ExposeMemba(Type*T,int8_t B::*m,string e){return ExposeMemba<int8_t,B>(T,&Type::i8,m,e);}
	template<typename B>Type::Memba*ExposeMemba(Type*T,int16_t B::*m,string e){return ExposeMemba<int16_t,B>(T,&Type::i16,m,e);}
	template<typename B>Type::Memba*ExposeMemba(Type*T,int32_t B::*m,string e){return ExposeMemba<int32_t,B>(T,&Type::i32,m,e);}
	template<typename B>Type::Memba*ExposeMemba(Type*T,int64_t B::*m,string e){return ExposeMemba<int64_t,B>(T,&Type::i64,m,e);}
	template<typename B>Type::Memba*ExposeMemba(Type*T,float B::*m,string e){return ExposeMemba<float,B>(T,&Type::f,m,e);}
	template<typename B>Type::Memba*ExposeMemba(Type*T,double B::*m,string e){return ExposeMemba<double,B>(T,&Type::d,m,e);}
	template<typename B>Type::Memba*ExposeMemba(Type*T,string B::*m,string e){return ExposeMemba<string,B>(T,&Type::str,m,e);}

	//void insComp(Instructionary*thee){thee->compile();}
	//Type instructionary("instructie",sizeof(Instructionary));
	//ExposeMemba(t,bool(0),&Instructionary::inliny,"inline");
	//ExposeMemba(t,sizeof(int*)<8?&Type::u8:&Type::u16,&Instructionary::mayChange,"mayChange");
	//insComp_a=.make<Type*>(&instructionary);
	//auto insComp_=.make<FuncType>(insComp,&Type::VOID,&insComp_a,1)
	//t->membas.set("compile",{insComp_->type,insComp_});
}