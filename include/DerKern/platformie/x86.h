#pragma once
#include"../Instruction.h"
namespace DerKern{
	struct CompileState;
	namespace Instructions{//https://www.felixcloutier.com/x86/
		/*struct RetPop:Return0{
			uint16_t w;
			inline RetPop(uint16_t w){v=w;}
			bool compile(CompileState&)override;
		};*/
		constexpr uint8_t retSize=1,stackClaimMaxSize=6+(sizeof(int*)>>3),stackUnclaimMaxSize=stackClaimMaxSize;//this should make porting from x86 a bit less painful
		//return 0 on failure, siz must be 1/2/4/8
		namespace x86_64{//https://www.agner.org/optimize/optimizing_assembly.pdf
			inline void exposeRegisters(DerKern::Scope&s){
				s.set("eax",_Variable(&Type::u32,(uint8_t)0));s.set("ax",_Variable(&Type::u16,(uint8_t)0));s.set("al",_Variable(&Type::u8,(uint8_t)0));
				s.set("ecx",_Variable(&Type::u32,(uint8_t)1));s.set("cx",_Variable(&Type::u16,(uint8_t)1));s.set("cl",_Variable(&Type::u8,(uint8_t)1));
				s.set("edx",_Variable(&Type::u32,(uint8_t)2));s.set("dx",_Variable(&Type::u16,(uint8_t)2));s.set("dl",_Variable(&Type::u8,(uint8_t)2));
				s.set("ebx",_Variable(&Type::u32,(uint8_t)3));s.set("bx",_Variable(&Type::u16,(uint8_t)3));s.set("bl",_Variable(&Type::u8,(uint8_t)3));
				s.set("esp",_Variable(&Type::u32,(uint8_t)4));s.set("sp",_Variable(&Type::u16,(uint8_t)4));
				s.set("ebp",_Variable(&Type::u32,(uint8_t)5));s.set("bp",_Variable(&Type::u16,(uint8_t)5));
				s.set("esi",_Variable(&Type::u32,(uint8_t)6));s.set("si",_Variable(&Type::u16,(uint8_t)6));
				s.set("edi",_Variable(&Type::u32,(uint8_t)7));s.set("di",_Variable(&Type::u16,(uint8_t)7));
				if(sizeof(int*)>4){
					s.set("spl",_Variable(&Type::u8,(uint8_t)4));s.set("bpl",_Variable(&Type::u8,(uint8_t)5));
					s.set("sil",_Variable(&Type::u8,(uint8_t)6));s.set("dil",_Variable(&Type::u8,(uint8_t)7));
					s.set("rax",_Variable(&Type::u64,(uint8_t)0));
					s.set("rcx",_Variable(&Type::u64,(uint8_t)1));
					s.set("rdx",_Variable(&Type::u64,(uint8_t)2));
					s.set("rbx",_Variable(&Type::u64,(uint8_t)3));
					s.set("rsp",_Variable(&Type::u64,(uint8_t)4));
					s.set("rbp",_Variable(&Type::u64,(uint8_t)5));
					s.set("rsi",_Variable(&Type::u64,(uint8_t)6));
					s.set("rdi",_Variable(&Type::u64,(uint8_t)7));

					s.set("r8b",_Variable(&Type::u8,(uint8_t)8));s.set("r8w",_Variable(&Type::u16,(uint8_t)8));s.set("r8d",_Variable(&Type::u32,(uint8_t)8));s.set("r8",_Variable(&Type::u64,(uint8_t)8));
					s.set("r9b",_Variable(&Type::u8,(uint8_t)9));s.set("r9w",_Variable(&Type::u16,(uint8_t)9));s.set("r9d",_Variable(&Type::u32,(uint8_t)9));s.set("r9",_Variable(&Type::u64,(uint8_t)9));
					s.set("r10b",_Variable(&Type::u8,(uint8_t)10));s.set("r10w",_Variable(&Type::u16,(uint8_t)10));s.set("r10d",_Variable(&Type::u32,(uint8_t)10));s.set("r10",_Variable(&Type::u64,(uint8_t)10));
					s.set("r11b",_Variable(&Type::u8,(uint8_t)11));s.set("r11w",_Variable(&Type::u16,(uint8_t)11));s.set("r11d",_Variable(&Type::u32,(uint8_t)11));s.set("r11",_Variable(&Type::u64,(uint8_t)11));
					s.set("r12b",_Variable(&Type::u8,(uint8_t)12));s.set("r12w",_Variable(&Type::u16,(uint8_t)12));s.set("r12d",_Variable(&Type::u32,(uint8_t)12));s.set("r12",_Variable(&Type::u64,(uint8_t)12));
					s.set("r13b",_Variable(&Type::u8,(uint8_t)13));s.set("r13w",_Variable(&Type::u16,(uint8_t)13));s.set("r13d",_Variable(&Type::u32,(uint8_t)13));s.set("r13",_Variable(&Type::u64,(uint8_t)13));
					s.set("r14b",_Variable(&Type::u8,(uint8_t)14));s.set("r14w",_Variable(&Type::u16,(uint8_t)14));s.set("r14d",_Variable(&Type::u32,(uint8_t)14));s.set("r14",_Variable(&Type::u64,(uint8_t)14));
					s.set("r15b",_Variable(&Type::u8,(uint8_t)15));s.set("r15w",_Variable(&Type::u16,(uint8_t)15));s.set("r15d",_Variable(&Type::u32,(uint8_t)15));s.set("r15",_Variable(&Type::u64,(uint8_t)15));
				}
			}
			ENUM(Size,uint8_t)
				byte=1,word=2,dword=4
				#if defined(__x86_64__) || defined(_M_X64)
				,qword=8
				#endif
				ENUM_END inline bool validS(Size::T s){return s==1||s==2||s==4||(sizeof(int*)&s);}
			ENUM(Register,uint8_t)//https://www.swansontec.com/sregisters.html
				eax,ecx,edx,ebx,esp,ebp,esi,edi//Yeah it really goes in that order... a c d b... Let's teach Intel the alphabet!
				#if defined(__x86_64__) || defined(_M_X64)
				,r8,r9,r10,r11,r12,r13,r14,r15
				#endif
				//,data=-1//OH FU--- WAIT I CAN JUST SET THIS AND I'LL HAVE A POINTER TO MY VARIABLES AND S---? THIS SIMPLIFIES IT A LOT... Mostly... maybe... if I can ask the OS for a new one... Meh, can like just use r15 if necessary... Why did I not realize it?.. Ah right, no one told me and I strongly refused to look into those segments and s--- after all the OS creation attempt torture I sent myself through...
				ENUM_END
			ENUM(Flags,uint8_t)
				CF,OF,SF,//Carry,Overflow,result Sign bit
				ZF,//ZF=result==0
				
				//Exotic sh--
				//AF,//weirdo flag. HOW would I even calculate that?
				PF//PF=(result bit(where =1) count%2==0)
				ENUM_END
			ENUM(FRegister,uint8_t)xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15 ENUM_END inline bool validFR(FRegister::T r){return FRegister::count>r;}
			ENUM(FStack,uint8_t)st0,st1,st2,st3,st4,st5,st6,st7 ENUM_END inline bool validFSt(FStack::T r){return FStack::count>r;}
			ENUM(FSize,uint16_t)xmm=128,ymm=256,zmm=512 ENUM_END inline bool validFS(FSize::T s){return s==128||s==256||s==512;}
			inline bool validL(Location l){l=l.resolve();return l.type!=0&&(l.type==1||l.type==Location::Types::ptr||(l.type==2||l.type==3));}
			// bool adc(BBuf*,Location to,Location f,uint8_t siz);					//to+=f+CF;		OF	SF	ZF	AF	CF	PF
			// bool adc(BBuf*,Location to,uint32_t v,uint8_t siz);//Unfortunately sign extends siz=8
			// bool adcx(BBuf*,Register::T to,Location f,uint8_t siz);					//to+=f+CF;						CF		size=4/8

			// bool addpd(BBuf*,FRegister::T to,FLocation f);//to(double)[i]+=f(double)[i]	i=0,1
			// bool vaddpd(BBuf*,FRegister::T to,FRegister::T a,FLocation b,FSize::T);//to(double)[i]+=a(double)[i]+b(double)[i]	i=0,size(double)
			// //bool vaddpd( BBuf*,FRegister to,FRegister a,FLocation b,FSize);
			
			// bool adox(BBuf*,Register::T to,Location f,uint8_t siz);					//to+=f+CF;		OF						size=4/8

			// bool And(BBuf*,Location to,Location f,uint8_t siz);					//to&=f;		OF=0SF	ZF	AF=?CF=0PF
			// bool And(BBuf*,Location to,uint32_t v,uint8_t siz);//Unfortunately sign extends siz=8
			// bool andn(BBuf*,Register::T to,Register::T f1,Location f2,uint8_t siz);	//to=f2 &~f1;		SF	ZF				size=4/8

			// bool bextr(BBuf*,Register::T to,Location f1,Register::T f2,uint8_t siz);	//				OF=0SF=?ZF	AF=?CF=0PF=?size=4/8

			// bool blsi(BBuf*,Register::T to,Location f,uint8_t siz);	//to=lowest bit in f			OF=0SF	ZF	AF=?CF=fPF=?size=4/8
			uint16_t add(CompileState&,Location to,Location f,uint8_t siz);		//to+=f;		OF	SF	ZF	AF	CF	PF
			uint16_t addI(CompileState&,Location to,int32_t v,uint8_t siz);		//to+=f;		OF	SF	ZF	AF	CF	PF

			uint16_t call(CompileState&,Location);
			uint16_t jmp(CompileState&,Location);

			uint16_t lea(CompileState&,Location to,Location v);

			uint16_t mov(CompileState&,Location to,Location f,uint8_t siz);
			uint16_t movI(CompileState&,Location to,uint64_t v,uint8_t siz);

			uint16_t pop(CompileState&,Location v,uint8_t siz);
			uint16_t push(CompileState&,Location v,uint8_t siz);

			uint16_t sub(CompileState&,Location to,Location f,uint8_t siz);		//to+=f;		OF	SF	ZF	AF	CF	PF
			uint16_t subI(CompileState&,Location to,int32_t v,uint8_t siz);		//to+=f;		OF	SF	ZF	AF	CF	PF

			uint16_t xchg(CompileState&,Location to,Location f,uint8_t siz);


			uint16_t makeXMM(CompileState&,Location f1,Location f2);//https://stackoverflow.com/a/28380358 // full ModRM!
			//https://software.intel.com/sites/landingpage/IntrinsicsGuide/
		}
	}
}