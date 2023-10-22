#pragma once
#include"../Instruction.cpp"
#include"../../include/DerKern/platformie/x86.h"
#include"../compile.cpp"
bool DerKern::Instruction::compile(CompileState&s){(*s.b)+=0xC3;return 1;}
namespace DerKern::Instructions{
	bool Return1::compile(CompileState&s){bef->compile(s);(*s.b)+=0xC3;return 1;}
	bool Pong::compile(CompileState&s){fprintf(stderr,"!!!pong is NOT complete!!!\n");throw std::exception();return 0;}
	bool jmpLine::compile(CompileState&s){fprintf(stderr,"!!!jmpLine is NOT complete!!!\n");throw std::exception();return 0;}
	namespace x86_64{
		inline bool op1rm(uint8_t op,BBuf*b,uint8_t to,Location f,uint8_t s){//https://defuse.ca/online-x86-assembler.htm
			if(f.type==0||Register::count<=to||Register::count<=f.reg)return 0;
			uint8_t z[10];
			if(s==2)b->append(0x66);
			if(to>7||(f.type!=1&&f.reg>7)||s==8)b->append(0x40|(f.type==1?to>7?4:0:((to>7?4:0)|(f.reg>7)))|(s&8));
			z[0]=op|(s>1);
			if(f.type==3){
				if(f.imov!=0||(f.reg&7)==5){
					if(~0xff&(uint32_t)f.imov){
						z[1]=(f.reg&7)|((to&7)<<3)|0x80;
						if((f.reg&7)==4){
							z[2]=0x24;memcpy(z+3,&f.imov,4);
							b->append(z,7);
						}else{memcpy(z+2,&f.imov,4);b->append(z,6);}
					}else{
						z[1]=(f.reg&7)|((to&7)<<3)|0x40;
						if((f.reg&7)==4){
							z[2]=0x24;z[3]=(uint8_t)f.imov;
							b->append(z,4);
						}else{z[2]=(uint8_t)f.imov;b->append(z,3);}
					}
				}else{
					z[1]=(f.reg&7)|((to&7)<<3);
					if((f.reg&7)==4){
						z[2]=0x24;
						b->append(z,3);
					}else b->append(z,2);
				}
				return 1;
			}else if(f.type==1){
				z[1]=4|((to)<<3);z[2]=0x25;
				memcpy(z+3,&f.imov,4);
				b->append(z,7);
				return 1;
			}
			z[1]=0xc0|(f.reg&7)|((to&7)<<3);
			b->append(z,2);
			return 1;
		}
		inline bool op1rm32(uint8_t op1,uint8_t op2,uint8_t op3,uint8_t op4,BBuf*b,uint8_t to,Location f,uint8_t s){//adcx,adox
			if(f.type==0||Register::count<=to||Register::count<=f.reg||(s!=4&&(sizeof(int*)==4||s!=8)))return 0;
			uint8_t z[10];
			z[0]=op1;
			if(s==8||(f.type!=1&&f.reg>7)||to>7){
				z[1]=0x40|(f.type==1?(to>7)*4:((to>7?4:0)|(f.reg>7)))|(s&8);
				z[2]=op2;z[3]=op3;z[4]=op4;b->append(z,5);
			}else{
				z[1]=op2;z[2]=op3;z[3]=op4;b->append(z,4);
			}
			if(f.type==3){
				if(f.imov>0||(f.reg&7)==5){
					if(~0xff&(uint32_t)f.imov){
						z[0]=(f.reg&7)|((to&7)<<3)|0x80;
						if((f.reg&7)==4){
							z[1]=0x24;memcpy(z+2,&f.imov,4);
							b->append(z,6);
						}else{memcpy(z+1,&f.imov,4);b->append(z,5);}
					}else{
						z[0]=(f.reg&7)|((to&7)<<3)|0x40;
						if((f.reg&7)==4){
							z[1]=0x24;z[2]=(uint8_t)f.imov;
							b->append(z,3);
						}else{z[1]=(uint8_t)f.imov;b->append(z,2);}
					}
				}else{
					if((f.reg&7)==4){
						z[0]=(f.reg&7)|((to&7)<<3);
						z[1]=0x24;
						b->append(z,2);
					}else b->append((f.reg&7)|((to&7)<<3));
				}
				return 1;
			}else if(f.type==1){
				z[0]=4|((to&7)<<3);z[1]=0x25;
				memcpy(z+2,&f.imov,4);
				b->append(z,6);
				return 1;
			}
			b->append(0xc0|(f.reg&7)|((to&7)<<3));
			return 1;
		}
		inline bool op1rvm(uint8_t op,BBuf*b,uint8_t to,uint8_t f1,Location f2,uint8_t siz){//andn,bextr
			if(Register::count<=to||Register::count<=f1||f2.type==0||Register::count<=f2.reg||(siz!=4&&(sizeof(int*)==4||siz!=8)))return 0;
			uint8_t z[10];
			z[0]=0xc4;
			z[1]=0xe2-((to&8)<<4)-(((f2.type&2)&&(f2.reg&8))*0x20);
			z[2]=(0x78-(f1<<3))|((siz&8)<<4);
			z[3]=op;
			if(f2.type==3){
				if(f2.imov>0||(f2.reg&7)==5){
					if(~0xff&(uint32_t)f2.imov){
						z[4]=(to&7)|((f2.reg&7)<<3)|0x80;
						if((f2.reg&7)==4){
							z[5]=0x24;memcpy(z+6,&f2.imov,4);
							b->append(z,10);
						}else{memcpy(z+5,&f2.imov,4);b->append(z,9);}
					}else{
						z[4]=(to&7)|((f2.reg&7)<<3)|0x40;
						if((f2.reg&7)==4){
							z[5]=0x24;z[6]=(uint8_t)f2.imov;
							b->append(z,7);
						}else{z[5]=(uint8_t)f2.imov;b->append(z,6);}
					}
				}else{
					if((f2.reg&7)==4){
						z[4]=(to&7)|((f2.reg&7)<<3);
						z[5]=0x24;
						b->append(z,6);
					}else{
						z[4]=(to&7)|((f2.reg&7)<<3);
						b->append(z,5);
					}
				}
				return 1;
			}else if(f2.type==1){
				z[4]=4|((to&7)<<3);z[5]=0x25;
				memcpy(z+6,&f2.imov,4);
				b->append(z,10);
				return 1;
			}
			z[4]=0xc0|(f2.reg&7)|((to&7)<<3);
			b->append(z,5);
			return 1;
		}
		inline bool op1mi(uint8_t op,uint8_t op2,BBuf*b,Location to,uint32_t v,uint8_t s){
			if(!validL(to)||!validS(s))return 0;
			uint8_t s2=(v&~0x7f)==0?1:s==2?2:4;
			if(s==2)b->append(0x66);
			if(to.reg>7||s==8)b->append(0x40|(to.reg>7)|(s&8));
			b->append(op|(s>1?(s2==1)*2+1:0));
			if(to.type==5){
				if(to.imov>0||(to.reg&7)==5){
					if(~0xff&(uint32_t)to.imov){
						b->append(to.reg|op2|0x80);
						b->append((uint8_t*)&to.imov,4);
					}else{
						b->append(to.reg|op2|0x40);
						b->append((uint8_t)to.imov);
					}
				}else{b->append(to.reg|op2);if(to.reg==4)b->append(0x24);}
			}else if(to.type==1){
				b->append(4|op2);b->append(0x25);
				b->append((uint8_t*)&to.imov,4);
			}else b->append(0xc0|to.reg|op2);
			b->append((uint8_t*)&v,s2);
			return 1;
		}
		inline bool op1i(uint8_t op,BBuf*b,uint32_t v,uint8_t s){//to=eax
			if(!validS(s))return 0;
			uint8_t s2=(v&~0x7f)==0?1:s==2?2:4;
			if(s==2)b->append(0x66);else if(s==8)b->append(0x48);
			b->append(op|(s>1));b->append((uint8_t*)&v,s2);
			return 1;
		}
		inline bool op1vm(uint8_t op2,BBuf*b,uint8_t to,Location f,uint8_t siz){//blsi
			if(to>=Register::count||!validL(f)||!validS(siz))return 0;
			uint8_t z[10];
			z[0]=0xc4;
			z[1]=0xe2-(((f.type&2)&&(f.reg&8))*0x20);
			z[2]=(0x78-(to<<3))|((siz&8)<<4);
			z[3]=0xf3;
			if(f.type==3){
				if(f.imov>0||(f.reg&7)==5){
					if(~0xff&(uint32_t)f.imov){
						z[4]=op2|((f.reg&7)<<3)|0x80;
						if((f.reg&7)==4){
							z[5]=0x24;memcpy(z+6,&f.imov,4);
							b->append(z,10);
						}else{memcpy(z+5,&f.imov,4);b->append(z,9);}
					}else{
						z[4]=op2|((f.reg&7)<<3)|0x40;
						if((f.reg&7)==4){
							z[5]=0x24;z[6]=(uint8_t)f.imov;
							b->append(z,7);
						}else{z[5]=(uint8_t)f.imov;b->append(z,6);}
					}
				}else{
					if((f.reg&7)==4){
						z[4]=op2|((f.reg&7)<<3);
						z[5]=0x24;
						b->append(z,6);
					}else{
						z[4]=op2|((f.reg&7)<<3);
						b->append(z,5);
					}
				}
				return 1;
			}else if(f.type==1){
				z[4]=4|op2;z[5]=0x25;
				memcpy(z+6,&f.imov,4);
				b->append(z,10);
				return 1;
			}
			z[4]=0xc0|op2|(f.reg&7);
			b->append(z,5);
			return 1;
		}
		// bool adc(BBuf*b,Location to,Location f,uint8_t s){
		// 	constexpr uint8_t opc=16;
		// 	if(to.type==2&&f.type!=0)return op1rm(opc|2,b,to.reg,f,s);
		// 	if((to.type&1)&&f.type==2)return op1rm(opc,b,f.reg,to,s);
		// }bool adc(BBuf*b,Location to,uint32_t v,uint8_t s){constexpr uint8_t opc=16;return to.type!=0&&((to.type==2&&to.reg==0&&(s==1||(v&~0xff)!=0))?op1i(4|opc,b,v,s):op1mi(0x80,opc,b,to,v,s));}
		// bool adcx(BBuf*b,uint8_t to,Location f,uint8_t s){return op1rm32(0x66,15,0x38,0xf6,b,to,f,s);}
		// bool add(BBuf*b,Location to,Location f,uint8_t s){
		// 	constexpr uint8_t opc=0;
		// 	if(to.type==2&&f.type!=0)return op1rm(opc|2,b,to.reg,f,s);
		// 	if((to.type&1)&&f.type==2)return op1rm(opc,b,f.reg,to,s);
		// }bool add(BBuf*b,Location to,uint32_t v,uint8_t s){constexpr uint8_t opc=0;return to.type!=0&&((to.type==2&&to.reg==0&&(s==1||(v&~0xff)!=0))?op1i(4|opc,b,v,s):op1mi(0x80,opc,b,to,v,s));}

		// bool adox(BBuf*b,uint8_t to,Location f,uint8_t s){return op1rm32(0xf3,15,0x38,0xf6,b,to,f,s);}

		// bool And(BBuf*b,Location to,Location f,uint8_t s){
		// 	constexpr uint8_t opc=32;
		// 	if(to.type==2&&f.type!=0)return op1rm(opc|2,b,to.reg,f,s);
		// 	if((to.type&1)&&f.type==2)return op1rm(opc,b,f.reg,to,s);
		// }bool And(BBuf*b,Location to,uint32_t v,uint8_t s){constexpr uint8_t opc=32;return to.type!=0&&((to.type==2&&to.reg==0&&(s==1||(v&~0xff)!=0))?op1i(4|opc,b,v,s):op1mi(0x80,opc,b,to,v,s));}
		// bool andn(BBuf*b,uint8_t to,uint8_t f1,Location f2,uint8_t s){return op1rvm(0xf2,b,to,f1,f2,s);}
		// bool bextr(BBuf*b,uint8_t to,Location f2,uint8_t f1,uint8_t s){return op1rvm(0xf7,b,to,f1,f2,s);}
		// bool blsi(BBuf*b,uint8_t to,Location f,uint8_t s){return op1vm(24,b,to,f,s);}
		bool add(CompileState&c,Location to,Location f,uint8_t s){
			constexpr uint8_t opc=0;
			if(to.type==2&&f.type!=0){

				return op1rm(opc|2,c.b,to.reg,f,s);
			}
			if((to.type&1)&&f.type==2){

				return op1rm(opc,c.b,f.reg,to,s);
			}
		}
		bool addI(CompileState&c,Location to,int32_t v,uint8_t s){
			constexpr uint8_t opc=0;
			return to.type!=0&&((to.type==2&&to.reg==0&&(s==1||(v&~0xff)!=0))?op1i(4|opc,c.b,v,s):op1mi(0x80,opc,c.b,to,v,s));
		}

		bool call(CompileState&c,Location l){
			if(l.type==0)return 0;
			if(l.type==1){(*c.b)+=0xe8;c.b->append((uint8_t*)&l.imov,4);}//Here's the thing... It's relative...
			if(l.type==2){
				if(l.reg>7)(*c.b)+=0x41;
				c.b->ensure(2);
				c.b->raw[c.b->count]=0xff;
				c.b->raw[c.b->count+1]=0xd0+(l.reg&7);
				c.b->count++;
				return 1;
			}else if(l.type==2){//I feel like I'll have a hard time with it...
				if(l.reg>7)(*c.b)+=0x41;
				c.b->ensure(2);
				c.b->raw[c.b->count]=0xff;
				c.b->raw[c.b->count+1]=0xd0+(l.reg&7);
				c.b->count++;
				return 1;
			}
		}

		bool mov(CompileState&c,Location to,Location f,uint8_t s){
			if(to==f&&!(s==4&&(sizeof(int*)==4||to.type==2)))return 1;
			constexpr uint8_t opc=0x88;
			if(to.type==2&&f.type!=0){

				return op1rm(opc|2,c.b,to.reg,f,s);
			}
			if((to.type&1)&&f.type==2){

				return op1rm(opc,c.b,f.reg,to,s);
			}
		}

		bool xchg(CompileState&c,Location to,Location f,uint8_t s){
			if(to==f&&!(s==4&&(sizeof(int*)==4||to.type==2)))return 1;
			constexpr uint8_t opc=0x86;
			if(to.type==2&&f.type!=0){
				return op1rm(opc|2,c.b,to.reg,f,s);
			}
			if((to.type&1)&&f.type==2){

				return op1rm(opc,c.b,f.reg,to,s);
			}
		}

		bool pop(CompileState&c,Location v,uint8_t siz){
			if(v.type==2){
				if(siz==sizeof(int*)){
					if(v.reg&8)(*c.b)+=0x41;
					(*c.b)+=0x58+(v.reg&7);
					return 1;
				}
			}
		}
		bool push(CompileState&c,Location v,uint8_t siz){
			if(v.type==2){
				if(siz==sizeof(int*)){
					if(v.reg&8)(*c.b)+=0x41;
					(*c.b)+=0x50+(v.reg&7);
					return 1;
				}
			}
		}
	}
	bool Add::compile(CompileState&c){
		using namespace x86_64;
		Location Out=out.resolve(),In=in.resolve(),In2=in2.resolve();
		if((Out.type&2)||((In.type&2)&&(In2.type&2))){if(!mov(c,Out,In,siz))return 0;}
		else{
			uint8_t _;
			if(c.UnusedRegister(&_))return mov(c,_,In,siz)&&add(c,_,In2,siz)&&mov(c,Out,_,siz);
			for(uint8_t i=0;i<3;i++)if(((In.type&2)&&In.reg!=i)&&((In2.type&2)&&In2.reg!=i)){
				_=(uint8_t)i;return xchg(c,_,Out,siz)&&mov(c,_,In,siz)&&add(c,_,In2,siz)&&xchg(c,_,Out,siz);
			}
		}
		return add(c,Out,In2,siz);
	}
	bool TransitionTo::compile(CompileState&c){
		using namespace x86_64;
		#if _WIN64//I simply need a way to get the first argument to ebp. Thanks https://en.wikipedia.org/wiki/X86_calling_conventions
			if(sizeof(int*)==8)xchg(c,(uint8_t)5,(uint8_t)0,sizeof(int*));// HELLO "Microsoft x64 calling convention", YOU BEING ENFORCED ONTO ME HERE SEEMS PROBLEMATIC.
		#else 
			if(sizeof(int*)==8)xchg(c,(uint8_t)5,(uint8_t)7,sizeof(int*));// HELLO "System V AMD64 ABI", YOU TOO ARE BEING ENFORCED... WHY ARE YOU USING rdi AND ONLY THEN rsi BY THE WAY?
		#endif
		else xchg(c,(uint8_t)5,Location(4,sizeof(int*)),sizeof(int*));

		for(uint8_t i=0;i<5;i++)xchg(c,(uint8_t)i,Location(5,i*sizeof(int*)),sizeof(int*));
		for(uint8_t i=6;i<(sizeof(int*)<<1);i++)xchg(c,(uint8_t)i,Location(5,i*sizeof(int*)),sizeof(int*));
		return 1;
	}
	bool TransitionFrom::compile(CompileState&c){
		using namespace x86_64;
		for(uint8_t i=0;i<5;i++)xchg(c,(uint8_t)i,Location(5,i*sizeof(int*)),sizeof(int*));
		for(uint8_t i=6;i<(sizeof(int*)<<1);i++)xchg(c,(uint8_t)i,Location(5,i*sizeof(int*)),sizeof(int*));

		#if _WIN64//I simply need a way to restore ebp. Thanks https://en.wikipedia.org/wiki/X86_calling_conventions
			if(sizeof(int*)==8)mov(c,(uint8_t)5,(uint8_t)0,sizeof(int*));// HELLO "Microsoft x64 calling convention", YOU BEING ENFORCED ONTO ME HERE SEEMS PROBLEMATIC.
		#else 
			if(sizeof(int*)==8)mov(c,(uint8_t)5,(uint8_t)7,sizeof(int*));// HELLO "System V AMD64 ABI", YOU TOO ARE BEING ENFORCED... WHY ARE YOU USING rdi AND ONLY THEN rsi BY THE WAY?
		#endif
		else mov(c,(uint8_t)5,Location(4,sizeof(int*)),sizeof(int*));

		(*c.b)+=0xC3;//ret
		return 1;
	}
	bool _call(CompileState&s,void*f){
		using namespace x86_64;

		//Thanks https://stackoverflow.com/a/5273354
		#if UINTPTR_MAX == 0xffffffff//Now what registers may change there?..

		#elif _WIN64//TODO: mark 0-2 and 8-11 registers as unknown.
			
		#else//TODO: mark 0-3 and 6-11 registers as unknown.

		#endif
		return call(s,f);
	}
}