#define NO_SANITY
#include"../../src/Functions.cpp"

using namespace DerKern;
int main(int argc, char const *argv[]){
	Type*T;
	#ifndef NO_SANITY
	printf("SanityError test: ");
	T=&Type::VOID;
	for(int i=0;i<6;i++){
		printf("\"%s\"+* -> ",T->name.c_str());T=T->pointer();
		if(!T){printf("%s",Error().c_str());break;}
	}printf("\n");
	#endif

	Type*_,*_2;
	T=&Type::u8;
	_=FuncType::Get(&Type::VOID,&T,1);
	_2=FuncType::Get(&Type::VOID,&T,1);if(_!=_2)printf("Get %s FAILED\n",_2->name.c_str());

	T=&Type::i8;
	_=FuncType::Get(&Type::VOID,&T,1);
	_2=FuncType::Get(&Type::VOID,&T,1);if(_!=_2)printf("Get %s FAILED\n",_2->name.c_str());


	_=Type::u8.array(1);
	_2=Type::u8.array(1);if(_!=_2)printf("Get %s FAILED\n",_2->name.c_str());

	_=Type::i8.array(1);
	_2=Type::i8.array(1);if(_!=_2)printf("Get %s FAILED\n",_2->name.c_str());
	return 0;
}