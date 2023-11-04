#pragma once
#include"../include/DerKern/StandloExeMem.h"
#include"StandloTypes.cpp"
namespace DerKern{
	/*namespace __STANDLOEXEMEM{
		static void finalize(void){ExeMem.next=0;}//Just making sure the OS doesn't swear after the thing at the next line... And while it may be only in exotic cases, this s--- can potentially corrupt the memory. OS level memory corruptions are scary. 
		INITIALIZER(init){atexit(finalize);ExeMem.next=(Allocery::Allo*)-1;}//Just some casual ensuring it won't allocate the next one by making the CPU and the OS swear at you...
	}*/
	Allocery::LinearExe ExeMem(2*1024);
}