#pragma once
#include"../Compile.h"

namespace DerKern{
	namespace Simpler{
		struct insLocSized{
			Instructionary*ins;Location loc;uint8_t siz;
			inline insLocSized(Instructionary*i,Location l,uint8_t s){ins=i;loc=l;siz=s;}
			inline void set(int64_t v){
				if(siz==1){ins->add<Instructions::Set8>(loc,(uint8_t)v);return;}
				if(siz==2){ins->add<Instructions::Set16>(loc,(uint16_t)v);return;}
				if(siz==4){ins->add<Instructions::Set32>(loc,(uint32_t)v);return;}
				if(siz==8){ins->add<Instructions::Set64>(loc,(uint64_t)v);return;}
				printf("Impossible operation =: invalid size");
			}
			template<typename t>inline void operator=(t);

			inline void add(int64_t v){
				assert(siz!=8);//if(siz==8){ins->add<Instructions::Add64_1(loc,(uint64_t)v);return;}
				if(siz==1){ins->add<Instructions::Add8_1>(loc,(uint8_t)v);return;}
				if(siz==2){ins->add<Instructions::Add16_1>(loc,(uint16_t)v);return;}
				if(siz==4){ins->add<Instructions::Add32_1>(loc,(uint32_t)v);return;}
				printf("Impossible operation +=: invalid size");
			}inline void operator+=(int64_t v){add(v);}inline void operator+=(uint64_t v){add((int64_t)v);}
		};
		template<>inline void insLocSized::operator=(uint64_t v){set((int64_t)v);}template<>inline void insLocSized::operator=(int64_t v){set(v);}
		template<>inline void insLocSized::operator=( uint8_t v){insLocSized::operator=((uint64_t)v);}template<>inline void insLocSized::operator=( int8_t v){insLocSized::operator=((int64_t)v);}
		template<>inline void insLocSized::operator=(uint16_t v){insLocSized::operator=((uint64_t)v);}template<>inline void insLocSized::operator=(int16_t v){insLocSized::operator=((int64_t)v);}
		template<>inline void insLocSized::operator=(uint32_t v){insLocSized::operator=((uint64_t)v);}template<>inline void insLocSized::operator=(int32_t v){insLocSized::operator=((int64_t)v);}
		template<>inline void insLocSized::operator=(Location v){ins->add<Instructions::Set>(loc,v,siz);}
		template<>inline void insLocSized::operator=(Location*v){ins->add<Instructions::Set>(loc,v,siz);}
		typedef pair<Location,uint8_t>locSized;struct insLocMemb;struct locMemb;struct insLocTyped;
		struct insLoc{
			Instructionary*ins;Location loc;
			inline insLoc(Instructionary*i,Location l){ins=i;loc=l;}
			inline insLocSized size(uint8_t s){return{ins,loc,s};}inline insLocSized operator()(uint8_t s){return size(s);}

			template<typename t>inline void operator=(t);
			inline void operator=(locSized l){ins->add<Instructions::Set>(loc,l.a,l.b);}
			inline void operator=(locMemb l);
			inline void operator=(Instructions::NopO*o){ins->add<Instructions::Set>(loc,o->out,o->outSize);}

			inline void operator+=( uint8_t v){ins->add<Instructions:: Add8_1>(loc,v);}	inline void operator+=( int8_t v){ins->add<Instructions:: Add8_1>(loc,(uint8_t)v);}
			inline void operator+=(uint16_t v){ins->add<Instructions::Add16_1>(loc,v);}	inline void operator+=(int16_t v){ins->add<Instructions::Add16_1>(loc,(uint16_t)v);}
			inline void operator+=( int32_t v){ins->add<Instructions::Add32_1>(loc,(uint32_t)v);}
			inline void operator+=(locSized l){ins->add<Instructions::Add_1>(loc,l.a,l.b);}
			inline void operator+=(Instructions::NopO*o){ins->add<Instructions::Add_1>(loc,o->out,o->outSize);}
			inline insLocMemb operator[](Type::Memba&);inline insLocMemb operator[](Type::Memba*m);
			inline insLocTyped operator[](Type*t);
		};}inline Simpler::insLoc Instructionary::operator[](Location l){return{this,l};}namespace Simpler{
		template<>inline void insLoc::operator=( uint8_t v){ins->add<Instructions:: Set8>(loc,v);}	template<>inline void insLoc::operator=( int8_t v){ins->add<Instructions:: Set8>(loc,(uint8_t)v);}
		template<>inline void insLoc::operator=(uint16_t v){ins->add<Instructions::Set16>(loc,v);}	template<>inline void insLoc::operator=(int16_t v){ins->add<Instructions::Set16>(loc,(uint16_t)v);}
		template<>inline void insLoc::operator=(uint32_t v){ins->add<Instructions::Set32>(loc,v);}	template<>inline void insLoc::operator=(int32_t v){ins->add<Instructions::Set32>(loc,(uint32_t)v);}
		template<>inline void insLoc::operator=(uint64_t v){ins->add<Instructions::Set64>(loc,v);}	template<>inline void insLoc::operator=(int64_t v){ins->add<Instructions::Set64>(loc,(uint64_t)v);}
		#ifdef type_stuff___H
			inline void operator+=(insLoc i,Type*t){i.ins->add<Instructions::Constry>(_Variable{t,i.loc});}
			inline void operator-=(insLoc i,Type*t){i.ins->add<Instructions::Assassinate>(_Variable{t,i.loc});}
			struct insLocMemb{
				insLoc i;Type::Memba*mem;
				inline insLocMemb(Instructionary*i,Location l,Type::Memba*m):i(i,l){mem=m;}
				inline insLocMemb(insLoc _,Type::Memba*m):i(_){mem=m;}
				inline void operator=(Location l){i.ins->add<Instructions::MovM>(i.loc,mem,l);}
			};inline insLocMemb insLoc::operator[](Type::Memba&m){return{*this,&m};}inline insLocMemb insLoc::operator[](Type::Memba*m){return{*this,m};}
			struct locMemb{
				Location loc;Type::Memba*mem;
				inline locMemb(Location l,Type::Memba&m){loc=l;mem=&m;}
				inline locMemb(Location l,Type::Memba*m){loc=l;mem=m;}
				inline void operator=(insLoc l){insLocMemb({l.ins,loc},mem)=l.loc;}
			};inline void insLoc::operator=(locMemb l){ins->add<Instructions::GetM>(loc,l.mem,l.loc);}
			struct insLocTyped{
				insLoc i;Type*typ;
				inline insLocTyped(insLoc _,Type*t):i(_){typ=t;}
				inline insLocMemb operator[](string s){return i[typ->memba(s)];}
				inline void construct(){i+=typ;}inline void destruct(){i-=typ;}
			};inline insLocTyped insLoc::operator[](Type*t){return{*this,t};}
		#endif
	}
}