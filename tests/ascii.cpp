#include"../src/Encods/Ascii.cpp"

using namespace DerKern;
// How shady... Eh, not like I change it anyway so (char*)"" seems fine here to both me and, for whatever reason, the compiler...
int main(int argc, char const *argv[]){
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
	return 0;
}