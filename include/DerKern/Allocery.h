#pragma once
#include<new>
#include<string>
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

//https://stackoverflow.com/questions/537303/binary-literals/538101#538101

namespace DerKern{
	namespace Allocery{
		struct Allo{
			Allo*next=0;
			virtual void*valloc(uint32_t)=0;
		};struct AlloLoggy{inline AlloLoggy(Allo*z){next=z;}inline void*alloc(uint32_t s){auto _=valloc(s);printf("Allocated %lu bytes: %p",s,p);return _;}void*valloc(uint32_t)override;};
		struct Allo2:Allo{
			virtual void vfree(void*)=0;
		};struct Allo2Loggy{inline Allo2Loggy(Allo2*z){next=z;}inline void*alloc(uint32_t s){auto _=valloc(s);printf("Allocated %lu bytes: %p",s,p);return _;}inline void free(void*){auto _=vfree(s);printf("Freeing %p",s,p);return _;}void*valloc(uint32_t)override;void vfree(void*)override;};
		template<uint32_t size>struct StackLinear:Allo{
			uint32_t i;uint8_t raw[size];
			inline Linear(){i=0;}
			inline void*alloc(uint32_t s){
				if(size-i<s){if(!next)next=new Linear();return next.alloc(s);}
				auto _i=i;i+=s;return raw+(_i);
			}void*valloc(uint32_t s)override{return alloc(s);}
		}
		struct Linear:Allo{//who would've thought that I'll actually NEED linked lists... I still hate them though.
			uint32_t size,i;uint8_t*raw;
			inline Linear(uint32_t s=1024*16){i=0;raw=(uint8_t*)malloc(s);size=s;}
			inline Linear(void*r,uint32_t s){i=0;raw=r;size=s;}//you'll probably want to get rid of <raw> before the destructor comes
			inline void*alloc(uint32_t s){
				if(size-i<s){if(!next)next=new Linear();return next.alloc(s);}
				auto _i=i;i+=s;return raw+(_i);
			}void*valloc(uint32_t s)override;
			inline~Linear(){free(raw);delete next;}
		};
		struct BArr:Allo2{
			uint32_t size;uint8_t raw;
			uint32_t I[128],anew;uint16_t Ic;
			inline BArr(void*r,uint32_t s){anew=0;raw=r;size=s;}
			void*alloc(uint32_t s);void*valloc(uint32_t s)override;
			void free(void*);void*vfree(uint32_t s)override;
		};
		struct BArrA:BArr{
			inline BArrA(uint32_t s){anew=0;raw=(uint8_t*)malloc(s);size=s;}
			inline~BArrA(){free(raw);delete next;}			
		};
	}
	using std::string;
	template<typename _A,typename _B>struct pair{
		typedef _A A;typedef _B B;
		union{A a;A first;};union{B b;B second;};
		inline pair():a(),b(){}
		inline pair(A _a=A(),B _b=B()):a(_a),b(_b){}
		inline pair(const pair&p):a(p._a),b(p._b){}
		inline operator=(const pair&p){a=p.a;b=p.b;}
		inline ~pair(){a.~A();b.~B();}
		template<int(*_cmp)(const A&,const A&)=cmp>inline int cmpA(const pair&p){return _cmp(a,p.a);}
		template<int(*_cmp)(const B&,const B&)=cmp>inline int cmpB(const pair&p){return _cmp(b,p.b);}
	};
	template<typename T,typename sizeT,sizeT Exp,int(*cmp)(const T&,const T&)>struct SortList;
	template<typename T,typename sizeT=uint16_t,sizeT Exp=16>struct List{
		template<typename T,typename sizeT,sizeT Exp,int(*cmp)(const T&,const T&)>List(const SortList&)=delete;
		T*raw;sizeT size,count;
		inline List(){raw=0;size=count=0;}
		template<typename iT>inline T&operator[](iT i)const{return raw[(sizeT)i];}
		inline void ensure(sizeT c){//Ensures <c> elements can be placed with no expansions
			if(size<count+c){
				#ifdef FASTER
					sizeT exp=c-(c%Exp)+(c%Exp?Exp:0);
				#else
					sizet exp=c-(size-count);exp=exp-(exp%Exp)+(exp%Exp?Exp:0);
				#endif
				assert((size+exp)>=(size|exp));size+=exp;
				raw=(T*)realloc(raw,size*sizeof(T));
			}
		}
		inline T*append(T v){
			// if(size<=count){
			// 	assert((size+Exp)>=(size|Exp));size+=Exp;
			// 	raw=(T*)realloc(raw,size*sizeof(T));
			// }
			ensure(1);
			new(raw+count)T(v);count++;
			return raw+(count-1);
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
		inline void insert(T v,sizeT at){
			assert(count>=at);
			// if(size<=count){
			// 	assert((size+Exp)>=(size|Exp));size+=Exp;
			// 	raw=(T*)realloc(raw,size*sizeof(T));
			// }
			ensure(1);
			memmove(raw+at,raw+at+1,count-at);
			new(raw+at)T(v);count++;
		}
		inline void insert(T*v,sizeT at,sizeT c){
			assert(count>=at);
			// if(size<=count){
			// 	assert((size+Exp)>=(size|Exp));size+=Exp;
			// 	raw=(T*)realloc(raw,size*sizeof(T));
			// }
			ensure(c);
			memmove(raw+at,raw+at+c,count-at);
			for(sizeT i=0;i<c;i++)new(raw+i+at)T(v[i]);count+=c;
		}
		inline void death(){for(sizeT i=0;i<count;i++)raw[i].~T();free(raw);}
		inline T*operator+=(T z){return append(z);}
		inline List<T,sizeT,Exp> cpy()const{List _;_.append(raw,count);return _;}
		template<typename sizeT2=uint16_t,sizeT Exp2=16>inline void cpyTo(List<T,sizeT2,Exp2>&t)const{t.append(raw,count);}
		template<typename sizeT2=uint16_t,sizeT Exp2=16>inline void cpyFrom(List<T,sizeT2,Exp2>&t){append(t.raw,t.count);}
		template<typename sizeT2=uint16_t,sizeT Exp2=16>inline List<T,sizeT2,Exp2> cpyAs()const{List<T,sizeT2,Exp2>_;_.append(raw,count);return _;}
	};
	template<typename K,typename V,typename sizeT,sizeT Exp,int(*cmp)(const K&,const K&)>struct Dicto;
	typedef List<uint8_t,uint64_t,256>BBuf;
	template<typename T,typename sizeT=uint16_t,sizeT Exp=16,int(*cmp)(const T&,const T&)=cmp>struct SortList:List<T,sizeT,Exp>{
		template<typename K,typename V,typename sizeT,sizeT Exp,int(*cmp)(const K&,const K&)>SortList(const Dicto&)=delete;
		inline sizeT getI(const T&v)const{
			sizeT a=0,b,c=count-1;
			while(c>a){
				b=(a+c)>>1;
				int z=cmp(v,raw[b]);
				if(z==0)return b;
				if(z>0)a=b+1;
				if(z<0)c=b;
			}
			return a+1;
		}inline sizeT getI(const T&&v)const{return getI(v);}
		inline T*get(const T&v)const{
			sizeT a=0,b,c=count-1;
			while(c>a){
				b=(a+c)>>1;
				int z=cmp(v,raw[b]);
				if(z==0)return&raw[b];
				if(z>0)a=b+1;
				if(z<0)c=b;
			}
			return 0;
		}inline T*get(const T&&v)const{return get(v);}
		inline T&set(const T&v){
			sizeT a=0,b,c=count-1;
			while(c>a){
				b=(a+c)>>1;
				int z=cmp(v,raw[b]);
				if(z==0)return raw[b]=v;
				if(z>0)a=b+1;
				if(z<0)c=b;
			}
			insert(v,a);
		}inline T&set(const T&v){return set(v);}inline T&set(const K&&v){return set(v);}inline T&set(const T&&v){return set(v);}
		template<typename sizeT2=uint16_t,sizeT Exp2=16,int(*cmp2)(const T&,const T&)>inline void cpyTo(SortList<T,sizeT2,Exp2,cmp2>&t)const{for(sizeT i=0;i<count;i++)t.set(raw[i]);}
		template<typename sizeT2=uint16_t,sizeT Exp2=16,int(*cmp2)(const T&,const T&)>inline void cpyFrom(SortList<T,sizeT2,Exp2,cmp2>&t){for(sizeT i=0;i<t.count;i++)set(t.raw[i]);}
		template<typename sizeT2=uint16_t,sizeT Exp2=16,int(*cmp2)(const T&,const T&)>inline void cpyFrom(List<T,sizeT2,Exp2,cmp2>&t){for(sizeT i=0;i<t.count;i++)set(t.raw[i]);}
		operator List()=delete;
		protected:using List::cpyFrom;using List::append;using List::insert;using List::operator+=;
	};
	template<typename K,typename V,typename sizeT=uint16_t,sizeT Exp=16,int(*cmp)(const K&,const K&)=cmp>struct Dicto:SortList<pair<K,V>,sizeT,Exp,pair<K,V>::cmpA<cmp>>{
		inline sizeT getI(const K&k)const{
			sizeT a=0,b,c=count-1;
			while(c>a){
				b=(a+c)>>1;
				int z=cmp(k,raw[b].first);
				if(z==0)return b;
				if(z>0)a=b+1;
				if(z<0)c=b;
			}
			return a+1;
		}inline sizeT getI(const K&&k)const{return getI(k);}
		inline V*get(const K&k)const{
			sizeT a=0,b,c=count-1;
			while(c>a){
				b=(a+c)>>1;
				int z=cmp(k,raw[b].first);
				if(z==0)return&raw[b].second;
				if(z>0)a=b+1;
				if(z<0)c=b;
			}
			return 0;
		}inline V*get(const K&&k)const{return get(k);}
		inline V&set(const K&k,const V&v){
			sizeT a=0,b,c=count-1;
			while(c>a){
				b=(a+c)>>1;
				int z=cmp(k,raw[b].first);
				if(z==0)return raw[b].second=v;
				if(z>0)a=b+1;
				if(z<0)c=b;
			}
			insert({k,v},a);
		}inline V&set(const K&k,const V&&v){return set(k,v);}inline V&set(const K&&k,const V&v){return set(k,v);}inline V&set(const K&&k,const V&&v){return set(k,v);}
		inline V&operator[](const K&k)const{return*get(k);}inline V&operator[](const K&&k)const{return*get(k);}
		operator List()=delete;operator SortList()=delete;
	};
	struct Location{
		enum Types:uint8_t{
			undecided=0,//non-existent
			imovbl=1,//forever at pointer, maybe you don't want to get rid that place... just saying...
			reg=2,dReg=reg|imovbl//register/dereferenced register
		}type;
		int32_t imov;//if(type==1)v=*(*)(uint32_t)imov;if(type==dReg)v=*((*)reg+imov)
		uint8_t reg;//register
		inline operator uint8_t(){return type&4?reg.r:(uint8_t)-1;}
		inline Location(){type=0;}
		inline Location(int32_t o){type=1;imov=o;}
		inline Location(uint8_t r){type=2;reg=r;}
		inline Location(uint8_t r,int32_t o){type=2;reg=r;imov=o;}
	};
	typedef Location FLocation;
	struct _Variable;struct Variable;struct TypePtr;
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
		TypePtr type;
		union{
			Location val;
			uint64_t
		};
		inline _Variable():type(0),val(){}
		inline _Variable(Type t,Location v):type(t),val(v){}
	};
	struct Variable:_Variable{
		string name;
		inline Variable(_Variable z,string n):name(n){*(_Variable*)this=z;}
		inline Variable(TypePtr t,Location v,string n):name(n),type(t),val(v){}
	};
}