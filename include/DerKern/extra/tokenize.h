#pragma once
#include"../StandloTypes.h"

namespace DerKern{
	struct Tokene{
		ENUM(Types,uint8_t)
			unparsed,//basically all unparsed characters and spaces
			spacings,//"unparsed" except only ' ','\n','\t'
			name,// [a-zA-Z_0-9]+  // http://syque.com/cstyle/ch5.1.htm ANSI C minimal maximum is 31 character? Pfft--
			brackies,//one character brackets like (),{},[],<>,(],[),<},{],[>,"",'',``,"\n //(yes quotes will be brackies, just that by default there'll be a different parser)
			sbrackies,//custom string brackets
			cust//completely yours to define
			ENUM_END
		Types::T type;
		union{
			string nam;//unparsed/spacings or name
			struct{
				Tokene*intern;uint16_t internC;
				struct{char lb,rb;};//LeftBracket,RightBracket
				struct{string lbs,rbs;};//same as previous except "string"s
			};
			struct{uint64_t custExtra;void*cust;}
		};
	};
	namespace Tokenin{
		typedef bool comped1(Allocery::Linear*,Tokene*result,const char*inp,size_t inp_siz);//return 1 on error and use <result> for keeping the error

	}
}