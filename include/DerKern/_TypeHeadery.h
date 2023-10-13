#pragma once
#include"Allocery.h"
namespace DerKern{
	typedef void*TypeHead;// void=(uint8_t)128; toResolve=(uint8_t)129,(uint16_t)stringSize,string; func=(uint8_t)130,*TypeHead,[(uint8_t)255,*TypeHead]?; point=(uint8_t)131,TypeHead; resolved=(uint8_t)132,TypeDesc*
	namespace TypeHeads{
		inline uint16_t _toResolveSize(string s){return 3+s.size();}
		inline TypeHead _toResolve(TypeHead z,string s){*(uint8_t*)z=129;z=1+(char*)z;*(uint16_t*)z=s.size();z=2+(char*)z;memcpy(z,s.c_str(),s.size());return s.size()+(char*)z;}//z=s; doesn't terminate; returns pointer to a byte after it
		constexpr uint8_t _PointSize=1+sizeof(int*);
		inline TypeHead _Point(TypeHead z,TypeHead t){*(uint8_t*)z=131;z=1+(char*)z;*(void**)z=t;return sizeof(int*)+(char*)z;}//z=&t; doesn't terminate; returns pointer to a byte after it
		inline TypeHead _Resolved(TypeHead z,TypeDesc*t){*(uint8_t*)z=132;z=1+(char*)z;*(void**)z=t;return sizeof(int*)+(char*)z;}//z=&t; doesn't terminate; returns pointer to a byte after it
		inline uint8_t* _skip(TypeHead _z){
			if(_z==0)return 0;
			auto z=(uint8_t*)_z;
			uint8_t _=*z;z+=1;
			if(_==132||_==131)z+=sizeof(int*);// Resolved || Point
			else if(_==129)z+=2+*(uint16_t*)z;// toResolve
			else if(_==130){z++;z=(uint8_t*)_skip(_z);while(255==*z){z++;z=(uint8_t*)_skip(_z);}}// func
			else if(_==128)z++;// void
			while((_=*z)&&_!=2&&_!=255){// <end>(0) || &(2) || <arg>
				if(1==*z)z++;// *
				else if(3==*z){z+=5;}// a[uint32_t]
			}if(_!=255)z++;
			return z;
		}//points to <after type at _z>
		inline uint16_t _size(TypeHead _z){return uint16_t(size_t(_skip(_z))-size_t(_z));}
		inline TypeHead _func(TypeHead _z,TypeHead ret,uint8_t argC,TypeHead*argV){//z= ret func(argV[argC]); doesn't terminate; returns pointer to a byte after it
			auto z=(uint8_t*)_z;
			*z=130;z++;
			uint16_t s=_size(ret);s=s?(s-(((uint8_t*)ret)[s-1]==0)):0;
			if(s>sizeof(int*)+1){memcpy(z,ret,s);}
			else{s=_PointSize;_Point(z,ret);}
			z+=s;
			for(uint8_t i=0;i<argC;i++){
				s=_size(argV[i]);s=s?(s-(((uint8_t*)argV[i])[s-1]==0)):0;
				if(s>sizeof(int*)+1){memcpy(z,argV[i],s);}
				else{s=_PointSize;_Point(z,argV[i]);}
				z+=s;
			}
			return z;
		}
		inline TypeHead _funcSize(TypeHead ret,uint8_t argC,TypeHead*argV){
			uint16_t z=1;
			uint16_t s=_size(ret);s=s?(s-(((uint8_t*)ret)[s-1]==0)):0;
			if(s<=sizeof(int*)+1)s=_PointSize;
			z+=s;
			for(uint8_t i=0;i<argC;i++){
				s=_size(argV[i]);s=s?(s-(((uint8_t*)argV[i])[s-1]==0)):0;
				if(s<=sizeof(int*)+1)s=_PointSize;
				z+=s;
			}
			return z;
		}
	}
}