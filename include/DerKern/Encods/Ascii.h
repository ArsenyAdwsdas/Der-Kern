#pragma once
#include"../parse.h"
namespace DerKern{
	namespace ASCII{
		#define int(a,b,c,d) a##b##c##d
		#define pI(a,u) bool u##a(Inputo::Universa*,int(u,int,a,_t)*);
		#define ___NOTH
		#define ints(a) pI(a,u)pI(a,___NOTH)
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
		void line(Inputo::Universa*,string*);

		bool value(Inputo::Universa*,pair<Type*,Location>&,List<Instruction*>*out);//if no Type/Location then it sets that by itself, returns 0 if failed
		Instruction*_value(Inputo::Universa*,pair<Type*,Location>&);//doesn't go back on fail, doesn't use operators.
		bool expr(Inputo::Universa*,pair<Type*,Location>&,List<Instruction*>*out);
	}
}