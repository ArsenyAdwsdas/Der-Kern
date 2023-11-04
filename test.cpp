#include<stdio.h>
#include<stdlib.h>
#include"src/DerKern.cpp"
#include"src/Encods/Ascii_CLITERAL.cpp"

using namespace DerKern;
template<size_t S>inline bool Command(const char(&cmd)[S],const char*str,uint64_t*i,uint64_t siz){
	if(siz>=(S-1)+*i&&strncmp(str+*i,cmd,(S-1))==0){
		(*i)+=S-1;return 1;
	}else return 0;
}
using ASCII::namend;
bool Spaces(uint64_t*i,string s){
	if(!namend(s[*i]))return 0;
	while(s.size()>*i&&(s.at(*i)==' '||s.at(*i)=='\t'))(*i)++;
	return 1;
}
void SPACES(uint64_t*i,string s){assert(s.size()>*i&&(s.at(*i)==' '||s.at(*i)=='\t'));Spaces(i,s);}

#ifndef _MAX_PATH
#define _MAX_PATH 260
#warning _MAX_PATH!
#endif
#warning test.cpp isn't ready!
InstructionWithOutput*value(Inputo::Universa*In,pair<Type*,Location>&V){//if no Type/Location then it sets that by itself, returns 0 if failed.
	auto _=ASCII::CLITERAL(In);
	if(_){V=_.toTV(0);return _;}

	BBuf z;char c;uint64_t i=In->getI();
	if(!In->peek(&c,1))return 0;
	if(c=='`'&&(*In)++){
		while(In->peek(&c,1)&&c!='`')if(!ch(In,&z)){In->setI(i);return 0;}
		V.a=&Type::str;return new Instructions::set(V.b,string((char*)z.raw,z.count));
	}

	return 0;
}
/*bool value_UOpy(Inputo::Universa*In,pair<Type*,Location>&V,List<Instruction*>*Is){
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
}*/
/*bool expr(Inputo::Universa*In,pair<Type*,Location>&V,List<Instruction*>*out){
	List<Instruction*>_;
	List<pair<List<Instruction*>,TypeDesc::Ops>>extras;
	if(!value(In,v,_))return 0;
	if(In->expect('%')){
		extras+={_,In->expect('=')?TypeDesc::Ops::modE:TypeDesc::Ops::mod}
	}
}*/
bool DerKern::Parse1(Inputo::Universa*I,ParseResult*r){
	uint64_t siz=S.size(),_i=*i;
	auto s=S.c_str();
	auto c=s[*i];
	if(c==' '||c=='\t'||c==';'){(*i)++;return 1;}
	if(c=='\n'){(*i)++;r->target->ln();return 1;}
	if(Command("file",s,i,siz)&&ASCII::SPACES(I)){
		FILE*f;{string n;if(ASCII::str(i,S,&n)){f=fopen(n.c_str(),"rb");if(!f){printf("?.. \"%s\" ?..\n",n.c_str());while(1);}}else{
			assert(siz>*i);
			//uint64_t _i=(uint64_t)strchr(s+*i,'\n');if(_i==0)_i=siz-*i;else _i-=(uintptr_t)s+*i;
			//char fN[_MAX_PATH];assert(_MAX_PATH>_i);
			//memcpy(fN,s+*i,_i);fN[_i]=0;*i+=_i;
			uint64_t _i=*i;while(siz>*i&&s[*i]!='\n')(*i)++;
			char fN[_MAX_PATH];assert(_MAX_PATH>*i-_i);
			memcpy(fN,s+_i,*i-_i);fN[*i-_i]=0;

			f=fopen(fN,"rb");if(!f){printf("?.. \"%s\" ?..\n",fN);while(1);}
		}}
		ParseResult r2;
		Inputo::Universa z=Inputo::RFileHP(f);
		Parse(&r2,&z);
		free(z.f2.data);fclose(f);
		return r->target->ins+=new Instructions::Paste(r2);
	}else*i=_i;
	if((Command("ret",s,i,siz)||Command("return",s,i,siz))&&Spaces(i,S)){Spaces(i,S);return r->target->ins+=Instructions::Return(Parse1(i,S,r));}else*i=_i;
	if(Command("pong",s,i,siz)&&namend(I)){return r->target->ins+=&Instructions::pong;}else*i=_i;
	if(Command("//",s,i,siz)){while(siz>*i&&s[*i]!='\n')(*i)++;if(siz>*i){(*i)++;r->target->ln();}return 1;}else*i=_i;
	if(Command("jmp",s,i,siz)&&ASCII::SPACES(I)&&siz>(*i)+1){
		int16_t o;bool m=s[*i]=='-';
		if(m||s[*i]=='+'){(*i)++;ASCII::spaces(I);if(ASCII::u16(i,S,&o)&&namend(I))return r->target->ins+=new Instructions::jmpLine(m?-o:o);}
		if(ASCII::i16(I,&o)&&namend(I))return r->target->ins+=new Instructions::jmpSetLine(o);
	}else*i=_i;
	/*Parses::Result r;
	if(!ASCII::any(i,S,&r))return (Instruction*)(uint64_t)-1;
	if(r.type==)*/
	pair<Type*,Location>_;
	if(value(i,S,&_))return 1;
	return 0;
}
void print(int32_t a){printf("%d\n",a);}
int main(int argc, char const *argv[]){
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	Environment e;
	//while(1){
	e.pointTo("int",&Type::_type,&Type::i32);
	e.setFunc("print",print,0,new DerKern::TypePtr(DerKern::Type::i32),1);

	ParseResult p=Parse(0,"print(08102023)");// file `input.txt`
	p.eval(&e);
	//p.compile(0,&e);
	p.death();

	//}
	return 0;
}
