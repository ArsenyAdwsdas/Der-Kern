#include"Ascii.cpp"
#include"../../include/DerKern/Encods/Ascii_CLITERAL.h"
namespace DerKern::ASCII{
	CLITERAL_result CLITERAL(Inputo::Universa*In){
		uint64_t u;char c;
		spaces(In);if(m=In->expect('-'))spaces(In);
		if(u64(In,&u)){
			if(In->expect('f')){
				if(!namend(In))return;
				return m?-(float)u:(float)u;
			}
			if(In->expect('d')){
				if(!namend(In))return;
				return m?-(double)u:(double)u;
			}
			if(In->expect('.')){
				V.a=Type::d;
				double d,_=.1;
				while(I->get(&c,1)&&((c>='0'&&c<='9')||((*I)--&&0))){d+=_*(c-'0');_/=10;}d+=u;
				if(In->expect('f'))return m?-(float)d:(float)d;
				In->expect('d');
				if(!namend(In))return;
				return m?-(double)u:(double)u;
			}
			if(!namend(In))return;
			if(m){
				if(!(u&~0x7f))return-int8_t(u);
				if(!(u&~0x7fff))return-int16_t(u);
				if(!(u&~0x7fffffff))return-int32_t(u);
				return-int64_t(u);
			}
			if(!(u&~0xff))return uint8_t(u);
			if(!(u&~0xffff))return uint16_t(u);
			if(!(u&~0xffffffff))return uint32_t(u);
			return u;
		}
		if(m)return;
		{
			string s;if(str(In,&s))return{s,0};
			if(name(In,&s))return{s,1};
		}
	}
}