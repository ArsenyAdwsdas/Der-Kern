#include"../../include/DerKern/Encods/AsciiBase.h"
#include"../StandloTypes.cpp"
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
		bool r=0;int(u,int,a,_t)z=0;if(c=='-'){r=1;(*I)++;if(I->peek(&c,1)&&c<'0'||c>'9')return(*I)--&&0;}else if(c<'0'||c>'9')return 0;\
		while(I->get(&c,1)&&c>='0'&&c<='9')z=(10*z)+c-'0';\
		*_z=r?(decltype(*_z))-z:z;\
		return 1;\
	}
	#define ints(a) pI(a,u)pI(a,)
	ints(8)ints(16)ints(32)ints(64)
	bool f(Inputo::Universa*I,float*_z){
		char c;if(!I->peek(&c,1))return 0;
		bool r=0,d=0;float z=0,_=.1;if(c=='-'){r=1;(*I)++;if(I->peek(&c,1)&&c<'0'||c>'9')return(*I)--&&0;}else if(c<'0'||c>'9')return 0;
		while(I->get(&c,1)&&c>='0'&&c<='9')if(c=='.')d=1;else if(d){z=_*(c-'0')+z;_/=10;}else*z=(10*z)+c-'0';
		*_z=r?-z:z;
		return 1;
	}
	bool d(Inputo::Universa*I,double*_z){
		char c;if(!I->peek(&c,1))return 0;
		bool r=0,d=0;double z=0,_=.1;if(c=='-'){r=1;(*I)++;if(I->peek(&c,1)&&c<'0'||c>'9')return(*I)--&&0;}else if(c<'0'||c>'9')return 0;
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
		else{(*I)--;return 0;}*_z=string((char*)z.raw,z.count);return 1;
	}
	bool name(Inputo::Universa*I,string*_z){
		BBuf z;char c;uint64_t i=I->getI();
		if(!I->peek(&c,1)||!namable[c])return 0;
		while(I->get(&c,1)&&namable[c])z+=c;
		*_z=string((char*)z.raw,z.count);return 1;
	}
	bool namend(Inputo::Universa*I){return!namable[I->peekC()];}
	void line(Inputo::Universa*I,string*_z){BBuf z;char c;while(I->get(&c,1)&&c!='\n'){z+=c;}*_z=string((char*)z.raw,z.count);}
	void spaces(Inputo::Universa*I){while(I->expect(' ')||I->expect('\t'));}
}