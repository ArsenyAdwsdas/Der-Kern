#pragma once
#include"Ascii.h"
namespace DerKern{
	namespace UTF8{//https://en.wikipedia.org/wiki/UTF-8#Encoding
		struct Char{
			uint8_t s;//How many characters it takes. If >4 then there's a problem because it's not really possible
			uint8_t c[4];//bytes of it.
			Char():size(0){}
			Char(uint8_t C){*c=C;s=1;}
		};
		bool c(uint64_t*sCurrent,string s,Char*);bool c(FILE*,Char*);bool c(char c,FILE*,Char*);
		bool c(uint64_t*sCurrent,string s,BBuf*);bool c(FILE*,BBuf*);bool c(char c,FILE*,BBuf*);
		bool str(uint64_t*sCurrent,string s,string*);bool str(FILE*,string*);
		uint8_t any(uint64_t*sCurrent,string s,Parses::Result*);uint8_t any(FILE*,Parses::Result*);
	}
}