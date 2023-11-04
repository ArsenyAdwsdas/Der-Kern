#pragma once
#include"../include/DerKern/StandloTypes.h"
namespace DerKern{
	int cmp(const string&a,const string&b){auto _a=a.size(),_b=b.size(),w=_a>_b?_b:_a;int _=memcmp(a.data(),b.data(),w);if(_!=0)return-_;return _a<_b?-1:_a==_b?0:1;}
	int cmp(const int&a,const int&b){return a>b?-1:a==b?0:1;}
	int cmp(const uint8_t&a,const uint8_t&b){return a>b?-1:a==b?0:1;}
	int cmp(const uint32_t&a,const uint32_t&b){return a>b?-1:a==b?0:1;}
	namespace Allocery{
		void*AlloLoggy::valloc(uint32_t s){return alloc(s);}
		void*Allo2Loggy::valloc(uint32_t s){return alloc(s);}void Allo2Loggy::vfree(void*_){return free(_);}
		void*Linear::valloc(uint32_t s){return alloc(s);}void*LinearExe::valloc(uint32_t s){return alloc(s);}
		void*BArr::alloc(uint32_t s){
			void*_;
			if(_=ANEW(s))return _;
			if(_=REUSE(s))return _;
			if(!next)next=new BArr(DEFAULT_SIZE);
			return ((BArr*)next)->alloc(s);
		}
		void BArr::free(void*_z){
			if(!_z)return;
			if(uintptr_t(_z)<uintptr_t(raw)||uintptr_t(_z)>uintptr_t(raw)+size){((Allo2*)next)->vfree(_z);return;}
			uint32_t siz;
			auto z=(uint8_t*)((uintptr_t)_z-sizeof(siz));auto o=(uintptr_t)z-(uintptr_t)raw;
			siz=*(decltype(siz)*)z;
			if(o+sizeof(siz)+siz==anew){anew=o;for(uint16_t i=0;i<(sizeof(I)/sizeof(*I));i++)if(I[i]==anew)I[i]=-1;}
			else{
				decltype(siz)siz2,siz3;
				for(uint16_t i0=0;i0<(sizeof(I)/sizeof(*I));i0++)if(I[i0]+sizeof(siz)+(siz2=*(decltype(siz)*)(raw+I[i0]))==o){//find one Before to merge with
					z=raw+I[i0];
					I[i0]=o;siz+=siz2;
					for(uint16_t i=i0;i<(sizeof(I)/sizeof(*I));i++){//look for more to merge
						if(o+sizeof(siz)+(siz3=*(decltype(siz)*)(raw+I[i0]))==I[i]){I[i]=-1;goto found1;}
					}
					*(decltype(siz)*)z=siz;return;//failed to find another one
					found1:*(decltype(siz)*)z=siz+siz3;return;
				}else if(o+sizeof(siz)+siz==I[i0]){//find one After to merge with
					I[i0]=o;siz+=siz2;
					for(uint16_t i=i0;i<(sizeof(I)/sizeof(*I));i++){//look for more to merge
						if(I[i]+sizeof(siz)+(siz3=*(decltype(siz)*)(raw+I[i]))==o){z=raw+I[i];I[i]=-1;goto found2;}
					}
					*(decltype(siz)*)z=siz;return;//failed to find another one
					found2:*(decltype(siz)*)z=siz+siz3;return;
				}else if(I[i0]==(uint32_t)-1){//found backup in case the search fails //I could've done it without copying the search except I'm obsessed with optimizing...
					for(i0++;i0<(sizeof(I)/sizeof(*I));i0++)if(I[i0]+sizeof(siz)+(siz2=*(decltype(siz)*)(raw+I[i0]))==o){//find one Before to merge with //this loop takes over completely
						z=raw+I[i0];
						I[i0]=o;siz+=siz2;
						for(uint16_t i=i0;i<(sizeof(I)/sizeof(*I));i++){//look for more to merge
							if(o+sizeof(siz)+(siz3=*(decltype(siz)*)(raw+I[i0]))==I[i]){I[i]=-1;goto found3;}
						}
						*(decltype(siz)*)z=siz;return;//failed to find another one
						found3:*(decltype(siz)*)z=siz+siz3;return;
					}else if(o+sizeof(siz)+siz==I[i0]){//find one After to merge with
						I[i0]=o;siz+=siz2;
						for(uint16_t i=i0;i<(sizeof(I)/sizeof(*I));i++){//look for more to merge
							if(I[i]+sizeof(siz)+(siz3=*(decltype(siz)*)(raw+I[i]))==o){z=raw+I[i];I[i]=-1;goto found4;}
						}
						*(decltype(siz)*)z=siz;return;//failed to find another one
						found4:*(decltype(siz)*)z=siz+siz3;return;
					}
					I[i0]=o;return;
				}
				while(1)printf("!!!BArr overfreeing!!!");//Your fault for freeing too much.
			}
		}void*BArr::valloc(uint32_t s){return alloc(s);}void BArr::vfree(void*_){return free(_);}
		void*BArrExe::alloc(uint32_t s){
			void*_;
			if(_=ANEW(s))return _;
			if(_=REUSE(s))return _;
			if(!next)next=new BArrExe(DEFAULT_SIZE);
			return ((BArrExe*)next)->alloc(s);
		}void*BArrExe::valloc(uint32_t s){return alloc(s);}
	}
}