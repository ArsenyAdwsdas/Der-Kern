#pragma once
#include"../include/DerKern/Erroring.h"
namespace DerKern{
	string err="";
	void Error(string s){err=s;}
	string&Error(){return err;}
	bool GetError(string*s){if(err=="")return 0;*s=err;return 1;}
}