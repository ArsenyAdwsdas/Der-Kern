#include"../../include/DerKern/Encods/Ascii.h"
namespace DerKern::ASCII{
	char spec[256]={0};
	bool namable[256]={0};
	namespace{INITIALIZER(init)(){
			spec['n']='\n';spec['r']='\r';
			spec['t']='\t';spec['v']='\v';
			spec['"']='"';spec['\'']='\'';
			spec['0']='\0';spec['f']='\f';
			spec['a']='\a';spec['b']='\b';
			spec['\\']='\\';
			namable['@']=1;namable['$']=1;namable['_']=1;
			for(int i='0';i<'9';i++)namable[i]=1;
			for(int i='a';i<'z';i++)namable[i]=1;
			for(int i='A';i<'Z';i++)namable[i]=1;
	}}
	#define int(a,b,c,d) a##b##c##d
	#define pI(a,u) bool u##a(Inputo::Universa*I,int(u,int,a,_t)*_z){\
		char c;if(!I->peek(&c,1))return 0;\
		bool r=0;decltype(*_z)z=0;if(c=='-'){r=1;(*I)++;if(I->peek(&c,1)&&c<'0'||c>'9')return(*I)--&&0;}else if(c<'0'||c>'9')return 0;\
		while(I->get(&c,1)&&c>='0'&&c<='9')z=(10*z)+c-'0';\
		*_z=r?(decltype(*_z))-z:z;\
		return 1;\
	}
	#define ints(a) pI(a,u)pI(a,)
	ints(8)ints(16)ints(32)ints(64)
	bool f(Inputo::Universa*I,float*_z){
		char c;if(!I->peek(&c,1))return 0;
		bool r=0,d=0;decltype(*_z)z=0,_=.1;if(c=='-'){r=1;(*I)++;if(I->peek(&c,1)&&c<'0'||c>'9')return(*I)--&&0;}else if(c<'0'||c>'9')return 0;
		while(I->get(&c,1)&&c>='0'&&c<='9')if(c=='.')d=1;else if(d){z=_*(c-'0')+z;_/=10;}else*z=(10*z)+c-'0';
		*_z=r?-z:z;
		return 1;
	}
	bool d(Inputo::Universa*I,double*_z){
		char c;if(!I->peek(&c,1))return 0;
		bool r=0,d=0;decltype(*_z)z=0,_=.1;if(c=='-'){r=1;(*I)++;if(I->peek(&c,1)&&c<'0'||c>'9')return(*I)--&&0;}else if(c<'0'||c>'9')return 0;
		while(I->get(&c,1)&&c>='0'&&c<='9')if(c=='.')d=1;else if(d){z=_*(c-'0')+z;_/=10;}else*z=(10*z)+c-'0';
		*_z=r?-z:z;
		return 1;
	}
	#undef ints
	#undef pI
	#undef int


	bool ch(Inputo::Universa*I,char*_c){
		char c;if(!I->get(&c,1))return 0;
		if(c=='\\'){
			if(I->get(&c,1)&&(spec[c]||((*I)--&&0))){*_c=spec[c];return 1;}
			return 0;
		}
		*_c=c;return 1;
	}
	bool ch(Inputo::Universa*I,BBuf*_c){
		char c;if(!I->get(&c,1))return 0;
		if(c=='\\'){
			if(I->get(&c,1)&&(spec[c]||((*I)--&&0))){_c->append(spec[c]);return 1;}
			return 0;
		}
		_c->append(c);return 1;
	}
	bool str(Inputo::Universa*I,string*_z){
		BBuf z;char c;uint64_t i=I->getI();
		if(!I->get(&c,1))return 0;
		if(c=='"'){while(I->peek(&c,1)&&c!='"')if(c=='\n'||!ch(I,&z)){I->setI(i);return 0;}}
		else if(c=='`'){while(I->peek(&c,1)&&c!='`')if(!ch(I,&z)){I->setI(i);return 0;}}
		else{(*I)--;return 0;}*_z=string(z.raw,z.count);return 1;
	}
	bool name(Inputo::Universa*I,string*_z){
		BBuf z;char c;uint64_t i=I->getI();
		if(!I->peek(&c,1)||!namable[c])return 0;
		while(I->get(&c,1)&&namable[c])z+=c;
		*_z=string(z.raw,z.count);return 1;
	}
	bool namend(Inputo::Universa*I){return!namable[I->peekC()];}
	void line(Inputo::Universa*I,string*_z){BBuf z;char c;while(I->get(&c,1)&&c!='\n'){z+=c;}*_z=string(z.raw,z.count);}
	void spaces(Inputo::Universa*I){while(I->expect(' ')||I->expect('\t'));}
	Instruction* _value(Inputo::Universa*In,pair<Type*,Location>&V){
		uint64_t u;char c;
		spaces(In);if(m=In->expect('-'))spaces(In);
		if(u64(In,&u)){
			if(In->expect('f')){
				if(!namend(In))return 0;
				V.a=&Type::f;return new Instructions::set(V.b,m?-(float)u:(float)u);
			}
			if(In->expect('d')){
				if(!namend(In))return 0;
				V.a=&Type::d;return new Instructions::set(V.b,m?-(double)u:(double)u);
			}
			if(In->expect('.')){
				V.a=Type::d;
				double d,_=.1;
				while(I->get(&c,1)&&((c>='0'&&c<='9')||((*I)--&&0))){d+=_*(c-'0');_/=10;}d+=u;
				if(In->expect('f')){
					V.a=&Type::f;return new Instructions::set(V.b,m?-(float)d:(float)d);
				}
				In->expect('d');
				if(!namend(In))return 0;
				V.a=&Type::d;return new Instructions::set(V.b,m?-(double)u:(double)u);
			}
			if(!namend(In))return 0;
			if(m){
				if(!(u&~0x7f)){V.a=&Type::i8;return new Instructions::set(V.b,-int8_t(u));}
				if(!(u&~0x7fff)){V.a=&Type::i16;return new Instructions::set(V.b,-int16_t(u));}
				if(!(u&~0x7fffffff)){V.a=&Type::i32;return new Instructions::set(V.b,-int32_t(u));}
				V.a=&Type::i64;return new Instructions::set(V.b,-int64_t(u));
			}
			if(!(u&~0xff)){V.a=&Type::u8;return new Instructions::set(V.b,uint8_t(u));}
			if(!(u&~0xffff)){V.a=&Type::u16;return new Instructions::set(V.b,uint16_t(u));}
			if(!(u&~0xffffffff)){V.a=&Type::u32;return new Instructions::set(V.b,uint32_t(u));}
			V.a=&Type::u64;return new Instructions::set(V.b,uint64_t(u));
		}
		if(m)return 0;
		{
			string s;if(str(In,&s)){V.a=&Type::str;return new Instructions::set(V.b,s);}
			if(name(In,&s)){V.a=&Type::str;return new Instructions::get(V,s);}
		}
	}
	bool value(Inputo::Universa*In,pair<Type*,Location>&V,List<Instruction*>*Is){
		spaces(In);uint64_t i,u;char c;if(!In->get(&c,1))return 0;i=In.getI();Instruction*I;
		pair<Type*,Location>_V;_V.a=0;
		#ifndef HASTE
			if(I=_value(In,V));else
		#endif
		switch(c){
			case '=':I=value(In,_V);if(I){I=new Instructions::UOp::Equ(I,V);(*Is)+=I;}break;
			case '$':I=value(In,_V);if(I){I=new Instructions::UOp::Dolla(I,V);(*Is)+=I;}break;
			case '#':I=value(In,_V);if(I){I=new Instructions::UOp::Hash(I,V);(*Is)+=I;}break;
			case '+':I=value(In,_V);if(I){I=new Instructions::UOp::Plugate(I,V);(*Is)+=I;}break;
			case '~':I=value(In,_V);if(I){I=new Instructions::UOp::BNot(I,V);(*Is)+=I;}break;
			case '!':I=value(In,_V);if(I){I=new Instructions::UOp::Not(I,V);(*Is)+=I;}break;
			case '*':I=value(In,_V);if(I){I=new Instructions::UOp::Star(I,V);(*Is)+=I;}break;
			case '-':I=value(In,_V);if(I){I=new Instructions::UOp::Negate(I,V);(*Is)+=I;}
			#ifdef HASTE
				break;default:(*In)--;I=_value(In,V);
			#endif
		}if(!I)In.setI(i);return I;
	}
	bool expr(Inputo::Universa*In,pair<Type*,Location>&V,List<Instruction*>*out){
		List<Instruction*>_;
		List<pair<List<Instruction*>,TypeDesc::Ops>>extras;
		if(!value(In,v,_))return 0;
		if(In->expect('%')){
			extras+={_,In->expect('=')?TypeDesc::Ops::modE:TypeDesc::Ops::mod}
		}
	}
}