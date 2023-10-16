#include"../src/Type.cpp"

using namespace DerKern;
int main(int argc, char const *argv[]){
	printf("SanityError test: ");
	Type*T=&Type::VOID;
	for(int i=0;i<6;i++){
		printf("\"%s\"+* -> ",T->name.c_str());T=T->pointer();
		if(!T){printf("%s",Error().c_str());break;}
	}printf("\n");


	T=&Type::u8;
	Type*_=Type::VOID.func(&T,1);
	printf("\nFuncCreation(%s) test done",_->name.c_str());
	Type*_2=Type::VOID.func(&T,1);printf("\tGet %s %s\n",_2->name.c_str(),_!=_2?"FAILED":"ok");

	T=&Type::i8;
	_=Type::VOID.func(&T,1);
	printf("FuncCreation(%s) test done",_->name.c_str());
	_2=Type::VOID.func(&T,1);printf("\tGet %s %s\n",_2->name.c_str(),_!=_2?"FAILED":"ok");


	_=Type::u8.array(1);
	printf("\nArrayCreation(%s) test done",_->name.c_str());
	_2=Type::u8.array(1);printf("\tGet %s %s\n",_2->name.c_str(),_!=_2?"FAILED":"ok");

	_=Type::i8.array(1);
	printf("ArrayCreation(%s) test done",_->name.c_str());
	_2=Type::i8.array(1);printf("\tGet %s %s\n",_2->name.c_str(),_!=_2?"FAILED":"ok");
	return 0;
}