#pragma once
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
namespace DerKern{
	namespace Inputo{
		struct _Chunk{
			void*data;size_t size;
			inline _Chunk(void*d,size_t s){data=d;size=s;}
			inline size_t get(void*dst,size_t start,size_t siz){size_t _=size-start;if(_>siz)_=siz;memcpy(dst,(char*)data+start,_);return _;}
		};
		struct RFile{
			constexpr static size_t DEFAULT_SIZE=1024*1024;
			FILE*f;void*data;size_t size;
			uint64_t fI,i;
			inline uint64_t operator=(uint64_t z){fI=z;i=0;_fseeki64(f,z,SEEK_SET);return fread(data,1,z,f);}//returns available bytes there out of <size>
			inline RFile(FILE*F){f=F;size=DEFAULT_SIZE;data=malloc(size);*this=(uint64_t)0;}
			inline RFile(FILE*F,size_t s){f=F;size=s;data=malloc(s);*this=(uint64_t)0;}
			inline RFile(FILE*F,void*d,size_t s){f=F;size=s;data=d;*this=(uint64_t)0;}
			inline uint64_t ensure(uint64_t z){//ensures [?previously loaded?]<i><loaded?>, returns how much ensured
				uint64_t _=size-i;assert(size>=z);if(_>z)return z;uint64_t _2=z-_;
				_fseeki64(f,fI+size+_2,SEEK_SET);
				memmove(data,(char*)data+_2,size-_2);i-=_2;fI+=_2;
				z=fread((char*)data+(size-_2),1,_2,f);return _+z;
			}
			inline uint64_t operator+=(uint64_t z){auto _=ensure(z);i+=_;return _;}//returns how much could
			inline int64_t operator+=(int64_t z){if(0>z)return-((*this)-=-(uint64_t)z);return(*this)+=(uint64_t)z;}//returns how much could
			inline bool operator++(){return(*this)+=(uint64_t)1;}//returns whether could do that
			inline uint64_t operator-=(uint64_t z){
				if(i>=z){i-=z;return z;}
				uint64_t _=z-i;if(_>fI)_=fI;
				fI-=_;memmove((char*)data+_,data,size-_);
				_fseeki64(f,fI,SEEK_SET);z=fread(data,1,_,f);
				z+=i;i=0;return z;
			}//returns how much could
			inline int64_t operator-=(int64_t z){if(z>0)return-((*this)-=-(uint64_t)z);return(*this)+=(uint64_t)z;}//returns how much could
			inline bool operator--(){return(*this)-=(uint64_t)1;}//returns whether could do that
			inline uint64_t ensureBack(uint64_t z){assert(i>=z);if(i>z){uint64_t _=i+z;z=((*this)-=z);i=_;return z;}else return(*this)-=z;}//ensures <loaded?><i>[?previously loaded?]
			inline uint64_t get(void*dst,uint64_t siz){siz=ensure(siz);memcpy(dst,(char*)data+i,siz);i+=siz;return siz;}//i+=<how much can get>, returns how much got
			inline char getC(){if(!ensure(1))return 0;i++;return ((char*)data)[i-1];}
			inline uint64_t peek(void*dst,uint64_t siz){siz=ensure(siz);memcpy(dst,(char*)data+i,siz);return siz;}//i+=<how much can peek>, returns how much could peek
			inline char peekC(){if(!ensure(1))return 0;return((char*)data)[i];}
			inline bool expect(char c){if(ensure(1)&&((char*)data)[i]==c){(*this)++;return 1;}return 0;}
			template<uint16_t S>inline bool expect(char c[S]){if((ensure(S-1)>=S-1)&&memcmp((char*)data+i,c,S-1)==0){(*this)+=S-1;return 1;}return 0;}
		};
		struct RFileHP{//tries to move by half size
			constexpr static size_t DEFAULT_SIZE=1024*1024;
			FILE*f;void*data;size_t size;
			uint64_t fI,i;
			inline uint64_t operator=(uint64_t z){fI=z;i=0;_fseeki64(f,z,SEEK_SET);return fread(data,1,z,f);}//returns available bytes there out of <size>
			inline RFile(FILE*F){f=F;size=DEFAULT_SIZE;data=malloc(size);*this=(uint64_t)0;}
			inline RFile(FILE*F,size_t s){f=F;size=s;data=malloc(s);*this=(uint64_t)0;}
			inline RFile(FILE*F,void*d,size_t s){f=F;size=s;data=d;*this=(uint64_t)0;}
			inline uint64_t _ensure(){//[?previously loaded?]<i><loaded?>, returns how much ensured
				uint64_t z=(size>>1);
				uint64_t _=size-i;assert(size>=z);if(_>z)return z;uint64_t _2=z-_;
				_fseeki64(f,fI+size+_2,SEEK_SET);
				memmove(data,(char*)data+_2,size-_2);i-=_2;fI+=_2;
				z=fread((char*)data+(size-_2),1,_2,f);return _+_2;
			}inline uint64_t ensure(uint64_t z){uint64_t _=_ensure();return z>_?_:z;}//returns how much could out of requested
			inline uint64_t operator+=(uint64_t z){auto _=ensure(z);i+=_;return _;}//returns how much could out of requested
			inline int64_t operator+=(int64_t z){if(0>z)return-((*this)-=-(uint64_t)z);return(*this)+=(uint64_t)z;}//returns how much could out of requested
			inline bool operator++(){return(*this)+=(uint64_t)1;}//returns whether could do that
			inline uint64_t operator-=(uint64_t _z){//returns how much could out of requested
				if(i>=_z){i-=_z;return _z;}
				uint64_t z=(size>>1);
				uint64_t _=z-i;if(_>fI)_=fI;
				fI-=_;memmove((char*)data+_,data,size-_);
				_fseeki64(f,fI,SEEK_SET);z=fread(data,1,_,f);
				z+=i;i=0;return z>_z?_z:z;
			}
			inline int64_t operator-=(int64_t z){if(z>0)return-((*this)-=-(uint64_t)z);return(*this)+=(uint64_t)z;}//returns how much could out of requested
			inline bool operator--(){return(*this)-=(uint64_t)1;}//returns whether could do that
			inline uint64_t ensureBack(uint64_t z){assert(i>=z);if(i>z){uint64_t _=i+z;z=((*this)-=z);i=_;return z;}else return(*this)-=z;}//ensures <loaded?><i>[?previously loaded?]
			inline uint64_t get(void*dst,uint64_t siz){siz=ensure(siz);memcpy(dst,(char*)data+i,siz);i+=siz;return siz;}//i+=<how much can get>, returns how much got
			inline char getC(){if(!_ensure(1))return 0;i++;return ((char*)data)[i-1];}
			inline uint64_t peek(void*dst,uint64_t siz){siz=ensure(siz);memcpy(dst,(char*)data+i,siz);return siz;}//i+=<how much can peek>, returns how much could peek
			inline char peekC(){if(!_ensure(1))return 0;return((char*)data)[i];}
			inline bool expect(char c){if(_ensure(1)&&((char*)data)[i]==c){(*this)++;return 1;}return 0;}
			template<uint16_t S>inline bool expect(char c[S]){if((_ensure(S-1)>=S-1)&&memcmp((char*)data+i,c,S-1)==0){(*this)+=S-1;return 1;}return 0;}
		};
		struct Universa{
			uint8_t type;//0=error
			union{
				struct{_Chunk c;uint64_t i;};//1
				RFile f;//2
				RFileHP f2;//3
			};
			inline uint64_t getI(){
				if(type==3)return f2.fI+f2.i;
				if(type==2)return f.fI+f.i;
				if(type==1)return i;
			}
			inline uint64_t setI(uint64_t z){
				if(type==3)return f2=z;
				if(type==2)return f=z;
				if(type==1){i=c.size>z?z:c.size;return c.size-i;}
			}
			inline uint64_t operator=(uint64_t z){return setI(z);}
			inline Universa(){}
			inline Universa(FILE*F){type=3;f2=RFileHP(F);}inline Universa(RFile F){type=2;f=F;}inline Universa(RFileHP F){type=3;f2=F;}
			inline Universa(void*a,size_t b){type=1;c=_Chunk(a,b);i=0;}inline Universa(_Chunk C){type=1;c=C;i=0;}
			inline uint64_t ensure(uint64_t z){
				if(type==3)return f2.ensure(z);
				if(type==2)return f.ensure(z);
				if(type==1){if(c.size-i>z)return r;return c.size-i;}
			}
			template<typename t>inline t operator+=(t z){
				if(type==3)return f2+=z;
				if(type==2)return f+=z;
				if(type==1){if(-z>i){uint64_t _=i;i=0;return _;}else if(i+z>c.size){uint64_t _=i;i=c.size;return c.size-_;}i+=z;return z;}
			}
			inline bool operator++(){return(*this)+=(bool)1;}
			template<typename t>inline t operator-=(t z){
				if(type==3)return f2-=z;
				if(type==2)return f-=z;
				if(type==1){if(z>i){uint64_t _=i;i=0;return _;}else if(i-z>c.size){uint64_t _=i;i=c.size;return c.size-_;}i-=z;return z;}
			}
			inline bool operator--(){return(*this)-=(bool)1;}
			inline size_t get(void*dst,uint64_t siz){
				if(type==3)return f2.get(dst,siz);
				if(type==2)return f.get(dst,siz);
				if(type==1){siz=c.get(dst,i,siz);i+=siz;return siz;}
			}
			inline char getC(){
				if(type==3)return f2.getC();
				if(type==2)return f.getC();
				if(type==1){if(c.size>=i)return 0;i++;return((char*)c.data)[i-1];}
			}
			inline size_t peek(void*dst,uint64_t siz){
				if(type==3)return f2.peek(dst,siz);
				if(type==2)return f.peek(dst,siz);
				if(type==1){siz=c.peek(dst,i,siz);i+=siz;return siz;}
			}
			inline char peekC(){
				if(type==3)return f2.peekC();
				if(type==2)return f.peekC();
				if(type==1){if(c.size>=i)return 0;return((char*)c.data)[i];}
			}
			inline bool expect(char c){
				if(type==3)return f2.expect(c);
				if(type==2)return f.expect(c);
				if(type==1){if(ensure(1)&&((char*)c.data)[i]==c){i++;return 1;}return 0;}
			}
			template<uint16_t S>inline bool expect(char c[S]){
				if(type==3)return f2.expect(c);
				if(type==2)return f.expect(c);
				if(type==1){if(ensure(S-1)&&memcmp((char*)c.data+i,c,S-1)==0){i+=S-1;return 1;}return 0;}
			}
		};
	}
}