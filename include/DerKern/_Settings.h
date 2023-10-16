#pragma once//this one file isn't supposed to be used at least yet.
#include"StandloTypes.h"
namespace DerKern{
	struct ParseSettings{
		SortList<string,uint16_t,16,cmp>UOperators;//before a value
		SortList<string,uint16_t,16,cmp>UPOperators;//after a value
		SortList<string,uint16_t,16,cmp>Operators;//after&before a value
		inline void MathU(){UOperators.set(string(1,'-'));}
		inline void _MathN(){char C[]={'-','+','*','/','%'};for(uint8_t i=0;i<sizeof(C);i++)Operators.set(string(1,C[i]));}
		inline void Cmp(){char C[]={'<','>'};for(uint8_t i=0;i<sizeof(C);i++)Operators.set(string(1,C[i]));Operators.set("<=");Operators.set(">=");Operators.set("==");Operators.set("!=");}
		inline void MathN(){Cmp();_MathN();}
		inline void Math(){MathU();MathN();}
		inline void MathExU(){char C[]={'+'};for(uint8_t i=0;i<sizeof(C);i++)Operators.set(string(1,C[i]));}
		inline void MathAll(){Math();MathExU();}

		inline void BinU(){Operators.set(string(1,'~'));}
		inline void BinN(){char C[]={'|','&','^'};for(uint8_t i=0;i<sizeof(C);i++)Operators.set(string(1,C[i]));}
		inline void Bin(){BinU();BinN();}

		inline void Logic(){string C[]={"||","&&"};for(uint8_t i=0;i<sizeof(C);i++)Operators.set(C[i]);}
		inline void Bitshift(){string C[]={"<<","<<"};for(uint8_t i=0;i<sizeof(C);i++)Operators.set(C[i]);}

		inline void C(){Math();Bin();Logic();Bitshift();}
	};
}