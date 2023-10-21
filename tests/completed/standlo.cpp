#include"../../src/StandloTypes.cpp"

using namespace DerKern;
int main(int argc, char const *argv[]){
	SortList<int,uint16_t,16,cmp>z;uint8_t c=5;
	for(int i=0;i<c;i++){z.set(i);}
	for(int i=0;i<c;i++){
		auto _=z.get(i);
		if(!_||*_!=i)printf("SortList test FAILED at %d %s\n",i,_?"WITH CORRUPTION...":"");
	}

	Dicto<int,int,uint16_t,16,cmp>z2;uint8_t c2=5;
	for(int i=0;i<c2;i++){z2.set(i,i);}
	for(int i=0;i<c2;i++){
		auto _=z2.get(i);
		if(!_||*_!=i)printf("Dicto test FAILED at %d %s\n",i,_?"WITH CORRUPTION...":"");
	}

	return 0;
}