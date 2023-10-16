#pragma once
#include"StandloTypes.h"
#include<string>
namespace DerKern{
	using std::string;
	void Error(string);
	string&Error();
	bool GetError(string*);
}