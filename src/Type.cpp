#include"../include/DerKern/Type.h"
#include"Allocery.cpp"
#include"Erroring.cpp"
namespace DerKern{
	Type Type::i8=Type("int8",1);Type Type::u8=Type("uint8",1);
	Type Type::i16=Type("int16",2);Type Type::u16=Type("uint16",2);
	Type Type::i32=Type("int32",4);Type Type::u32=Type("uint32",4);
	Type Type::i64=Type("int64",8);Type Type::u64=Type("uint64",8);
	Type Type::f=Type("float",sizeof(float));Type Type::d=Type("double",sizeof(double));
	Type Type::str=Type("string",sizeof(int*));Type Type::_type=Type("type",sizeof(Type));
	Type Type::VOID=Type("void",1);
	Type Type::func_container=Type("<FUNCTION>",FuncHead_size);
	Type Type::idk[16];
	namespace{INITIALIZER(init){
		static const char _idk[]="idk0\0idk1\0idk2\0idk3\0idk4\0idk5\0idk6\0idk7\0idk8\0idk9\0idk10\0idk11\0idk12\0idk13\0idk14\0idk15";
		for(uint8_t i=0;i<50;i+=5)Type::idk[i]=Type(_idk+i,Type::PtrSize);for(uint8_t i=45;i<81;i+=6)Type::idk[i]=Type(_idk+i,Type::PtrSize);
		//Type::VOID.size=0;Type::VOID.ptr2Valid=Type::VOID.ptr3Valid=0;
	}}
	Ptr1Type*Type::pointer(){
		if(_pointer)return _pointer;
		#ifndef NO_SANITY
			if(name.size()>4&&name.substr(name.size()-4,4)=="****"){Error("Sanity Error");return 0;}
		#endif
		if(!ptr1Valid){Error("An attempt to have a * pointer with a type that doesn't allow it");return 0;}
		return _pointer=new Ptr1Type(this);
		
	}Ptr1Type*Type::ptr1_container=Type::VOID.pointer();
	Ptr2Type*Type::pointer2(){
		if(_pointer2)return _pointer2;
		if(!ptr2Valid){Error("An attempt to have a & pointer with a type that doesn't allow it");return 0;}
		return _pointer2=new Ptr2Type(this);
	}//Ptr2Type*Type::ptr2_container=Type::VOID.pointer2();
	Ptr3Type*Type::pointer3(){
		if(_pointer3)return _pointer3;
		if(!ptr3Valid){Error("An attempt to have a && pointer with a type that doesn't allow it");return 0;}
		return _pointer3=new Ptr3Type(this);
	}//Ptr3Type*Type::ptr3_container=Type::VOID.pointer3();
	ArrayType*Type::array(uint32_t count){
		string s=ArrayType::NAME(this,count);
		uint16_t b;if(BinSearch<Type*,string,cmp,uint16_t>(0,b,_arrays.count,_arrays.raw,s))return(ArrayType*)_arrays[b];
		return(ArrayType*)_arrays._insert(new ArrayType(s,this,count),b);
	}
	FuncType*Type::func(Type**argv,uint8_t argc){
		string s=FuncType::NAME(this,argv,argc);
		uint16_t b;if(BinSearch<Type*,string,cmp,uint16_t>(0,b,_funcs.count,_funcs.raw,s))return(FuncType*)_funcs[b];
		return(FuncType*)_funcs._insert(new FuncType(s,this,argv,argc),b);
	}
	int cmp(Type*const&a,Type*const&b){return cmp(a->name,b->name);}
	int cmp(Type*const&a,const string&b){return cmp(a->name,b);}
}