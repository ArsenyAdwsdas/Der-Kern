#pragma once
#include"AsciiBase.h"
#include"../Type.h"
#include"../Instruction.h"
namespace DerKern{
	namespace ASCII{
		bool value(Inputo::Universa*,pair<Type*,Location>&,List<Instruction*>*out);//if no Type/Location then it sets that by itself, returns 0 if failed
		Instruction*_value(Inputo::Universa*,pair<Type*,Location>&);//doesn't go back on fail, doesn't use operators.
		bool expr(Inputo::Universa*,pair<Type*,Location>&,List<Instruction*>*out);
	}
}