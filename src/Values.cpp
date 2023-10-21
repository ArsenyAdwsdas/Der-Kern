#pragma once
#include"../include/DerKern/Values.h"
#include"StandloTypes.cpp"
namespace DerKern{
	void Scope::operator+=(Type*t){set(t->name,{&Type::_type,t});}
	void Scope::operator+=(Type&t){(*this)+=&t;}
}