#include"../src/Compile.cpp"
int main(){
	DerKern::Instructionary instructionary(0);
	instructionary.inliny=1;
	instructionary +=new DerKern::Instructions::Set32((uint8_t)0,69420);
	instructionary.compile();
	uint64_t _=((uint32_t(*)())instructionary._compiled)();
	printf("%llu\n",_);
}
