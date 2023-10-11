#pragma once
#include"../Instruction.h"
namespace DerKern{
	struct CompileState;struct EvalState;
	namespace Instructions{//https://www.felixcloutier.com/x86/
		//return 0 on failure, siz must be 1/2/4/8
		namespace x86_64{
			enum Size:uint8_t{
				byte=1,word=2,dword=4
				#if _WIN64||__x86_64__||__ppc64__
				,qword=8
				#endif
			};inline bool validS(Size s){return s==1||s==2||s==4||(sizeof(int*)&s);}
			enum Register:uint8_t{
				eax=0,ecx,edx,ebx,esp,ebp,esi,edi,//Yeah it really goes in that order... a c d b
				#if _WIN64||__x86_64__||__ppc64__
				r8,r9,r10,r11,r12,r13,r14,r15,
				#endif
				count//,high=16//high bit means you want the other half of register
			};inline bool validR(Register r){return Register.count>r;}
			inline bool validL(Location l){return l.type!=0&&((r.type&2)||validR(l.reg));}
			enum Flags:uint8_t{
				CF=0,OF,SF//Carry,Overflow,result Sign bit
				AF,ZF,PF//weirdo flag,Zero==result,(result bit(=1) number%2==0)
			};
			enum FRegisters:uint8_t{xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15,count};inline bool validFR(FRegister r){return FRegister.count>r;}
			enum FStack:uint8_t{st0,st1,st2,st3,st4,st5,st6,st7,count};inline bool validFSt(FStack r){return FStack.count>r;}
			enum FSize:uint8_t{xmm=128,ymm=256,zmm=512};inline bool validFS(FSize s){return s==128||s==256||s==512;}
			bool adc	(BBuf*,Location to,Location f,uint8_t siz);					//to+=f+CF;		OF	SF	ZF	AF	CF	PF
			bool adc	(BBuf*,Location to,uint32_t v,uint8_t siz);//Unfortunately sign extends siz=8
			bool adcx	(BBuf*,Register to,Location f,uint8_t siz);					//to+=f+CF;						CF		size=4/8
			bool add	(BBuf*,Location to,Location f,uint8_t siz);					//to+=f;		OF	SF	ZF	AF	CF	PF
			bool add	(BBuf*,Location to,uint32_t v,uint8_t siz);//Unfortunately sign extends siz=8
			bool addpd	(BBuf*,FRegister to,FLocation f)//to(double)[i]+=f(double)[i]	i=0,1
			bool vaddpd( BBuf*,FRegister to,FRegister a,FLocation b,FSize);//to(double)[i]+=a(double)[i]+b(double)[i]	i=0,size(double)
			//bool vaddpd( BBuf*,FRegister to,FRegister a,FLocation b,FSize);
			
			bool adox	(BBuf*,Register to,Location f,uint8_t siz);					//to+=f+CF;		OF						size=4/8

			bool and	(BBuf*,Location to,Location f,uint8_t siz);					//to&=f;		OF=0SF	ZF	AF=?CF=0PF
			bool and	(BBuf*,Location to,uint32_t v,uint8_t siz);//Unfortunately sign extends siz=8
			bool andn	(BBuf*,Register to,Register f1,Location f2,uint8_t siz);	//to=f2 &~f1;		SF	ZF				size=4/8

			bool bextr	(BBuf*,Register to,Location f1,Register f2,uint8_t siz);	//				OF=0SF=?ZF	AF=?CF=0PF=?size=4/8

			bool blsi	(BBuf*,Register to,Location f,uint8_t siz);	//to=lowest bit in f			OF=0SF	ZF	AF=?CF=fPF=?size=4/8
		}
	}
}