#pragma once
#include"../include/DerKern/Type.h"
#include"Values.cpp"
#include"Erroring.cpp"
namespace DerKern{
	Type Type::i8=Type("int8",1);Type Type::u8=Type("uint8",1);
	Type Type::i16=Type("int16",2);Type Type::u16=Type("uint16",2);
	Type Type::i32=Type("int32",4);Type Type::u32=Type("uint32",4);
	Type Type::i64=Type("int64",8);Type Type::u64=Type("uint64",8);
	Type Type::f=Type("float",sizeof(float));Type Type::d=Type("double",sizeof(double));
	Type Type::str=Type("string",sizeof(int*));Type Type::_type=Type("type",sizeof(Type));
	Type Type::VOID=Type("void",1);
	Type Type::const_container=Type("<CONST>",0);Type Type::array_container=Type("<ARRAY>",0);
	namespace __TYPE{INITIALIZER(init){
		Type::VOID.size=0;
	}}
	Ptr1Type*Type::pointer(){
		if(isFinal){Error("An attempt to have a * pointer with a type that doesn't allow it");return 0;}//decided to write it like having a child
		if(_pointer)return _pointer;
		#ifndef NO_SANITY//Seems like a more sane way than "if(typeID==)"+"(TypeModifier*)"... Not like I care about sanity though... Just that it would look like too much.
			if(name.size()>4&&name.substr(name.size()-4,4)=="****"){Error("Sanity Error");return 0;}
		#endif
		_pointer=new Ptr1Type(this);_pointer->parent=ptr1_container;
		return _pointer;
	}Ptr1Type*Type::ptr1_container=Type::VOID.pointer();
	Ptr2Type*Type::pointer2(){
		if(!size||isFinal){Error("An attempt to have a & pointer with a type that doesn't allow it");return 0;}
		if(_pointer2)return _pointer2;
		_pointer2=new Ptr2Type(this);_pointer2->parent=ptr2_container;
		return _pointer2;
	}Ptr2Type*Type::ptr2_container=Type::VOID.pointer2();
	Ptr3Type*Type::pointer3(){
		if(!size||isFinal){Error("An attempt to have a && pointer with a type that doesn't allow it");return 0;}
		if(_pointer3)return _pointer3;
		_pointer3=new Ptr3Type(this);_pointer3->parent=ptr3_container;
		return _pointer3;
	}Ptr3Type*Type::ptr3_container=Type::VOID.pointer3();
	ConstType*Type::Const(){
		if(!size){Error("An attempt to \"const\" a basically mythological(non-existent like \"void\") type");return 0;}
		if(_const)return _const;
		_const=new ConstType(this);_const->parent=&const_container;
		return _const;
	}
	static int cmp(ArrayType*const&a,const uint32_t &b){return cmp(a->count,b);}
	ArrayType*Type::array(uint32_t count){
		if(isFinal){Error("An attempt to array a type that doesn't allow it");return 0;}
		uint16_t b;if(BinSearch<ArrayType*,uint32_t,cmp,uint16_t>(0,b,_arrays.count,(ArrayType**)_arrays.raw,count))return(ArrayType*)_arrays[b];
		auto z=_arrays._insert(new ArrayType(this,count),b);z->parent=&array_container;
		return(ArrayType*)z;
	}
	int cmp(Type*const&a,Type*const&b){return cmp(a->name,b->name);}
	int cmp(Type*const&a,const string&b){return cmp(a->name,b);}
}