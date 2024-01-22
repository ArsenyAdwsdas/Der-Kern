#pragma once
#include"StandloTypes.h"
namespace DerKern{
	inline uint32_t ceilSize(uint32_t a,uint32_t m){return a-(a%m)+(a%m?m-(a%m):0);}
	template<uint8_t bytes>struct ByteBooly{
		uint8_t raw[bytes];
		inline bool operator()(uint8_t N)const{return raw[N>>3]&(1<<(N&7));}
		inline bool operator()(uint8_t N,bool val){uint8_t&w=raw[N>>3];N=N&7;w=(w&(~(1<<N)))|((val?1:0)<<N);return val;}
		template<uint8_t n>inline operator ByteBooly<n>()const{ByteBooly<n>_;uint8_t i=0;if(n>bytes)for(;i<bytes;i++)_.raw[i]=raw[i];else for(;i<n;i++)_.raw[i]=raw[i];return _;}
		inline ByteBooly operator~()const{ByteBooly _=*this;uint8_t i=0;for(;i<bytes;i+=8)*(uint64_t*)(_.raw+i)=~*(uint64_t*)(raw+i);if(bytes&4){*(uint32_t*)(_.raw+i)=~*(uint32_t*)(raw+i);i+=4;}if(bytes&2){*(uint16_t*)(_.raw+i)=~*(uint16_t*)(raw+i);i+=2;}if(bytes&1)*(uint8_t*)(_.raw+i)=~*(uint8_t*)(raw+i);return _;}
		inline ByteBooly operator&(ByteBooly b)const{ByteBooly _=*this;uint8_t i=0;for(;i<bytes;i+=8)*(uint64_t*)(_.raw+i)=*(uint64_t*)(raw+i)&*(uint64_t*)(b.raw+i);if(bytes&4){*(uint32_t*)(_.raw+i)=~*(uint32_t*)(raw+i)&*(uint32_t*)(b.raw+i);i+=4;}if(bytes&2){*(uint16_t*)(_.raw+i)=~*(uint16_t*)(raw+i)&*(uint16_t*)(b.raw+i);i+=2;}if(bytes&1)*(uint8_t*)(_.raw+i)=~*(uint8_t*)(raw+i)&*(uint8_t*)(b.raw+i);return _;}
		inline ByteBooly operator|(ByteBooly b)const{ByteBooly _=*this;uint8_t i=0;for(;i<bytes;i+=8)*(uint64_t*)(_.raw+i)=*(uint64_t*)(raw+i)|*(uint64_t*)(b.raw+i);if(bytes&4){*(uint32_t*)(_.raw+i)=~*(uint32_t*)(raw+i)|*(uint32_t*)(b.raw+i);i+=4;}if(bytes&2){*(uint16_t*)(_.raw+i)=~*(uint16_t*)(raw+i)|*(uint16_t*)(b.raw+i);i+=2;}if(bytes&1)*(uint8_t*)(_.raw+i)=~*(uint8_t*)(raw+i)|*(uint8_t*)(b.raw+i);return _;}
		inline ByteBooly operator^(ByteBooly b)const{ByteBooly _=*this;uint8_t i=0;for(;i<bytes;i+=8)*(uint64_t*)(_.raw+i)=*(uint64_t*)(raw+i)^*(uint64_t*)(b.raw+i);if(bytes&4){*(uint32_t*)(_.raw+i)=~*(uint32_t*)(raw+i)^*(uint32_t*)(b.raw+i);i+=4;}if(bytes&2){*(uint16_t*)(_.raw+i)=~*(uint16_t*)(raw+i)^*(uint16_t*)(b.raw+i);i+=2;}if(bytes&1)*(uint8_t*)(_.raw+i)=~*(uint8_t*)(raw+i)^*(uint8_t*)(b.raw+i);return _;}
		inline void operator&=(ByteBooly b){*this=b&*this;}
		inline void operator|=(ByteBooly b){*this=b|*this;}
		inline void operator^=(ByteBooly b){*this=b^*this;}

		inline ByteBooly(){for(uint8_t i=0;i<bytes;i++)raw[i]=0;}
		inline ByteBooly(uint8_t a){raw[0]=a;for(uint8_t i=1;i<bytes;i++)raw[i]=0;}
		inline ByteBooly(uint8_t a,uint8_t b){raw[0]=a;raw[1]=b;for(uint8_t i=2;i<bytes;i++)raw[i]=0;}
		inline ByteBooly(uint8_t a,uint8_t b,uint8_t c){raw[0]=a;raw[1]=b;raw[2]=c;for(uint8_t i=3;i<bytes;i++)raw[i]=0;}
		inline ByteBooly(uint8_t a,uint8_t b,uint8_t c,uint8_t d){raw[0]=a;raw[1]=b;raw[2]=c;raw[3]=c;for(uint8_t i=4;i<bytes;i++)raw[i]=0;}
	};
	struct RegisterState{
		#if _WIN64||__x86_64__||__ppc64__
			typedef uint64_t regT;
			typedef uint16_t bits;
		#else
			typedef uint32_t regT;
			typedef uint8_t bits;
		#endif
		regT raw[sizeof(int*)==4?8:16]={0};
		template<typename t>inline constexpr regT&operator[](t r){return raw[r];}
		inline constexpr static RegisterState Nulled(){RegisterState w;for(uint8_t i=0;i<(sizeof(int*)<<1);i++)w[i]=0;return w;}
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
			offset=1,//a dereference... of something... maybe...
			reg=2,dReg=reg|offset,//register/dereferenced register
			ptr=4,//will probably be mostly very unusable...
			stacky=ptr|offset,
			ref=ptr|dReg,//will act as *(Location*)ptr
			st=8,xmm=st|reg//ancient and new registers
			,dDReg=dReg|8//an eldritch horror that should be disposed of at some point, I guess?
			ENUM_END uint8_t type;
		union{
			struct{
				int32_t imov;//v=*((*)reg+imov)
				uint8_t reg;//register
			};
			void*ptr;
		};
		inline const Location&resolve()const{auto _=this;while(_->type==7)_=(Location*)_->ptr;return*_;}
		inline Location&resolve(){return(Location&)((const Location*)this)->resolve();}//Yes, a const_cast. SO WHAT, GONNA NAG AND SAY IT'S BAD?
		/*inline void*eval(RegisterState s)const{
			//if(type==1)return(void*)(int64_t)imov;
			if(type==2)return&s[reg];
			if(type==3)return*(void**)(s[reg]+imov);
			if(type==4)return ptr;
			return 0;
		}inline void*eval()const{
			//if(type==1)return(void*)(int64_t)imov;
			if(type==4)return ptr;
			return 0;
		}*/
		inline Location(){type=0;}
		//inline Location(int32_t o){type=1;imov=o;}
		inline Location(uint8_t r){type=2;reg=r;}
		inline Location(uint8_t r,int32_t o){type=3;reg=r;imov=o;}
		inline Location(void*p){type=4;ptr=p;}
		inline Location(Location*p){type=7;ptr=p;}
		inline bool operator==(Location z)const{
			if(type!=z.type)return 0;
			if(type==1)return imov==z.imov;
			if(type==2)return reg==z.reg;
			if(type==3)return reg==z.reg&&imov==z.imov;
			if(type==5)return imov==z.imov;
			if(type==4||type==7)return ptr==z.ptr;
			if(type==11)return imov==z.imov;
			return 0;
		}inline bool operator!=(Location z)const{return!(z==*this);}
		inline operator bool()const{return type;}inline bool operator!()const{return!type;}
		/**@brief For defaulting**/
		inline Location operator||(Location w)const{return *this?*this:w;}inline Location operator||(uint8_t w)const{return *this?*this:(Location)w;}
		inline static Location Reg(uint8_t r){return r;}
		inline static Location dReggy(uint8_t reg,bool deref){return deref?Location(reg,0):(Location)reg;}inline bool isDReg()const{return type==3||type==5;}
		inline static Location stack(int32_t o){return{(uint8_t)4,o};}
		inline static Location dStack(int32_t o){Location _;_.reg=4;_.type=11;_.imov=o;return _;}
		inline static Location loca(int32_t o){Location _;_.type=5;_.imov=o;return _;}
		inline static Location st(uint8_t i){Location _;_.type=8;_.reg=i;return _;}
		inline static Location xmm(uint8_t i){Location _;_.type=10;_.reg=i;return _;}
		template<typename t>inline Location operator+(t o)const{Location _=*this;_.imov+=type!=5?o:-o;return _;}
		//template<typename t>inline operator t*()const{return(t*)resolve().eval();}
		/**@brief it returns the bit of the register... but only when Location directly points to it and not something like "dreg".**/
		inline RegisterState::bits regDireBit()const{return type==2?(1<<reg):0;}
		/**@brief returns the bits of all the registers involved**/
		inline RegisterState::bits regBits()const{return type==2?(1<<reg):type==3?(1<<reg):type==5?16:0;}//
	};
	typedef Location FLocation;
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
	struct Type;
	typedef pair<Type*,Location>_Variable;
	typedef pair<string,_Variable>Variable;inline int cmp(const Variable&a,const Variable&b){return cmp(a.a,b.a);}
	struct Scope{
		Dicto<string,_Variable,uint16_t,16,cmp>vars;
		Scope*parent;inline Scope(Scope*p){parent=p;}inline Scope():Scope(0){}
		inline _Variable*get(string n)const{auto z=vars.get(n);if(z)return z;if(!parent)return 0;return parent->get(n);}
		inline void set(string n,_Variable z){vars.set(n,z);}
		void operator+=(Type*t);void operator+=(Type&t);
	};
}