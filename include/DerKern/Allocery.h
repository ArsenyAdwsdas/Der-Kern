#pragma once
#include"StandloTypes.h"
namespace DerKern{
	struct Location{
		enum class Types:uint8_t{
			undecided=0,//non-existent
			imovbl=1,//forever at pointer, maybe you don't want to get rid that place... just saying...
			reg=2,dReg=reg|imovbl//register/dereferenced register
		};uint8_t type;
		int32_t imov;//if(type==1)v=*(*)(uint32_t)imov;if(type==dReg)v=*((*)reg+imov)
		uint8_t reg;//register
		inline operator uint8_t(){return type&4?reg:(uint8_t)-1;}
		inline Location(){type=0;}
		inline Location(int32_t o){type=1;imov=o;}
		inline Location(uint8_t r){type=2;reg=r;}
		inline Location(uint8_t r,int32_t o){type=2;reg=r;imov=o;}
	};
	typedef Location FLocation;
	struct _Variable;struct Variable;struct Type;
	/*struct _Func{
		bool mayChange[RegisterCount];
		//bool external;//may or may not need registers from C++
		pair<Type*,Location>out;//no type=no output
		uint8_t inC;pair<Type*,Location>*in;//inC=count of pairs at "in"
		void*f;//where it is located at
	};*/
	typedef void*FuncHead;
	constexpr uint8_t FuncHead_size=(9*sizeof(int*)/4)+1;
	typedef
		#if _WIN64||__x86_64__||__ppc64__
			uint16_t
		#else
			uint8_t
		#endif
		FuncHead_Changies_t;
	inline void*&FuncHead_FuncPtr(FuncHead z){return *(void**)z;}
	inline FuncHead_Changies_t&FuncHead_Changies(FuncHead z){return*(FuncHead_Changies_t*)((char*)z+1);}//&1 tells it's inline one
	inline uint8_t&FuncHead_ArgC(FuncHead z){return*(uint8_t*)(1+&FuncHead_Changies(z));}
	inline void*&FuncHead_ArgV(FuncHead z){return*(void**)(1+&FuncHead_ArgC(z));}
	
	struct _Variable{
		Type*type;
		union{
			Location val;
		};
		inline _Variable():type(0),val(){}
		inline _Variable(Type*t,Location v):type(t),val(v){}
	};
	struct Variable:_Variable{
		string name;
		inline Variable(_Variable z,string n):name(n){*(_Variable*)this=z;}
		inline Variable(Type*t,Location v,string n):name(n),_Variable(t,v){}
	};
}