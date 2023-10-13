#include"../include/DerKern/Standlo.h"
namespace DerKern{
	int cmp(const string&a,const string&b){return a.compare(b);}
	namespace Allocery{
		void*AlloLoggy::valloc(uint32_t s){return alloc(s);}
		void*AlloLoggy2::valloc(uint32_t s){return alloc(s);}void AlloLoggy2::vfree(void*_){return free(_);}
		void*StackLinear::valloc(uint32_t s){return alloc(s);}
		void*Linear::valloc(uint32_t s){return alloc(s);}
		void*BArr::alloc(uint32_t s){
			//reuse
			for(uint32_t i=0;i<(sizeof(I)/sizeof(*I));i++)if(I[i]!=(uint32_t)-1){
				auto siz=*(uint16_t*)(raw+I[i]);if(s+2>siz);else{
					auto raw+_=I[i];I[i]=-1;
					if(siz-s>1){
						*(uint16_t*)_=s;
						*(uint16_t*)(_+2+s)=siz-s;
					}else*(uint16_t*)_=siz;
					return _+2;
				}
			}
			if(anew!=(uint32_t)-1){
				if(size-anew>=s+2){auto _=raw+anew;anew+=2+s;*(uint16_t*)(_)=s;return _+2;}else anew=-1;
			}
			if(!next)next=new BArr();
			return next->alloc(s);
		}
		void BArr::free(void*_z){
			uint8_t*z=(uint8_t*)_z;
			auto size=*(uint16_t*)z;
			if(z+2+size==anew)anew=((uint64_t)z-(uint64_t)raw);
			else{
				uint32_t i=0;
				for(;i<(sizeof(I)/sizeof(*I));i++)if(I[i]==(uint32_t)-1){I[i]=((uint64_t)z-(uint64_t)raw);break;}
				if(i==sizeof(I)/sizeof(*I)){while(1)printf("!!!BArr overfreeing!!!");}
			}
			for(;i<(sizeof(I)/sizeof(*I));i++)if(I[i]==anew)I[i]=-1;
		}
	}
}