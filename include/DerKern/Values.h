#pragma once
#include"StandloTypes.h"
namespace DerKern{
	template<uint8_t bytes>struct ByteBooly{
		uint8_t raw[bytes];
		inline ByteBooly(){for(uint8_t i=0;i<bytes;i++)raw[i]=0;}
		inline bool operator()(uint8_t N){return raw[N>>3]&(1<<(N&7));}
		inline bool operator()(uint8_t N,bool val){uint8_t&w=raw[N>>3];N=N&7;w=(w&(~(1<<N)))|((val?1:0)<<N);return val;}
	};
	struct RegisterState{
		#if _WIN64||__x86_64__||__ppc64__
			typedef uint64_t regT;
		#else
			typedef uint32_t regT;
		#endif
		regT raw[sizeof(int*)==4?8:16]={0};
		template<typename t>inline regT&operator[](t r){return raw[r];}
	};
	struct KnownStateR:RegisterState{
		bool known[sizeof(int*)*2]={0};
		template<typename t>inline bool&operator%(t r){return known[r];}
	};
	struct FlagState{
		uint8_t raw=0;
		inline bool operator()(uint8_t f){return raw&(1<<f);}
		inline void operator()(uint8_t f,bool val){raw=(raw&(~(1<<f)))|((val?1:0)<<f);}
	};
	struct KnownStateF:FlagState{
		FlagState known;
		inline void operator()(uint8_t f,bool val){known(f,1);((FlagState&)*this)(f,val);}
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
			struct{
				int32_t imov;//if(type==1)v=*(*)(uint32_t)imov;if(type==dReg)v=*((*)reg+imov)
				uint8_t reg;//register
			};
			void*ptr;
		};
		inline const Location&resolve()const{auto _=this;while(_->type==7)_=(Location*)_->ptr;return*_;}
		inline Location&resolve(){return(Location&)((const Location*)this)->resolve();}////Yes, a const_cast. SO WHAT, GONNA NAG AND SAY IT'S BAD?
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
		inline Location(uint8_t r,int32_t o){type=3;reg=r;imov=o;}
		inline Location(void*p){if(sizeof(int*)==4){type=1;imov=(int32_t)(uintptr_t)p;}else{type=4;ptr=p;}}
		inline Location(Location*p){type=7;ptr=p;}
		inline bool operator==(Location z)const{
			if(type!=z.type)return 0;
			if(type==1)return imov==z.imov;
			if(type==2)return reg==z.reg;
			if(type==3)return reg==z.reg&&imov==z.imov;
			if(type==4||type==7)return ptr==z.ptr;
			return 0;
		}inline bool operator!=(Location z)const{return!(z==*this);}
		inline operator bool()const{return resolve().type;}
		inline bool operator!()const{return resolve().type;}
	};
	typedef Location FLocation;
	struct Type;
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
	
	/*struct _Variable{
		Type*type;
		//union{
			Location val;
		//};
		inline _Variable():type(0),val(){}
		inline _Variable(Type*t,Location v=Location()):type(t),val(v){}
	};*/
	typedef pair<Type*,Location>_Variable;
	typedef pair<string,_Variable>Variable;inline int cmp(const Variable&a,const Variable&b){return cmp(a.a,b.a);}
	struct DefaultableVariable:Variable{
		Location deflt;bool deflted=0;
	};
	struct Scope{
		Dicto<string,_Variable,uint16_t,16,cmp>vars;
		Scope*parent;inline Scope(Scope*p){parent=p;}inline Scope():Scope(0){}
		inline _Variable*get(string n)const{auto z=vars.get(n);if(z)return z;if(!parent)return 0;return parent->get(n);}
		inline void set(string n,_Variable z){vars.set(n,z);}
		void operator+=(Type*t);void operator+=(Type&t);
	};
}