#pragma once
#include"Ascii.h"
#include"../Type.h"
#include"../Instruction.h"
namespace DerKern{
	namespace ASCII{
		#warning Ascii_CLITERAL.h may be not ready for use...
		InstructionWithOutput*CLITERAL(Inputo::Universa*,pair<Type*,Location>&);//doesn't go back on fail, only supports numbers, strings.
	}
}