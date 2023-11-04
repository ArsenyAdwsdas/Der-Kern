#pragma once
#include"../Instruction.cpp"
#include"../../include/DerKern/platformie/x86.h"
#include"../compile.cpp"
bool DerKern::Instruction::compile(CompileState&s){if(s.inlining){s.backpawchy+={s.b->claim(5),{}};s.b->raw[s.b->count-5]=0xe9;}else(*s.b)+=0xC3;return 1;}

namespace DerKern::Instructions{
	#ifdef VisualStudioCode_SHUT_UP//BECAUSE IT APPARENTLY THINKS THAT std::FILE* CAN'T BE CONVERTED TO FILE*
		#undef stderr
		#define stderr (FILE*)0
	#endif
	bool Return1::compile(CompileState&s){bef->compile(s);(*s.b)+=0xC3;return 1;}
	bool Pong::compile(CompileState&s){fprintf(stderr,"!!!pong is NOT complete!!!\n");throw std::exception();return 0;}
	bool jmpLine::compile(CompileState&s){fprintf(stderr,"!!!jmpLine is NOT complete!!!\n");throw std::exception();return 0;}
	namespace x86_64{//https://defuse.ca/online-x86-assembler.htm
		inline bool op1rm(uint8_t op,BBuf*b,uint8_t to,Location f,uint8_t s){
			if(f.type==0||Register::count<=to||((f.type==2||f.type==3)&&Register::count<=f.reg)||f.type>3)return 0;
			uint8_t z[12],off=0;
			if(s==2){z[0]=0x66;off++;}
			if(to>7||(f.type!=1&&f.reg>7)||s==8){z[off++]=0x40|(f.type==1?to>7?4:0:((to>7?4:0)|(f.reg>7)))|(s&8);}
			z[off++]=op|(s>1);
			if(f.type==3){
				if(f.imov!=0||(f.reg&7)==5){
					if(~0xff&(uint32_t)f.imov){
						z[off++]=(f.reg&7)|((to&7)<<3)|0x80;
						if((f.reg&7)==4)z[off++]=0x24;
						memcpy(z+off,&f.imov,4);off+=4;
					}else{
						z[off++]=(f.reg&7)|((to&7)<<3)|0x40;
						if((f.reg&7)==4)z[off++]=0x24;
						z[off++]=(uint8_t)f.imov;
					}
				}else{
					z[off++]=(f.reg&7)|((to&7)<<3);
					if((f.reg&7)==4)z[off++]=0x24;
				}
			}else if(f.type==1){
				z[off++]=4|((to)<<3);z[off++]=0x25;
				memcpy(z+off,&f.imov,4);off+=4;
			}else{z[off++]=0xc0|(f.reg&7)|((to&7)<<3);}
			b->append(z,off);
			return off;
		}
		inline bool op1rm32(uint8_t op1,uint8_t op2,uint8_t op3,uint8_t op4,BBuf*b,uint8_t to,Location f,uint8_t s){//adcx,adox
			if(f.type==0||Register::count<=to||((f.type==2||f.type==3)&&Register::count<=f.reg)||f.type>3||(s!=4&&(sizeof(int*)==4||s!=8)))return 0;
			uint8_t z[12],off=1;
			z[0]=op1;
			if(s==8||(f.type!=1&&f.reg>7)||to>7){
				z[1]=0x40|(f.type==1?(to>7)*4:((to>7?4:0)|(f.reg>7)))|(s&8);off++;
			}
			z[off++]=op2;z[off++]=op3;z[off++]=op4;
			if(f.type==3){
				if(f.imov>0||(f.reg&7)==5){
					if(~0xff&(uint32_t)f.imov){
						z[off++]=(f.reg&7)|((to&7)<<3)|0x80;
						if((f.reg&7)==4)z[off++]=0x24;
						memcpy(z+off,&f.imov,4);off+=4;
					}else{
						z[off++]=(f.reg&7)|((to&7)<<3)|0x40;
						if((f.reg&7)==4)z[off++]=0x24;
						z[off++]=(uint8_t)f.imov;
					}
				}else{
					z[off++]=(f.reg&7)|((to&7)<<3);
					if((f.reg&7)==4)z[off++]=0x24;
				}
				return 1;
			}else if(f.type==1){
				z[off++]=4|((to&7)<<3);z[off++]=0x25;
				memcpy(z+off,&f.imov,4);off+=4;
			}
			b->append(z,off);
			return off;
		}
		inline bool op1rvm(uint8_t op,BBuf*b,uint8_t to,uint8_t f1,Location f2,uint8_t siz){//andn,bextr
			if(Register::count<=to||Register::count<=f1||f2.type==0||((f2.type==2||f2.type==3)&&Register::count<=f2.reg)||f2.type>3||(siz!=4&&(sizeof(int*)==4||siz!=8)))return 0;
			uint8_t z[10],off=4;
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
							return 10;
						}else{memcpy(z+5,&f2.imov,4);b->append(z,9);return 9;}
					}else{
						z[4]=(to&7)|((f2.reg&7)<<3)|0x40;
						if((f2.reg&7)==4){
							z[5]=0x24;z[6]=(uint8_t)f2.imov;
							b->append(z,7);
							return 7;
						}else{z[5]=(uint8_t)f2.imov;b->append(z,6);return 6;}
					}
				}else{
					if((f2.reg&7)==4){
						z[4]=(to&7)|((f2.reg&7)<<3);
						z[5]=0x24;
						b->append(z,6);
						return 6;
					}else{
						z[4]=(to&7)|((f2.reg&7)<<3);
						b->append(z,5);
						return 5;
					}
				}
			}else if(f2.type==1){
				z[4]=4|((to&7)<<3);z[5]=0x25;
				memcpy(z+6,&f2.imov,4);
				b->append(z,10);
				return 10;
			}
			z[4]=0xc0|(f2.reg&7)|((to&7)<<3);
			b->append(z,5);
			return 5;
		}
		inline bool op1mi(uint8_t op,uint8_t op2,BBuf*b,Location to,uint32_t v,uint8_t s){
			if(!validL(to)||!validS(s))return 0;
			uint8_t s2=(v&~0x7f)==0?1:s==2?2:4,off=0;
			uint8_t z[16];
			if(s==2){z[0]=0x66;off++;}
			if(to.reg>7||s==8){z[off++]=0x40|(to.reg>7)|(s&8);}
			z[off++]=op|(s>1?(s2==1)*2+1:0);
			if(to.type==3){
				if(to.imov>0||(to.reg&7)==3){
					if(~0xff&(uint32_t)to.imov){
						z[off++]=to.reg|op2|0x80;
						memcpy(z+off,(uint8_t*)&to.imov,4);off+=4;
					}else{
						z[off++]=to.reg|op2|0x40;
						z[off++]=(uint8_t)to.imov;
					}
				}else{z[off++]=to.reg|op2;if(to.reg==4)z[off++]=0x24;}
			}else if(to.type==1){
				z[off++]=4|op2;z[off++]=0x25;
				memcpy(z+off,(uint8_t*)&to.imov,4);off+=4;
			}else z[off++]=0xc0|to.reg|op2;
			memcpy(z+off,(uint8_t*)&v,s2);off+=s2;
			b->append(z,off);
			return off;
		}
		inline uint16_t op1i(uint8_t op,BBuf*b,uint32_t v,uint8_t s){//to=eax
			if(!validS(s))return 0;
			uint8_t z[8],off=0;
			if(s==2){z[0]=0x66;off++;}
			else if(s==8){z[0]=0x48;off++;}
			z[off++]=op|(s>1);
			s=(v&~0x7f)==0?1:s==2?2:4;
			memcpy(z+off,(uint8_t*)&v,s);off+=s;
			b->append(z,off);
			return 1+off;
		}
		inline uint16_t op1vm(uint8_t op2,BBuf*b,uint8_t to,Location f,uint8_t siz){//blsi
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
							return 10;
						}else{memcpy(z+5,&f.imov,4);b->append(z,9);return 9;}
					}else{
						z[4]=op2|((f.reg&7)<<3)|0x40;
						if((f.reg&7)==4){
							z[5]=0x24;z[6]=(uint8_t)f.imov;
							b->append(z,7);
						}else{z[5]=(uint8_t)f.imov;b->append(z,6);return 6;}
					}
				}else{
					if((f.reg&7)==4){
						z[4]=op2|((f.reg&7)<<3);
						z[5]=0x24;
						b->append(z,6);
						return 6;
					}else{
						z[4]=op2|((f.reg&7)<<3);
						b->append(z,5);
						return 5;
					}
				}
			}else if(f.type==1){
				z[4]=4|op2;z[5]=0x25;
				memcpy(z+6,&f.imov,4);
				b->append(z,10);
				return 10;
			}
			z[4]=0xc0|op2|(f.reg&7);
			b->append(z,5);
			return 5;
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
		uint16_t add(CompileState&c,Location to,Location f,uint8_t s){
			typedef RegisterState::regT regT;
			constexpr uint8_t opc=0;
			if(to.type==2&&f.type!=0){
				/*if(c.regs%to.reg&&c.regs%f.reg){
					regT v=c.regs[f.reg],z=c.regs[to.reg],r=z+v;
					c.regs[to.reg]=r;

					//TODO: update known AF, PF... I dislike them.
					c.flgs(Flags::CF,z>regT(v)||r>regT(v));
					c.flgs(Flags::OF,(z&((regT(-1)>>1)+1))^(r&((regT(-1)>>1)+1)));
					c.flgs(Flags::SF,r&((regT(-1)>>1)+1));
					c.flgs(Flags::ZF,r==0);

				}else{c.regs%to.reg=0;for(uint8_t i=0;i<Flags::count;i++)c.flgs.known(i,0);}*/
				return op1rm(opc|2,c.b,to.reg,f,s);
			}
			//c.regs%to.reg=0;for(uint8_t i=0;i<Flags::count;i++)c.flgs.known(i,0);
			if((to.type&1)&&f.type==2){

				return op1rm(opc,c.b,f.reg,to,s);
			}
		}
		uint16_t addI(CompileState&c,Location to,int32_t v,uint8_t s){
			typedef RegisterState::regT regT;
			constexpr uint8_t opc=0;
			/*if(to.type==2&&c.regs%to.reg){
				regT z=c.regs[to.reg],r=z+v;
				c.regs[to.reg]=r;

				//TODO: update known AF, PF... I dislike them.
				c.flgs(Flags::CF,z>regT(v)||r>regT(v));
				c.flgs(Flags::OF,(z&((regT(-1)>>1)+1))^(r&((regT(-1)>>1)+1)));
				c.flgs(Flags::SF,r&((regT(-1)>>1)+1));
				c.flgs(Flags::ZF,r==0);
			}else{
				c.regs%to.reg=0;
				for(uint8_t i=0;i<Flags::count;i++)c.flgs.known(i,0);
			}*/
			return to.type!=0&&((to.type==2&&to.reg==0&&(s==1||(v&~0xff)!=0))?op1i(4|opc,c.b,v,s):op1mi(0x80,opc,c.b,to,v,s));
		}

		uint16_t call(CompileState&c,Location l){
			if(l.type==0)return 0;
			if(l.type==1){c.b->append(0xe8);c.b->append((uint8_t*)&l.imov,4);return 5;}//Here's the thing... It's relative...
			if(l.type==2){
				uint8_t z[sizeof(int*)]={0x41,0xff,uint8_t(0xd0|(l.reg&7))};//I CAST MANUALLY BECAUSE APPARENTLY & RETURNS int
				if(l.reg>7){c.b->append(z,3);return 3;}
				c.b->append(z+1,2);return 2;
			}else if(l.type==3){//I feel like I'll have a hard time with it...
				uint8_t z[sizeof(int*)]={0x41,0xff,uint8_t(0x10|(l.reg&7)|((uint32_t(l.imov)<0x80)?0x40:0x80))};
				uint8_t i=0;
				if(l.reg==4)z[i++]=0x24;
				memcpy(z+3+i,&l.imov,(z[2]&0x80)?4:1);i=((z[2]&0x80)?6:3)+(l.reg>7)+(l.reg==4);
				c.b->append(z+(l.reg<8),i);
				return i;
			}
		}

		uint16_t mov(CompileState&c,Location to,Location f,uint8_t s){
			if(to==f&&!(s==4&&(sizeof(int*)==4||to.type==2)))return 1;
			constexpr uint8_t opc=0x88;
			if(to.type==2&&f.type!=0){
				//if(f.type==2&&c.regs%f.reg){c.regs%to.reg=1;c.regs[to.reg]=c.regs[f.reg];}else c.regs%to.reg=0;
				return op1rm(opc|2,c.b,to.reg,f,s);
			}
			if((to.type&1)&&f.type==2)return op1rm(opc,c.b,f.reg,to,s);
		}
		uint16_t movI(CompileState&c,Location to,uint64_t v,uint8_t s){
			typedef RegisterState::regT regT;
			uint8_t z[99],off=0;
			if(s==2){z[0]=0x66;off++;}
			if(to.reg>7||s==8){z[off++]=0x40|(to.reg>7)|(s&8);}
			if(to.type==2){
				z[off++]=0xb0|(to.reg&7)|(s>1?8:0);
				memcpy(z+off,&v,s);off+=s;
				c.b->append(z,off);
				/*if(s==1){c.regs.known[to.reg]=1;c.regs%to.reg=(uint8_t)v;}
				if(s==2){c.regs.known[to.reg]=1;c.regs%to.reg=(uint16_t)v;}
				if(s==4){c.regs.known[to.reg]=1;c.regs%to.reg=(uint32_t)v;}
				if(s==8){c.regs.known[to.reg]=1;c.regs%to.reg=v;}*/
				return off;
			}
		}

		uint16_t xchg(CompileState&c,Location to,Location f,uint8_t s){
			if(to==f&&!(s==4&&(sizeof(int*)==4||to.type==2)))return 1;
			constexpr uint8_t opc=0x86;
			if(to.type==2&&f.type!=0){
				/*if(f.type==2){
					bool _=c.regs%f.reg;
					c.regs%f.reg=c.regs%to.reg;
					c.regs%to.reg=_;
				}else (c.regs%to.reg)=(c.regs%f.reg)=0;*/
				return op1rm(opc|2,c.b,to.reg,f,s);
			}
			if((to.type&1)&&f.type==2){
				//c.regs%f.reg=0;
				return op1rm(opc,c.b,f.reg,to,s);
			}
		}

		uint16_t pop(CompileState&c,Location v,uint8_t siz){
			if(v.type==2){
				//c.regs%v.reg=0;
				if(siz==sizeof(int*)){
					if(v.reg&8){uint8_t z[sizeof(int*)]={0x41,uint8_t(0x58|(v.reg&7))};c.b->append(z,2);return 2;}
					c.b->append(0x58+(v.reg&7));return 1;
				}
			}
		}
		uint16_t push(CompileState&c,Location v,uint8_t siz){
			if(v.type==2){
				//c.regs%v.reg=0;
				if(siz==sizeof(int*)){
					if(v.reg&8){uint8_t z[sizeof(int*)]={0x41,uint8_t(0x50|(v.reg&7))};c.b->append(z,2);return 2;}
					c.b->append(0x50+(v.reg&7));return 1;
				}
			}
		}
	}
	/*bool Add::compile(CompileState&c){
		using namespace x86_64;
		Location Out=out.resolve(),In=in.resolve(),In2=in2.resolve();
		uint16_t z=0,_z;
		if((Out.type&2)||((In.type&2)&&(In2.type&2))){
			if(_z=mov(c,Out,In,siz))z+=_z;else return 0;
			if(_z=add(c,Out,In2,siz))z+=_z;else return 0;
			return z;
		}else{
			uint8_t _;
			if(c.UnusedRegister(&_)){
				if(_z=mov(c,_,In,siz))z+=_z;else return 0;
				if(_z=add(c,_,In2,siz))z+=_z;else return 0;
				if(_z=mov(c,Out,_,siz))z+=_z;else return 0;
				return z;
			}
			for(uint8_t i=0;i<3;i++)if(((In.type&2)&&In.reg!=i)&&((In2.type&2)&&In2.reg!=i)){
				if(_z=xchg(c,_,Out,siz))z+=_z;else return 0;
				if(_z=mov(c,_,In,siz))z+=_z;else return 0;
				if(_z=add(c,_,In2,siz))z+=_z;else return 0;
				if(_z=xchg(c,_,Out,siz))z+=_z;else return 0;
				return z;
			}
		}
	}*/
	bool Set8::compile(CompileState&c){return x86_64::movI(c,to,val,1);}
	bool Set16::compile(CompileState&c){return x86_64::movI(c,to,val,2);}
	bool Set32::compile(CompileState&c){return x86_64::movI(c,to,val,4);}
	bool Set64::compile(CompileState&c){return x86_64::movI(c,to,val,8);}
	bool Set::compile(CompileState&c){return x86_64::mov(c,to,from,size);}
	bool Swap::compile(CompileState&c){return x86_64::xchg(c,to,from,size);}

	bool Add8_1::compile(CompileState&c){return x86_64::addI(c,to,val,1);}
	bool Add16_1::compile(CompileState&c){return x86_64::addI(c,to,val,2);}
	bool Add32_1::compile(CompileState&c){return x86_64::addI(c,to,val,4);}
	bool Add_1::compile(CompileState&c){return x86_64::add(c,to,from,size);}
	bool TransitionTo::compile(CompileState&c){
		using namespace x86_64;
		#if _WIN64//I simply need a way to get the first argument to ebp. Thanks https://en.wikipedia.org/wiki/X86_calling_conventions
			if(sizeof(int*)==8)xchg(c,(uint8_t)5,(uint8_t)0,sizeof(int*));// HELLO "Microsoft x64 calling convention", YOU BEING ENFORCED ONTO ME HERE SEEMS PROBLEMATIC.
		#else 
			if(sizeof(int*)==8)xchg(c,(uint8_t)5,(uint8_t)7,sizeof(int*));// HELLO "System V AMD64 ABI", YOU TOO ARE BEING ENFORCED... WHY ARE YOU USING rdi AND ONLY THEN rsi BY THE WAY?
		#endif
		else xchg(c,(uint8_t)5,Location(4,sizeof(int*)),sizeof(int*));

		//Sure I do save some registers I don't have to save if I listen to conventions... meh. Don't wanna deal with it.
		for(uint8_t i=0;i<5;i++)xchg(c,(uint8_t)i,Location(5,i*sizeof(int*)),sizeof(int*));
		for(uint8_t i=6;i<(sizeof(int*)<<1);i++)xchg(c,(uint8_t)i,Location(5,i*sizeof(int*)),sizeof(int*));
		return 1;
	}
	bool TransitionFrom::compile(CompileState&c){
		using namespace x86_64;
		//Sure I do save some registers I don't have to save if I listen to conventions... meh. Don't wanna deal with it.
		for(uint8_t i=0;i<5;i++)xchg(c,(uint8_t)i,Location(5,i*sizeof(int*)),sizeof(int*));
		for(uint8_t i=6;i<(sizeof(int*)<<1);i++)xchg(c,(uint8_t)i,Location(5,i*sizeof(int*)),sizeof(int*));

		#if _WIN64//I simply need a way to restore ebp. Thanks https://en.wikipedia.org/wiki/X86_calling_conventions
			if(sizeof(int*)==8)mov(c,(uint8_t)5,(uint8_t)0,sizeof(int*));// HELLO "Microsoft x64 calling convention", YOU BEING ENFORCED ONTO ME HERE SEEMS PROBLEMATIC.
		#else 
			if(sizeof(int*)==8)mov(c,(uint8_t)5,(uint8_t)7,sizeof(int*));// HELLO "System V AMD64 ABI", YOU TOO ARE BEING ENFORCED... WHY ARE YOU USING rdi AND ONLY THEN rsi BY THE WAY?
		#endif
		else mov(c,(uint8_t)5,Location(4,sizeof(int*)),sizeof(int*));
		return 1;
	}
	bool _set(CompileState&c,Location l,RegisterState::regT v){
		if(l.type==2){

		}
	}
	bool _call(CompileState&s,Location l){
		using namespace x86_64;
		if(l.type==1){

		}else if(l.type>3){
			uint8_t r;
			if(s.UnusedRegister(&r)){
				movI(s,r,l.imov,sizeof(int*));
				return x86_64::call(s,r);
			}
			//OK HOW THE F--- DO I DO IT IN THIS CASE?
		}else return x86_64::call(s,l);
	}
	bool call::compile(CompileState&c){return _call(c,loc);}
}