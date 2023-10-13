#include"../include/DerKern/Type.h"
namespace DerKern{
	Type Type::i8=Type("int8",1);Type Type::u8=Type("uint8",1);
	Type Type::i16=Type("int16",2);Type Type::u16=Type("uint16",2);
	Type Type::i32=Type("int32",4);Type Type::u32=Type("uint32",4);
	Type Type::i64=Type("int64",8);Type Type::u64=Type("uint64",8);
	Type Type::f=Type("float",sizeof(float));Type TypeDesc::d=Type("double",sizeof(double));
	Type Type::str=Type("string",sizeof(int*));Type Type::_type=Type("type",sizeof(Type));
	Type Type::ptr=Type("<*>",Type::PtrSize);Type Type::ptr2=Type("<&>",Type::PtrSize);
	Type Type::func=Type("<FUNCTION>",FuncHead_size);
	Type Type::idk[16];
	namespace{INITIALIZER(init)(){
		static const char _idk[]="idk0\0idk1\0idk2\0idk3\0idk4\0idk5\0idk6\0idk7\0idk8\0idk9\0idk10\0idk11\0idk12\0idk13\0idk14\0idk15";
		for(uint8_t i=0;i<50;i+=5)idk[i]=Type(_idk+i,8);for(uint8_t i=45;i<81;i+=6)idk[i]=Type(_idk+i);

	}}
	Type*Type::pointer(){
		if(!_pointer)_pointer=new Type(name+'*',PtrSize);return _pointer;
	}
	Type*Type::pointer2(){
		if(!_pointer)_pointer=new Type(name+'&',PtrSize);return _pointer;
	}
	Type*Type::array(uint32_t count){
		char _[9];sprintf(_,"%x",count);
		string s=name+'['+_+']';

		sizeT b;if(BinSearch<Type*,string,cmp>(0,b,_arrays.count-1,_arrays.raw,s))return _arrays[b];
		return _arrays._insert(new Type(s,size*count),b);
	}
	Type*Type::func(Type**argv,uint8_t argc){
		string s=FuncType::NAME(this,argv,argc);

		sizeT b;if(BinSearch<Type*,string,cmp>(0,b,_funcs.count-1,_funcs.raw,s))return _funcs[b];
		return _funcs._insert(new FuncType(s,this,argv,argc),b);
	}
	int cmp(const Type*&a,const Type*&b){return cmp(a->name,b->name);}
	int cmp(const Type*&a,const string&b){return cmp(a->name,b);}
}