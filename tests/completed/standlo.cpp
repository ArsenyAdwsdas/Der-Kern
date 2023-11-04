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
	
	Dicto<string,uint8_t,uint16_t,16,cmp>w;
	w.set("1",1);
	w.set("5",5);
	w.set("2",2);
	w.set("6",6);
	w.set("4",4);
	w.set("3",3);
	for(uint8_t i=0;i<w.count;i++)printf(" %s\n",w.raw[i].a.c_str());
	return 0;
}