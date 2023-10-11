#include"../include/DerKern/Type.h"
namespace DerKern{
	TypeDesc TypeDesc::i8=Type("int8",1);TypeDesc TypeDesc::u8=TypeDesc("uint8",1);
	TypeDesc TypeDesc::i16=Type("int16",2);TypeDesc TypeDesc::u16=TypeDesc("uint16",2);
	TypeDesc TypeDesc::i32=Type("int32",4);TypeDesc TypeDesc::u32=TypeDesc("uint32",4);
	TypeDesc TypeDesc::i64=Type("int64",8);TypeDesc TypeDesc::u64=TypeDesc("uint64",8);
	TypeDesc TypeDesc::f=Type("float",sizeof(float));TypeDesc TypeDesc::d=TypeDesc("double",sizeof(double));
	TypeDesc TypeDesc::str=Type("string",sizeof(int*));TypeDesc TypeDesc::_type=TypeDesc("type",sizeof(TypeDesc));
	TypeDesc TypeDesc::func=Type("func",FuncHead_size);
	TypeDesc TypeDesc::idk[16];
	namespace{INITIALIZER(init)(){
		static const char _idk[]="idk0\0idk1\0idk2\0idk3\0idk4\0idk5\0idk6\0idk7\0idk8\0idk9\0idk10\0idk11\0idk12\0idk13\0idk14\0idk15";
		for(uint8_t i=0;i<50;i+=5)idk[i]=TypeDesc(_idk+i,8);for(uint8_t i=45;i<81;i+=6)idk[i]=TypeDesc(_idk+i);

	}}
}