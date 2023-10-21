#include"../../src/Encods/Ascii.cpp"

using namespace DerKern;
// How shady... Eh, not like I change it anyway so (char*)"" seems fine here to both me and, for whatever reason, the compiler...
void testU8(Inputo::Universa&a,uint8_t expect){
	uint8_t z;
	if(!ASCII::u8(&a,&z))printf("ASCII::u8 FAILED ON %.*s\n",a.c.c.size,a.c.c.data);
	else if(z!=expect)printf("ASCII::u8 test fail\n\t%.*s -> %d\n",a.c.c.size,a.c.c.data,(int)expect);
}
int main(int argc, char const *argv[]){
	Inputo::Universa str=(char*)"1";testU8(str,1);
	str=(char*)"100";testU8(str,100);

	str=(char*)"\"\\n1\\n2\\t2\\\\\"";
	string s;
	if(!ASCII::str(&str,&s))return-1;
	if(cmp(s,"\n1\n2\t2\\")!=0)printf("ASCII::str test fail\n");

	str=(char*)"12351\\\n125";
	ASCII::line(&str,&s);
	if(cmp(s,"12351\\")!=0)printf("ASCII::line test fail\n");

	/*//and here's an old version
	Inputo::Universa str=(char*)"1";
	uint8_t z=9;
	if(!ASCII::u8(&str,&z))return-1;
	printf("%u\n",(unsigned)z);

	str=(char*)"\"\\n1\\n2\\t2\"";
	string s;
	if(!ASCII::str(&str,&s))return-1;
	printf("```%s```\n",s.c_str());

	str=(char*)"12351\n125";
	ASCII::line(&str,&s);
	printf("%s\n",s.c_str());
	*/
	return 0;
}