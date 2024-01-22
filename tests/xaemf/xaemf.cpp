#include"../../src/Compile.cpp"
#include"../../include/DerKern/extra/parse.h"
#include"../../src/Encods/Ascii_CLITERAL.cpp"
// X86-64 Asm Except More Fun
// This will later become like a replacement for NASM... maybe...

using namespace DerKern;
using ASCII::CLITERAL_result;
void getArg(CLITERAL_result*a,Inputo::Universa*I,const char*argName,const char*w){
	*a=ASCII::CLITERAL(I);if(a->type==0){printf("<%s> couldn't be parsed for \"%s\"\n",argName,w);throw std::exception();}
}
void argSep(Inputo::Universa*I){ASCII::spaces(I);if(I->expect(','))ASCII::spaces(I);}
_Variable resArg(CLITERAL_result&w,ParseResult*r){_Variable _=w.toTV(r->target->env);if(!_.a){printf("UNKNOWN NAME \"%s\"\n",w.s.c_str());throw std::exception();}return _;}
bool instructio(Inputo::Universa*I,ParseResult*r){
	if(I->expect("mov")){
		CLITERAL_result _a;getArg(&_a,I,"a","mov");argSep(I);CLITERAL_result _b;getArg(&_b,I,"b","mov");
		_Variable a=resArg(_a,r),b=resArg(_b,r);

		assert(a.a->typeID!=Type::IDs::Const);//to ensure "a" is something you're allowed to rewrite

		a.b=a.b.resolve();b.b=b.b.resolve();
		if((b.b.type==1||b.b.type==4)&&b.a->typeID==Type::IDs::Const){
			uint64_t w;w=(uint64_t)_b;
			if(a.a->size==1)r->target->add<Instructions::Set8>(a.b,(uint8_t)w);
			if(a.a->size==2)r->target->add<Instructions::Set16>(a.b,(uint16_t)w);
			if(a.a->size==4)r->target->add<Instructions::Set32>(a.b,(uint32_t)w);
			if(a.a->size==8)r->target->add<Instructions::Set64>(a.b,w);
		}else if(a.a->size<b.a->size){//Expanding "b" is needed... Nah, right now I don't wanna do that...

		}else r->target->add<Instructions::Set>(a.b,b.b,a.a->size);
	}else if(I->expect("add")){
		CLITERAL_result _a;getArg(&_a,I,"a","add");argSep(I);CLITERAL_result _b;getArg(&_b,I,"b","add");
		_Variable a=resArg(_a,r),b=resArg(_b,r);

		assert(a.a->typeID!=Type::IDs::Const);//to ensure "a" is something you're allowed to rewrite

		a.b=a.b.resolve();b.b=b.b.resolve();
		if((b.b.type==1||b.b.type==4)&&b.a->typeID==Type::IDs::Const){
			int32_t w;w=(int32_t)_b;
			if(a.a->size==1)r->target->add<Instructions::Add8_1>(a.b,(uint8_t)w);
			if(a.a->size==2)r->target->add<Instructions::Add16_1>(a.b,(uint16_t)w);
			if(a.a->size==4)r->target->add<Instructions::Add32_1>(a.b,w);
		}else if(a.a->size<b.a->size){//Expanding "b" is needed... Nah, right now I don't wanna do that...

		}else r->target->add<Instructions::Add_1>(a.b,b.b,a.a->size);
	}else if(I->expect("xchg")){
		CLITERAL_result _a;getArg(&_a,I,"a","xchg");argSep(I);CLITERAL_result _b;getArg(&_b,I,"b","xchg");
		_Variable a=resArg(_a,r),b=resArg(_b,r);

		assert(a.a->typeID!=Type::IDs::Const);//to ensure "a" is something you're allowed to rewrite

		a.b=a.b.resolve();b.b=b.b.resolve();
		if((b.b.type==1||b.b.type==4)&&b.a->typeID==Type::IDs::Const){printf("CAN'T SWAP A CONSTANT\n");throw std::exception();}
		else if(a.a->size<b.a->size){printf("DO YOU REALLY EXPECT TO SWAP WITH DIFFERENT SIZES?\n");throw std::exception();}else r->target->add<Instructions::Swap>(a.b,b.b,a.a->size);
	}else if(I->expect("ret"))r->target->ins+=&Instructions::ret;else return 0;
	return 1;
}
bool Parse1(Inputo::Universa*I,ParseResult*r){
	if(I->expect('\n'))return 1;
	if(I->expect("//")||I->expect(';')){ASCII::line(I,0);return 1;}
	if(I->expect("/*")){while(!(I->getC()=='*'&&I->getC()=='/'))if(!I->ensure(1)){printf("UNTERMINATED /*\n");return 0;}return 1;}
	if(instructio(I,r)){
		char c;
		ASCII::spaces(I);if((c=I->getC())&&c!='\\'&&c!='\n' &&!((c=='/'||c=='#'||c==';')&&(*I)--)){printf("UNEXPECTED CHARACTER AFTER INSTRUCTION: '%c'(%d)\n",c,int(c));return 0;}
		return 1;
	}
	return 0;
}

int main(){
	Environment env=Environment();
	//Type regT("Register",1),dRegT("DRegister",5);
	RegisterState inpRegisters=env.regs;
	Instructionary ins(&env);
	ParseResult p;p.target=&ins;
	Instructions::x86_64::exposeRegisters(env);
	if(sizeof(int*)==8){		
		env.set("vax",_Variable(&Type::u64,(uint8_t)0));
		env.set("vcx",_Variable(&Type::u64,(uint8_t)1));
		env.set("vdx",_Variable(&Type::u64,(uint8_t)2));
		env.set("vbx",_Variable(&Type::u64,(uint8_t)3));
		env.set("vsp",_Variable(&Type::u64,(uint8_t)4));
		env.set("vbp",_Variable(&Type::u64,(uint8_t)5));
		env.set("vsi",_Variable(&Type::u64,(uint8_t)6));
		env.set("vdi",_Variable(&Type::u64,(uint8_t)7));
	}else{
		env.set("vax",_Variable(&Type::u32,(uint8_t)0));
		env.set("vcx",_Variable(&Type::u32,(uint8_t)1));
		env.set("vdx",_Variable(&Type::u32,(uint8_t)2));
		env.set("vbx",_Variable(&Type::u32,(uint8_t)3));
		env.set("vsp",_Variable(&Type::u32,(uint8_t)4));
		env.set("vbp",_Variable(&Type::u32,(uint8_t)5));
		env.set("vsi",_Variable(&Type::u32,(uint8_t)6));
		env.set("vdi",_Variable(&Type::u32,(uint8_t)7));
	}
	//for(uint8_t i=0;i<env.vars.count;i++)printf(" %s\n",env.vars.raw[i].a.c_str());

	//Inputo::Universa inp=Inputo::RFileHP(fopen("test.xae","rb"));//this one doesn't work and is too overcomplicated for me to casually fix

	FILE*f=fopen("test.xae","r");fseek(f,0, SEEK_END);auto _s=ftell(f);fseek(f,0,SEEK_SET);void*_=malloc(_s);fread(_,1,_s,f);
	Inputo::Universa inp=Inputo::Chunk(_,_s);
	Parse<Parse1>(&p,&inp);
	
	ins.compile();
	if(!ins._compiled){printf("COMPILATION FAIL\n");return-1;}
	ins.eval(env.regs.raw);
	printf("State:\n");for(uint8_t i=0;i<(sizeof(int*)<<1);i++)printf("  r%.2d: %p -> %p\n",(int)i,inpRegisters[i],env.regs[i]);
}