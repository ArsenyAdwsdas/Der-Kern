#pragma once
#include"Type.h"
namespace DerKern{
	struct Environment{
		Dicto<string,pair<Type*,Location>>vars;
		#if _WIN64||__x86_64__||__ppc64__
			typedef uint64_t regT;
		#else
			typedef uint32_t regT;
		#endif
		Environment();
		regT regs[sizeof(int*)==4?8:16];
		uint8_t*ip;//https://en.wikipedia.org/wiki/Program_counter
		uint8_t*stack;//It doesn't really HAVE to be used as stack, though I dunno why anyone would just replace "sp" register and not put it back later... Also this pointer shouldn't be modified ever.
		uint32_t stackSize;

		~Environment();
		int32_t&pointTo(string name,Type*,void*location);//returns offset to access it
	};
}