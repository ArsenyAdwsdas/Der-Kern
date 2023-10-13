#include"AsciiBase.cpp"
#include"../../include/DerKern/Encods/Ascii.h"
namespace DerKern::ASCII{
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