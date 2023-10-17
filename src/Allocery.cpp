#pragma once
#include"../include/DerKern/Allocery.h"
#include"StandloTypes.cpp"
namespace DerKern{
	void VarStorage::operator+=(Type*t){set(t->name,{&Type::_type,t});}
	void VarStorage::operator+=(Type&t){(*this)+=&t;}
}