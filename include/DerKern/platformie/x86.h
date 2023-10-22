#pragma once
#include"../Instruction.h"
namespace DerKern{
	struct CompileState;
	namespace Instructions{//https://www.felixcloutier.com/x86/
		//return 0 on failure, siz must be 1/2/4/8
		namespace x86_64{
			ENUM(Size,uint8_t)
				byte=1,word=2,dword=4
				#if _WIN64||__x86_64__||__ppc64__
				,qword=8
				#endif
				ENUM_END inline bool validS(Size::T s){return s==1||s==2||s==4||(sizeof(int*)&s);}
			ENUM(Register,uint8_t)
				eax=0,ecx,edx,ebx,esp,ebp,esi,edi,//Yeah it really goes in that order... a c d b
				#if _WIN64||__x86_64__||__ppc64__
				r8,r9,r10,r11,r12,r13,r14,r15,
				#endif
				count//,high=16//high bit means you want the other half of register
				ENUM_END
			ENUM(Flags,uint8_t)
				CF=0,OF,SF,//Carry,Overflow,result Sign bit
				AF,ZF,PF//weirdo flag,Zero==result,(result bit(where =1) count%2==0)
				ENUM_END
			ENUM(FRegister,uint8_t)xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15,count ENUM_END inline bool validFR(FRegister::T r){return FRegister::count>r;}
			ENUM(FStack,uint8_t)st0,st1,st2,st3,st4,st5,st6,st7,count ENUM_END inline bool validFSt(FStack::T r){return FStack::count>r;}
			ENUM(FSize,uint16_t)xmm=128,ymm=256,zmm=512 ENUM_END inline bool validFS(FSize::T s){return s==128||s==256||s==512;}
			inline bool validL(Location l){l=l.resolve();return l.type!=0&&(l.type==1||l.type==Location::Types::ptr||(l.type==2||l.type==3));}
			// bool adc(BBuf*,Location to,Location f,uint8_t siz);					//to+=f+CF;		OF	SF	ZF	AF	CF	PF
			// bool adc(BBuf*,Location to,uint32_t v,uint8_t siz);//Unfortunately sign extends siz=8
			// bool adcx(BBuf*,Register::T to,Location f,uint8_t siz);					//to+=f+CF;						CF		size=4/8
			// bool add(BBuf*,Location to,Location f,uint8_t siz);					//to+=f;		OF	SF	ZF	AF	CF	PF
			// bool add(BBuf*,Location to,uint32_t v,uint8_t siz);//Unfortunately sign extends siz=8
			// bool addpd(BBuf*,FRegister::T to,FLocation f);//to(double)[i]+=f(double)[i]	i=0,1
			// bool vaddpd(BBuf*,FRegister::T to,FRegister::T a,FLocation b,FSize::T);//to(double)[i]+=a(double)[i]+b(double)[i]	i=0,size(double)
			// //bool vaddpd( BBuf*,FRegister to,FRegister a,FLocation b,FSize);
			
			// bool adox(BBuf*,Register::T to,Location f,uint8_t siz);					//to+=f+CF;		OF						size=4/8

			// bool And(BBuf*,Location to,Location f,uint8_t siz);					//to&=f;		OF=0SF	ZF	AF=?CF=0PF
			// bool And(BBuf*,Location to,uint32_t v,uint8_t siz);//Unfortunately sign extends siz=8
			// bool andn(BBuf*,Register::T to,Register::T f1,Location f2,uint8_t siz);	//to=f2 &~f1;		SF	ZF				size=4/8

			// bool bextr(BBuf*,Register::T to,Location f1,Register::T f2,uint8_t siz);	//				OF=0SF=?ZF	AF=?CF=0PF=?size=4/8

			// bool blsi(BBuf*,Register::T to,Location f,uint8_t siz);	//to=lowest bit in f			OF=0SF	ZF	AF=?CF=fPF=?size=4/8
			bool add(CompileState&,Location to,Location f,uint8_t siz);
			bool addI(CompileState&,Location to,int32_t v,uint8_t siz);

			bool mov(CompileState&,Location to,Location f,uint8_t siz);

			bool pop(CompileState&,Location v,uint8_t siz);
			bool push(CompileState&,Location v,uint8_t siz);

			bool xchg(CompileState&,Location to,Location f,uint8_t siz);
		}
	}
}