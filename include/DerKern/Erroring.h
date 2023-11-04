#pragma once
#include"StandloTypes.h"
#include<string>
namespace DerKern{
	void Error(string);
	string&Error();
	bool GetError(string*);
}