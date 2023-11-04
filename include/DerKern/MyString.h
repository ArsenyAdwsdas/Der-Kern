#pragma once
#include<assert.h>
#include<new>
#include<stddef.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifndef LOG_ACTIONS
	//#define LOG_ACTIONS(...)printf(__VA_ARGS__) // to disable comment this line or   #define LOG_ACTIONS(...){}
#endif
namespace DerKern{
	//my version of https://en.cppreference.com/w/cpp/string/basic_string
	struct IMyString{//https://github.com/malware-dev/MDK-SE/wiki/Api-Index has lots of "IMy" so I decided I'll name it this way since I don't really want to replace "std::string" with it yet...
		char*raw;
		typedef uint32_t SIZET;inline SIZET&size(){return*(SIZET*)(uintptr_t(raw)-sizeof(SIZET));}inline const SIZET&size()const{return((IMyString*)this)->size();}
		inline SIZET&length(){return size();}inline SIZET length()const{return size();}
		explicit inline operator char*()const{return raw;}
		inline const char*c_str()const{if(!size())return"";return raw;};inline const char*data()const{return c_str();}
		inline char&operator[](SIZET i){assert(size()>i);return raw[i];}inline char operator[](SIZET i)const{assert(size()>i);return raw[i];}
		//inline int compare(IMyString b)const{SIZET a=size(),_b=b.size(),w=a>_b?_b:a;int _=memcmp(raw,b.raw,w);if(_!=0)return _;return a<_b?1:a==_b?0:-1;}
	};
	struct _MyString:IMyString{
		inline _MyString(){raw=0;}
		inline _MyString(const char*str,SIZET siz){raw=(char*)malloc(siz+sizeof(siz))+sizeof(siz);memcpy(raw,str,siz);size()=siz;}
		inline _MyString(const char*str):_MyString(str,strlen(str)){}
		inline _MyString(IMyString w):_MyString(w.raw,w.size()){}
		inline~_MyString(){free(&size());}
	};
	#ifndef LOG_ACTIONS
		#define LOG_ACTIONS(...){}
	#endif
	struct MyString:IMyString{
		typedef uint8_t REF_T;inline REF_T&ref_count(){return*(REF_T*)(uintptr_t(&size())-sizeof(REF_T));}inline const REF_T&ref_count()const{return((MyString*)this)->ref_count();}
		//^ starts with 0 ^
		inline MyString(){raw=0;}explicit inline MyString(void*w){raw=(char*)w;}
		inline MyString(const char*str,SIZET siz){raw=(char*)malloc(siz+sizeof(siz)+sizeof(REF_T)+1)+sizeof(siz)+sizeof(REF_T);memcpy(raw,str,siz);raw[siz]=0;size()=siz;ref_count()=0;LOG_ACTIONS("/summon %p(%llu)\n",raw,uint64_t(ref_count()));}
		inline MyString(const char*str):MyString(str,strlen(str)){}
		inline MyString(IMyString w):MyString(w.raw,w.size()){}
		inline MyString operator+(IMyString b)const{
			SIZET _s=size()+b.size();
			MyString c;c.raw=(char*)malloc(_s+sizeof(_s)+sizeof(REF_T))+sizeof(_s)+sizeof(REF_T);c.size()=_s;c.ref_count()=0;
			memcpy(c.raw,raw,size());memcpy(c.raw+size(),b.raw,b.size());
			return c;
		}inline MyString&operator+=(IMyString b){return(*this=*this+b);}
		inline MyString substr(SIZET start,SIZET count)const{return{raw+start,count};}
		inline bool operator==(const MyString&z)const{if(size()!=z.size())return 0;for(SIZET i=0;i<size();i++)if(raw[i]!=z.raw[i])return 0;return 1;}inline bool operator==(const MyString&&z)const{return*this==z;}



		inline MyString(const MyString&w){raw=w.raw;if(raw){ref_count()++;assert(ref_count());LOG_ACTIONS("+%p(%llu)\n",raw,(uint64_t)ref_count());}}inline MyString(const MyString&&w):MyString(w){}
		inline~MyString(){if(raw){if(ref_count()){ref_count()--;LOG_ACTIONS("-%p(%llu)\n",raw,(uint64_t)ref_count());}else{LOG_ACTIONS("/kill %p(%llu)\n",raw,(uint64_t)ref_count());free(&ref_count());}}}
		inline MyString&operator=(const MyString&w){if(w.raw==raw)return*this;this->~MyString();raw=0;new(this)MyString(w);return*this;}inline MyString&operator=(const MyString&&w){return*this=w;}
	};
	#undef LOG_ACTIONS
}