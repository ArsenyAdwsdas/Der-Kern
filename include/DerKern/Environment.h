#pragma once
#include"Values.h"
#include"Erroring.h"
namespace DerKern{
	struct Environment:Scope{
		Environment(size_t stackSize);inline Environment():Environment(1024*512){}
		uint8_t*ip;//https://en.wikipedia.org/wiki/Program_counter
		RegisterState regs;
		// FlagState flags;//I don't really have a way to store those.
		uint8_t*stack;//It doesn't really HAVE to be used as stack, though I dunno why anyone would just replace "sp" register and not put it back later... Also this pointer shouldn't be modified ever.
		uint32_t stackSize;

		~Environment();

		Allocery::BArrExe exeMem;
	};
}