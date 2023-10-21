#pragma once
#include"StandloTypes.h"
namespace DerKern{
	struct RegisterState{
		#if _WIN64||__x86_64__||__ppc64__
			typedef uint64_t regT;
		#else
			typedef uint32_t regT;
		#endif
		regT raw[sizeof(int*)==4?8:16];
		template<typename t>inline regT&operator[](t z){return raw[z];}
	};
	struct Location{
		ENUM(Types,uint8_t)
			undecided=0,//non-existent
			imovbl=1,//forever at pointer, maybe you don't want to get rid that place... just saying...
			reg=2,dReg=reg|imovbl,//register/dereferenced register
			ptr=4,//will probably be mostly very unusable...
			ref=ptr|dReg//will act as *(Location*)ptr
			ENUM_END uint8_t type;
		union{
			int32_t imov;//if(type==1)v=*(*)(uint32_t)imov;if(type==dReg)v=*((*)reg+imov)
			uint8_t reg;//register
			void*ptr;
		};
		inline Location&resolve(){auto _=this;while(_->type==7)_=(Location*)_->ptr;return*_;}
		inline const Location&resolve()const{auto _=this;while(_->type==7)_=(Location*)_->ptr;return*_;}
		inline void*eval(RegisterState s)const{
			if(type==1)return(void*)(int64_t)imov;
			if(type==2)return&s[reg];
			if(type==3)return*(void**)(s[reg]+imov);
			if(type==4)return ptr;
			return 0;
		}inline void*eval()const{
			if(type==1)return(void*)(int64_t)imov;
			if(type==4)return ptr;
			return 0;
		}
		inline Location(){type=0;}
		inline Location(int32_t o){type=1;imov=o;}
		inline Location(uint8_t r){type=2;reg=r;}
		inline Location(uint8_t r,int32_t o){type=2;reg=r;imov=o;}
		inline Location(void*p){type=4;ptr=p;}
		inline Location(Location*p){type=7;ptr=p;}
	};
	typedef Location FLocation;
	struct _Variable;struct Type;
	/*struct _Func{
		bool mayChange[RegisterCount];
		//bool external;//may or may not need registers from C++
		pair<Type*,Location>out;//no type=no output
		uint8_t inC;pair<Type*,Location>*in;//inC=count of pairs at "in"
		void*f;//where it is located at
	};*/
	typedef
		#if _WIN64||__x86_64__||__ppc64__
			uint16_t
		#else
			uint8_t
		#endif
		FuncHead_Changies_t;
	// typedef void FuncHead;
	// constexpr uint8_t FuncHead_size=(9*sizeof(int*)/4)+1;
	// inline void*&FuncHead_FuncPtr(FuncHead*z){return *(void**)z;}
	// inline FuncHead_Changies_t&FuncHead_Changies(FuncHead z){return*(FuncHead_Changies_t*)((char*)z+1);}//&32 means inline,&64 means special esp/ebp hell
	// inline uint8_t&FuncHead_ArgC(FuncHead*z){return*(uint8_t*)(1+&FuncHead_Changies(z));}
	// inline void*&FuncHead_ArgV(FuncHead*z){return*(void**)(1+&FuncHead_ArgC(z));}
	
	struct _Variable{
		Type*type;
		union{
			Location val;
		};
		inline _Variable():type(0),val(){}
		inline _Variable(Type*t,Location v=Location()):type(t),val(v){}
	};
	typedef pair<string,_Variable>Variable;inline int cmp(const Variable&a,const Variable&b){return cmp(a.a,b.a);}
	struct DefaultableVariable:Variable{
		Location deflt;bool deflted=0;
	};
	struct Scope{
		Dicto<string,_Variable,uint16_t,16,cmp>vars;
		Scope*parent;
		inline _Variable*get(string n)const{auto z=vars.get(n);if(z)return z;return parent->get(n);}
		inline void set(string n,_Variable z){vars.set(n,z);}
		void operator+=(Type*t);void operator+=(Type&t);
	};
}