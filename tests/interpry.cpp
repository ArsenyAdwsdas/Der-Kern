#include"../src/Encods/Ascii_CLITERAL.cpp"
#include"../src/Type.cpp"
#include"../src/Values.cpp"
using namespace DerKern;
#error NOT READY
/*
ASCII::CLITERAL_result calc(ASCII::CLITERAL_result a,uint8_t op,ASCII::CLITERAL_result b){
	if(a.type==12){
		if()
		a=
	}
}
void expr_handl(VarStorage&vars,ASCII::CLITERAL_result v,List<pair<ASCII::CLITERAL_result,uint8_t>>l){
	if(!l.count)return;
	for(uint16_t i=0;i<l.count;){
		if(l[i].b=='*'||l[i].b=='/'||l[i].b=='%'){//* / %
			assert(v.type!=11);
			auto&_=*(i?&l[i-1].a:&v);
			_=calc(_,l[i].b,l[i].a);l.remove(i);
		}else i++;
	}
	for(uint16_t i=0;i<l.count;){
		if(l[i].b=='+'||l[i].b=='-'){//+ -
			assert(v.type!=11);
			auto&_=*(i?&l[i-1].a:&v);
			_=calc(_,l[i].b,l[i].a);l.remove(i);
		}else i++;
	}
	for(uint16_t i=0;i<l.count;){
		if(l[i].b==1||l[i].b==2){//<< >>
			assert(v.type!=11);
			auto&_=*(i?&l[i-1].a:&v);
			_=calc(_,l[i].b,l[i].a);l.remove(i);
		}else i++;
	}
	for(uint16_t i=0;i<l.count;){
		if(l[i].b=='<'||l[i].b=='>'||l[i].b=='<'+128||l[i].b=='>'+128){//< > <= >=
			assert(v.type!=11);
			auto&_=*(i?&l[i-1].a:&v);
			_=calc(_,l[i].b,l[i].a);l.remove(i);
		}else i++;
	}
	for(uint16_t i=0;i<l.count;){
		if(l[i].b=='='+128||l[i].b=='!'){//== !=
			assert(v.type!=11);
			auto&_=*(i?&l[i-1].a:&v);
			_=calc(_,l[i].b,l[i].a);l.remove(i);
		}else i++;
	}
	for(uint16_t i=0;i<l.count;){
		if(l[i].b=='&'){//&
			assert(v.type!=11);
			auto&_=*(i?&l[i-1].a:&v);
			_=calc(_,l[i].b,l[i].a);l.remove(i);
		}else i++;
	}
	for(uint16_t i=0;i<l.count;){
		if(l[i].b=='^'){//^
			assert(v.type!=11);
			auto&_=*(i?&l[i-1].a:&v);
			_=calc(_,l[i].b,l[i].a);l.remove(i);
		}else i++;
	}
	for(uint16_t i=0;i<l.count;){
		if(l[i].b=='|'){//|
			assert(v.type!=11);
			auto&_=*(i?&l[i-1].a:&v);
			_=calc(_,l[i].b,l[i].a);l.remove(i);
		}else i++;
	}
	for(uint16_t i=0;i<l.count;){
		if(l[i].b==3){//&&
			assert(v.type!=11);
			auto&_=*(i?&l[i-1].a:&v);
			_=calc(_,l[i].b,l[i].a);l.remove(i);
		}else i++;
	}
	for(uint16_t i=0;i<l.count;){
		if(l[i].b==4){//||
			assert(v.type!=11);
			auto&_=*(i?&l[i-1].a:&v);
			_=calc(_,l[i].b,l[i].a);l.remove(i);
		}else i++;
	}
	for(uint16_t i=0;i<l.count;){
 		assert(v.type!=11);
 		auto&_=*(i?&l[i-1].a:&v);
 		_=calc(_,l[i].b,l[i].a);l.remove(i);
	}
}
ASCII::CLITERAL_result expr(VarStorage&vars,Inputo::Universa&in){
	auto v=in.expect('(')?expr(vars,in):ASCII::CLITERAL(vars,in);List<pair<ASCII::CLITERAL_result,uint8_t>>l;
	while(1){
		auto c=(uint8_t)in.peekC();
		switch(c){
			default:if(!l.count)return v;else{
				expr_handl(vars,v,l);return v;
			}
			case '+':case '-':case '*':case '/':case '%':case '=':{
				in++;
				if(c!='='&&in.expect('='))c|=128;
				auto v2=in.expect('(')?expr(vars,in):ASCII::CLITERAL(vars,in);
				if(!v2){
					//if(in.expect("++"))
					return;
				}
				l+={v2,c};
			}
		}
	}
}
*/
#define numHell(a,op,b) 
ASCII::CLITERAL_result expr(VarStorage&vars,Inputo::Universa&in){
	auto a=ASCII::CLITERAL(in).resolve(&vars);;
	ASCII::spaces(in);
	if(in.expect('=')){
		ASCII::spaces(in);
		assert(a.type==12);
		auto v=vars.get(a.s);if(!v)return;
		void*pntr=
	}
}
int main(){
	VarStorage vars;
	// vars+=Type::u8;vars+=Type::u16;vars+=Type::u32;vars+=Type::u64;
	// vars+=Type::i8;vars+=Type::i16;vars+=Type::i32;vars+=Type::i64;
	// vars+=Type::f;vars+=Type::d;vars+=Type::str;vars+=Type::VOID;
	int64_t var=0;vars.set("var",{&Type::i64,&var});
	Inputo::Universa u=(char*)"var=1";
	if(!expr(vars,u))return-1;
	printf("%lu\n",var);
	return 0;
}