#pragma once //STAND aLOne TYPES
#include<assert.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<new>
//https://stackoverflow.com/a/2390626
#ifdef __cplusplus
	#define INITIALIZER(f) \
		static void f(void); \
		struct f##_t_ { f##_t_(void) { f(); } }; static f##_t_ f##_; \
		static void f(void)
#elif defined(_MSC_VER)
	#pragma section(".CRT$XCU",read)
	#define INITIALIZER2_(f,p) \
		static void f(void); \
		__declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
		__pragma(comment(linker,"/include:" p #f "_")) \
		static void f(void)
	#ifdef _WIN64
		#define INITIALIZER(f) INITIALIZER2_(f,"")
	#else
		#define INITIALIZER(f) INITIALIZER2_(f,"_")
	#endif
#else
	#define INITIALIZER(f) \
		static void f(void) __attribute__((constructor)); \
		static void f(void)
#endif
//Blame C++ standards for not allowing to use "enum class" as just "uint8_t". // https://stackoverflow.com/questions/8357240/how-to-automatically-convert-strongly-typed-enum-into-int
//https://stackoverflow.com/a/13273876 gave me bravery to dare use "class" instead of "namespace"
// //"I wanna make a macro for it... OH?.. I'M ALLOWED TO FORWARD-DECLARE ENUMS?.. GOOD. I'LL DO THAT." // https://stackoverflow.com/questions/71416/forward-declaring-an-enum-in-c
//Ended up finally checking. Bye "=0"! // https://stackoverflow.com/a/34811588 (fortunately by default the first enum value has to be 0)
//Turned out that forward declarations are useless to me here... https://stackoverflow.com/a/1021809
//VSC sometimes shows me non-existent errors due to those... Eh, not the worst I've had. I've written lots of stuff in 1 line of Lua... Back when I was making my mods for Factorio... Like 3 years ago...
#define ENUM(name,type) class name{name();public:enum T:type{
#define ENUM_END };};//I do hate python syntax... But this seems to be the most readable way to use these macros... Since when do I care about it though?..

//https://stackoverflow.com/questions/537303/binary-literals/538101#538101
template<char...digits>struct conv2bin;
template<char high, char... digits>struct conv2bin<high, digits...>{
    static_assert(high == '0' || high == '1', "no bin num!");
    enum z:int64_t{value=(high-'0')*(1<<sizeof...(digits))+conv2bin<digits...>::value};
};
template<char high>struct conv2bin<high>{
    static_assert(high == '0' || high == '1', "no bin num!");
    enum z:int64_t{value=(high - '0')};
};
template<char...digits>constexpr int64_t operator "" _b(){return conv2bin<digits...>::value;}

namespace DerKern{
	using std::string;int cmp(const string&,const string&);int cmp(const int&,const int&);
	int cmp(const uint8_t&a,const uint8_t&b);int cmp(const uint32_t&a,const uint32_t&b);
	template<typename A,typename B,int(*cmp)(const A&,const B&)>inline int cmpR(const A&a,const B&b){return-cmp(a,b);}//to reverse the order.
	template<typename _A,typename _B>struct pair{
		typedef _A A;typedef _B B;
		union{A a;A first;};union{B b;B second;};
		inline pair():a(),b(){}
		inline pair(A _a=A(),B _b=B()):a(_a),b(_b){}
		inline pair(const pair&p):a(p.a),b(p.b){}
		inline pair&operator=(const pair&p){a=p.a;b=p.b;return*this;}
		inline ~pair(){a.~A();b.~B();}
	};
	//they're not "template<int(*_cmp)(const A&,const A&)=cmp>static" in "struct pair" because apparently "pair<K,V>::cmpA<_cmp>" causes "expected '{' before '>' token" error at the 2nd ">"
	template<typename A,typename B,int(*_cmp)(const A&,const A&)=cmp>inline int cmpA(const pair<A,B>&a,const pair<A,B>&b){return _cmp(a.a,b.a);}
	template<typename A,typename B,int(*_cmp)(const B&,const B&)=cmp>inline int cmpB(const pair<A,B>&a,const pair<A,B>&b){return _cmp(a.b,b.b);}
	template<typename A,typename B,int(*_cmp)(const A&,const A&)=cmp>inline int cmpA2(const pair<A,B>&a,const A&b){return _cmp(a.a,b);}
	template<typename A,typename B,int(*_cmp)(const B&,const B&)=cmp>inline int cmpB2(const pair<A,B>&a,const B&b){return _cmp(a.b,b);}
	template<typename A,typename B,int(*_cmp)(const A&,const A&)=cmp>inline int cmpA3(const A&b,const pair<A,B>&a){return _cmp(b,a.a);}
	template<typename A,typename B,int(*_cmp)(const B&,const B&)=cmp>inline int cmpB3(const B&b,const pair<A,B>&a){return _cmp(b,a.b);}

	//a=startIndex, b is the index where the result of the search is(or should be), c=endIndex, z(T*)=array to search in with <cmp>, v(T2)=what to search
	//returns 1 when successfuly found, else 0
	template<typename T,typename T2,int(*cmp)(const T&,const T2&),typename S>inline bool BinSearch(S a,S&b,S c,const T*z,const T2&v){
		if(c>99999)printf("!!!BinSearch <c> seems too big!!!\n");//an extra sanity check to be a bit more sure I don't try "c=count-1" again
		while(c>a){
			b=(a+c)>>1;
			int _=cmp(z[b],v);
			if(_==0){return 1;}
			else if(_>0)a=b+1;else if(_<0)c=b;
		}
		b=a;return 0;
	}
	// template<typename T,typename T2,int(*cmp)(const T&,const T&),typename S>inline bool BinSearch(S a,S&b,S c,const T*z,const T2&&v){return BinSearch<T,T2,cmp,S>(a,b,c,z,(const T2&)v);}
	// template<typename T,int(*cmp)(const T&,const T&),typename S>inline bool BinSearch(S a,S&b,S c,const T*z,const T&v){return BinSearch<T,T,cmp,S>(a,b,c,z,v);}
	// template<typename T,int(*cmp)(const T&,const T&),typename S>inline bool BinSearch(S a,S&b,S c,const T*z,const T&&v){return BinSearch<T,T,cmp,S>(a,b,c,z,v);}

	template<typename T>inline T*mallocCpy(T*a,size_t c){T*_=(T*)malloc(sizeof(T)*c);memcpy(_,a,sizeof(T)*c);return _;}

	template<typename T,typename sizeT,sizeT Exp,int(*cmp)(const T&,const T&)>struct SortList;
	template<typename T,typename sizeT=uint16_t,sizeT Exp=16>struct List{
		template<typename _T,typename _sizeT,sizeT _Exp,int(*_cmp)(const _T&,const _T&)>List(const SortList<_T,_sizeT,_Exp,_cmp>&)=delete;
		T*raw;sizeT size,count;
		inline List(){raw=0;size=count=0;}
		template<typename iT>inline T&operator[](iT i)const{return raw[(sizeT)i];}
		inline void ensure(sizeT c){//Ensures <c> elements can be placed with no manual expansions
			if(size<count+c){
				if(!Exp){assert(Exp!=0);printf("!!!List.ensure used when it ain't available!!!\n");while(1);}
				#ifdef FASTER
					sizeT exp=c-(c%Exp)+(c%Exp?Exp:0);
				#else
					sizeT exp=c-(size-count);exp=exp-(exp%Exp)+(exp%Exp?Exp:0);
				#endif
				assert((size+exp)>=(size|exp));size+=exp;
				raw=(T*)realloc(raw,size*sizeof(T));
			}
		}
		inline T&append(T v){
			// if(size<=count){
			// 	assert((size+Exp)>=(size|Exp));size+=Exp;
			// 	raw=(T*)realloc(raw,size*sizeof(T));
			// }
			ensure(1);
			new(raw+count)T(v);count++;
			return raw[count-1];
		}
		inline T*append(T*v,sizeT c){
			// if(size<count+c){
			// 	#ifdef FASTER
			// 		sizeT exp=c-(c%Exp)+(c%Exp?Exp:0);
			// 	#else
			// 		sizet exp=c-(size-count);exp=exp-(exp%Exp)+(exp%Exp?Exp:0);
			// 	#endif
			// 	assert((size+exp)>=(size|exp));size+=exp;
			// 	raw=(T*)realloc(raw,size*sizeof(T));
			// }
			ensure(c);
			for(sizeT i=0;i<c;i++)new(raw+i+count)T(v[i]);count+=c;
			return raw+(count-c);
		}
		inline T&insert(T v,sizeT at){
			if(at>count){printf("insert at %u\n",unsigned(at));assert(0);}
			// if(size<=count){
			// 	assert((size+Exp)>=(size|Exp));size+=Exp;
			// 	raw=(T*)realloc(raw,size*sizeof(T));
			// }
			ensure(1);
			memmove(raw+at+1,raw+at,(count-at)*sizeof(T));//A https://youtu.be/VaMR_xDhsGg?t=61 moment was here... Forgot to multiply by sizeof somehow after SO MANY times I've written such stuff already...
			new(raw+at)T(v);count++;
			return raw[at];
		}
		inline T*insert(T*v,sizeT at,sizeT c){
			if(at>count){printf("insert at %u\n",unsigned(at));assert(0);}
			// if(size<=count){
			// 	assert((size+Exp)>=(size|Exp));size+=Exp;
			// 	raw=(T*)realloc(raw,size*sizeof(T));
			// }
			ensure(c);
			memmove(raw+at+c,raw+at,(count-at)*sizeof(T));
			for(sizeT i=0;i<c;i++)new(raw+i+at)T(v[i]);count+=c;
			return raw+at;
		}
		inline void death(){for(sizeT i=0;i<count;i++)raw[i].~T();free(raw);}
		inline T&operator+=(T z){return append(z);}
		inline List<T,sizeT,Exp> cpy()const{List _;_.append(raw,count);return _;}
		template<typename sizeT2=uint16_t,sizeT Exp2=16>inline void cpyTo(List<T,sizeT2,Exp2>&t)const{t.append(raw,count);}
		template<typename sizeT2=uint16_t,sizeT Exp2=16>inline void cpyFrom(List<T,sizeT2,Exp2>&t){append(t.raw,t.count);}
		template<typename sizeT2=uint16_t,sizeT Exp2=16>inline List<T,sizeT2,Exp2> cpyAs()const{List<T,sizeT2,Exp2>_;_.append(raw,count);return _;}
	};
	template<typename K,typename V,typename sizeT,sizeT Exp,int(*cmp)(const K&,const K&)>struct Dicto;
	typedef List<uint8_t,uint64_t,256>BBuf;
	template<typename T,typename sizeT,sizeT Exp,int(*_cmp)(const T&,const T&)>struct SortList:List<T,sizeT,Exp>{
		inline SortList(){}
		template<typename _K,typename _V,typename _sizeT,sizeT _Exp,int(*cmp)(const _K&,const _K&)>SortList(const Dicto<_K,_V,_sizeT,_Exp,cmp>&)=delete;
		inline sizeT getI(const T&v)const{sizeT b;BinSearch<T,T,_cmp,sizeT>(0,b,this->count-1,this->raw,v);return b;}
		inline sizeT getI(const T&&v)const{return getI(v);}
		template<typename T2,int(*Cmp)(const T&,const T2&)=cmp>inline sizeT getI(const T2&v)const{sizeT b;BinSearch<T,T2,Cmp,sizeT>(0,b,this->count,this->raw,v);return b;}
		template<typename T2,int(*Cmp)(const T&,const T2&)=cmp>inline sizeT getI(const T2&&v)const{return getI<T2,Cmp>(v);}

		inline T*get(const T&v)const{sizeT b;if(BinSearch<T,T,_cmp,sizeT>(0,b,this->count,this->raw,v))return&this->raw[b];return 0;}
		inline T*get(const T&&v)const{return get(v);}
		template<typename T2,int(*Cmp)(const T&,const T2&)=cmp>inline T*get(const T2&v)const{sizeT b;if(BinSearch<T,T2,Cmp,sizeT>(0,b,this->count,this->raw,v))return&this->raw[b];return 0;}
		template<typename T2,int(*Cmp)(const T&,const T2&)=cmp>inline T*get(const T2&&v)const{return get<T2,Cmp>(v);}

		inline T&set(const T&v){sizeT b;if(BinSearch<T,T,_cmp,sizeT>(0,b,this->count,this->raw,v))return this->raw[b]=v;return insert(v,b);}
		inline T&set(const T&&v){return set(v);}
		template<typename T2,int(*Cmp)(const T&,const T2&)=cmp>inline T&setKV(const T2&k,const T&v){sizeT b;if(BinSearch<T,T2,Cmp,sizeT>(0,b,this->count,this->raw,k))return this->raw[b]=v;return insert(v,b);}
		//template<typename T2,int(*Cmp)(const T&,const T2&)=cmp>inline T&setKV(const T2&k,const T&&v){sizeT b;if(BinSearch<T,T2,Cmp,sizeT>(0,b,this->count,this->raw,k))return this->raw[b]=v;return insert(v,b);}
		// template<typename T2,int(*Cmp)(const T2&,const T2&)=cmp>inline T&setKV(const T2&k,const T&&v){return setKV<T2,Cmp>(k,v);}
		// template<typename T2,int(*Cmp)(const T2&,const T2&)=cmp>inline T&setKV(const T2&&k,const T&v){return setKV<T2,Cmp>(k,v);}
		template<typename T2,int(*Cmp)(const T&,const T2&)=cmp>inline T&setKV(const T2&&k,const T&&v){return setKV<T2,Cmp>(k,v);}

		inline T&_insert(T v,sizeT at){return insert(v,at);};inline T*_insert(T*v,sizeT at,sizeT c){return insert(v,at,c);};
		template<typename sizeT2=uint16_t,sizeT Exp2=16,int(*cmp2)(const T&,const T&)>inline void cpyTo(SortList<T,sizeT2,Exp2,cmp2>&t)const{for(sizeT i=0;i<this->count;i++)t.set(this->raw[i]);}
		template<typename sizeT2=uint16_t,sizeT Exp2=16,int(*cmp2)(const T&,const T&)>inline void cpyFrom(SortList<T,sizeT2,Exp2,cmp2>&t){for(sizeT i=0;i<t.count;i++)set(t.raw[i]);}
		template<typename sizeT2=uint16_t,sizeT Exp2=16>inline void cpyFrom(List<T,sizeT2,Exp2>&t){for(sizeT i=0;i<t.count;i++)set(t.raw[i]);}
		operator List<T,sizeT,Exp>()=delete;
		protected:using List<T,sizeT,Exp>::cpyFrom;using List<T,sizeT,Exp>::append;using List<T,sizeT,Exp>::insert;using List<T,sizeT,Exp>::operator+=;
	};
	template<typename K,typename V,typename sizeT,sizeT Exp,int(*_cmp)(const K&,const K&)>struct Dicto:SortList<pair<K,V>,sizeT,Exp,cmpA<K,V,_cmp>>{
		inline Dicto(){}
		//using SortList<pair<K,V>,sizeT,Exp,cmpA<K,V,_cmp>>::getI<K,cmpA<K,V,_cmp>>;using SortList<pair<K,V>,sizeT,Exp,cmpA<K,V,_cmp>>::get<K,cmpA<K,V,_cmp>>;
		//using SortList<pair<K,V>,sizeT,Exp,cmpA<K,V,_cmp>>::set<pair<K,V>,cmpA<K,V,_cmp>>;using SortList<pair<K,V>,sizeT,Exp,cmpA<K,V,_cmp>>::setKV<K,cmpA<K,V,_cmp>>;
		/*inline V*get(const K&k)const{
			sizeT a=0,b,c=count-1;
			while(c>a){
				b=(a+c)>>1;
				int z=cmp(k,raw[b].first);
				if(z==0)return&raw[b].second;
				else if(z>0)a=b+1;if(z<0)c=b;
			}
			return 0;
		}inline V*get(const K&&k)const{return get(k);}*/
		inline V&set(const K&k,const V&v){sizeT b;if(BinSearch<pair<K,V>,K,cmpA2<K,V,_cmp>,sizeT>(0,b,this->count,this->raw,k))return this->raw[b].b=v;return this->_insert({k,v},b).b;}
		inline V&set(const K&k,const V&&v){return set(k,v);}inline V&set(const K&&k,const V&v){return set(k,v);}inline V&set(const K&&k,const V&&v){return set(k,v);}

		inline V*get(const K&k)const{sizeT b;if(BinSearch<pair<K,V>,K,cmpA2<K,V,_cmp>,sizeT>(0,b,this->count,this->raw,k))return&this->raw[b].b;return 0;}
		inline V*get(const K&&k)const{return get(k);}

		inline V&operator[](const K&k)const{return*get(k);}inline V&operator[](const K&&k)const{return*get(k);}
	};
	namespace Allocery{
		struct Allo{
			Allo*next=0;
			virtual void*valloc(uint32_t)=0;
		};struct AlloLoggy:Allo{inline AlloLoggy(Allo*z){next=z;}inline void*alloc(uint32_t s){auto _=valloc(s);printf("Allocated %lu bytes: %p",s,_);return _;}void*valloc(uint32_t)override;};
		struct Allo2:Allo{
			virtual void vfree(void*)=0;
		};struct Allo2Loggy:Allo2{inline Allo2Loggy(Allo2*z){next=z;}inline void*alloc(uint32_t s){auto _=valloc(s);printf("Allocated %lu bytes: %p",s,_);return _;}inline void free(void*_){vfree(_);printf("Freed %p",_);}void*valloc(uint32_t)override;void vfree(void*)override;};
		struct Linear:Allo,List<uint8_t,uint32_t,0>{//who would've thought that I'll actually NEED linked lists... I still hate them though.
			inline Linear(void*r,uint32_t s):List(){size=s;raw=(uint8_t*)r;}//you'll probably want to get rid of <raw> before the destructor comes
			inline Linear(uint32_t s):Linear(malloc(s),s){}
			inline Linear():Linear(16*1024){}
			inline void*alloc(uint32_t s){
				if(size-count>s){count+=s;return raw+(count-s);}else{return (next?next:next=new Linear())->valloc(s);}
			}void*valloc(uint32_t s)override;
			inline~Linear(){free(raw);delete next;}
		};
		template<uint32_t size>struct AlloclessLinear:Allo{
			uint32_t i;uint8_t raw[size];
			inline AlloclessLinear(){i=0;}
			inline void*alloc(uint32_t s){
				if(size-i<s){if(!next)next=new Linear();return next->valloc(s);}
				auto _i=i;i+=s;return raw+(_i);
			}void*valloc(uint32_t s)override{return alloc(s);}
		};
		struct BArr:Allo2{
			constexpr static uint32_t DEFAULT_SIZE=32*1024;
			uint32_t size;uint8_t*raw;
			uint32_t I[128],anew;uint16_t Ic;
			inline BArr(void*r,uint32_t s){anew=0;raw=(uint8_t*)r;size=s;}
			inline BArr(uint32_t s):BArr(malloc(s),s){}
			void*alloc(uint32_t s);void*valloc(uint32_t s)override;
			void free(void*);void vfree(void*)override;
		};
		struct BArrA:BArr{
			inline BArrA(uint32_t s):BArr(malloc(s),s){}
			inline~BArrA(){free(raw);delete next;}
		};
	}
}