#pragma once
#include"../StandloTypes.h"
#include"../Inputo.h"
namespace DerKern{
	namespace ASCII{
		#define int(a,b,c,d) a##b##c##d
		#define pI(a,B,C) bool C##a(Inputo::Universa*,int(B,int,a,_t));
		#define ___NOTH
		#define ints(a) pI(a,u,u)pI(a,___NOTH,i)
		ints(8)ints(16)ints(32)ints(64)
		#undef ints
		#undef pI
		#undef int
		bool f(Inputo::Universa*,float*);
		bool d(Inputo::Universa*,double*);
		bool name(Inputo::Universa*,string*);
		bool ch(Inputo::Universa*,char*);
		bool ch(Inputo::Universa*,BBuf*);
		bool str(Inputo::Universa*,BBuf*);bool str(Inputo::Universa*,string*);
		bool namend(Inputo::Universa*);//An exception that only returns whether the character can't be in a name, only peeks at the next character
		void spaces(Inputo::Universa*);//skip(regular spaces)
		inline bool SPACES(Inputo::Universa*I){if(!namend(I))return 0;spaces(I);return 1;}//namend&&(spaces|1)
		void line(Inputo::Universa*,string*dst);//"dst" can be 0 to skip a line
	}
}