#pragma once
#include"StandloTypes.h"
#if defined(__APPLE__) && (__arm__||__arm64__)
	#error '"'Apple silicon'"'? "Really?.. IT'S CURRENTLY IMPOSSIBLE FOR THIS TO WORK WITH IT. WHY?" "https://stackoverflow.com/a/76089716".
#endif
namespace DerKern{
	extern Allocery::LinearExe ExeMem;
}