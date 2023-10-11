#include"../../include/DerKern/platformie/x86.h"
namespace DerKern::Instructions{
	void Return0::compile(CompileState&s){s.b+=0xC3;}
	void Return1::compile(CompileState&s){bef->compile(s);(*s.b)+=0xC3;}
	void Pong::compile(CompileState&s){fprintf(stderr,"!!!pong is NOT complete!!!\n");throw std::exception();}
	void jmpLine::compile(CompileState&s){fprintf(stderr,"!!!jmpLine is NOT complete!!!\n");throw std::exception();}
	namespace x86_64{
		inline bool op1rm(uint8_t op,BBuf*b,uint8_t to,Location f,uint8_t s){//https://defuse.ca/online-x86-assembler.htm
			if(f.type!=0||Registers.count<=to||Registers.count<=f.reg)return 0;
			uint8_t z[10];
			if(s==2)b->append(0x66);
			if(to>7||(f.type!=1&&f.reg>7)||s==8)b->append(0x40|(f.type==1?(to>7)*4:(to>7)|((f.reg>7)*4))|(s&8));
			z[0]=opcode|(s>1);
			if(f.type==3){
				if(f.imov!=0||f.reg&7==5){
					if(~0xff&(uint32_t)f.imov){
						z[1]=(to&7)|((f.reg&7)<<3)|0x80;
						if(f.reg&7==4){
							z[2]=0x24;memcpy(z+3,&f.imov,4);
							b->append(z,7);
						}else{memcpy(z+2,&f.imov,4);b->append(z,6);}
					}else{
						z[1]=(to&7)|((f.reg&7)<<3)|0x40;
						if(f.reg&7==4){
							z[2]=0x24;z[3]=(uint8_t)f.imov;
							b->append(z,4);
						}else{z[2]=(uint8_t)f.imov;b->append(z,3);}
					}
				}else{
					z[1]=(to&7)|((f.reg&7)<<3);
					if(f.reg&7==4){
						z[2]=0x24
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
			z[1]=0xc0|(to&7)|((f.reg&7)<<3);
			b->append(z,2);
			return 1;
		}
		inline bool op1rm32(uint8_t op1,uint8_t op2,uint8_t op3,uint8_t op4,BBuf*b,uint8_t to,Location f,uint8_t s){//adcx,adox
			if(f.type!=0||Registers.count<=to||Registers.count<=f.reg||(s!=4&&(sizeof(int*)==4||s!=8)))return 0;
			uint8_t z[10];
			z[0]=op1;
			if(s==8||(f.type!=1&&f.reg>7)||to>7){
				z[1]=0x40|(f.type==1?(to>7)*4:(to>7)|((f.reg>7)*4))|(s&8);
				z[2]=op2;z[3]=op3;z[4]=op4;b->append(z,5);
			}else{
				z[1]=op2;z[2]=op3;z[3]=op4;b->append(z,4);
			}
			if(f.type==3){
				if(f.imov>0||f.reg&7==5){
					if(~0xff&(uint32_t)f.imov){
						z[0]=(to&7)|((f.reg&7)<<3)|0x80;
						if(f.reg&7==4){
							z[1]=0x24;memcpy(z+2,&f.imov,4);
							b->append(z,6);
						}else{memcpy(z+1,&f.imov,4);b->append(z,5);}
					}else{
						z[0]=(to&7)|((f.reg&7)<<3)|0x40;
						if(f.reg&7==4){
							z[1]=0x24;z[2]=(uint8_t)f.imov;
							b->append(z,3);
						}else{z[1]=(uint8_t)f.imov;b->append(z,2);}
					}
				}else{
					if(f.reg&7==4){
						z[0]=(to&7)|((f.reg&7)<<3);
						z[1]=0x24
						b->append(z,2);
					}else b->append((to&7)|((f.reg&7)<<3));
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
			if(Registers.count<=to||Registers.count<=f1||f2.type!=0||Registers.count<=f2.reg||(s!=4&&(sizeof(int*)==4||s!=8)))return 0;
			uint8_t z[10];
			z[0]=0xc4;
			z[1]=0xe2-((to&8)<<4)-(((f2.type&2)&&(f2.reg&8))*0x20);
			z[2]=(0x78-(f1<<3))|((siz&8)<<4);
			z[3]=op;
			if(f2.type==3){
				if(f2.imov>0||f2.reg&7==5){
					if(~0xff&(uint32_t)f2.imov){
						z[4]=(to&7)|((f2.reg&7)<<3)|0x80;
						if(f2.reg&7==4){
							z[5]=0x24;memcpy(z+6,&f2.imov,4);
							b->append(z,10);
						}else{memcpy(z+5,&f2.imov,4);b->append(z,9);}
					}else{
						z[4]=(to&7)|((f2.reg&7)<<3)|0x40;
						if(f2.reg&7==4){
							z[5]=0x24;z[6]=(uint8_t)f2.imov;
							b->append(z,7);
						}else{z[5]=(uint8_t)f2.imov;b->append(z,6);}
					}
				}else{
					if(f2.reg&7==4){
						z[4]=(to&7)|((f2.reg&7)<<3);
						z[5]=0x24
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
				if(to.imov>0||to.reg&7==5){
					if(~0xff&(uint32_t)f2.imov){
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
			if(!validR(to)||!validL(f2)||!validS(siz))return 0;
			uint8_t z[10];
			z[0]=0xc4;
			z[1]=0xe2-(((f.type&2)&&(f.reg&8))*0x20);
			z[2]=(0x78-(to<<3))|((siz&8)<<4);
			z[3]=0xf3;
			if(f.type==3){
				if(f2.imov>0||f.reg&7==5){
					if(~0xff&(uint32_t)f.imov){
						z[4]=op2|((f.reg&7)<<3)|0x80;
						if(f.reg&7==4){
							z[5]=0x24;memcpy(z+6,&f.imov,4);
							b->append(z,10);
						}else{memcpy(z+5,&f.imov,4);b->append(z,9);}
					}else{
						z[4]=op2|((f.reg&7)<<3)|0x40;
						if(f.reg&7==4){
							z[5]=0x24;z[6]=(uint8_t)f.imov;
							b->append(z,7);
						}else{z[5]=(uint8_t)f.imov;b->append(z,6);}
					}
				}else{
					if(f2.reg&7==4){
						z[4]=op2|((f.reg&7)<<3);
						z[5]=0x24
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
		bool adc(BBuf*b,Location to,Location f,uint8_t s){
			constexpr uint8_t opc=16;
			if(to.type==2&&f.type!=0)return op1rm(opc|2,b,to,f,s);
			if((to.type&1)&&f.type==2)return op1rm(opc,b,f,to,s);
		}bool adc(BBuf*b,Location to,uint32_t v,uint8_t s){constexpr uint8_t opc=16;return to.type!=0&&((to.type==2&&to.reg==0&&(s==1||(v&~0xff)!=0))?op1i(4|opc,b,v,s):op1mi(0x80,opc,b,to,v,s));}
		bool adcx(BBuf*b,uint8_t to,Location f,uint8_t s){return op1rm32(0x66,15,0x38,0xf6,b,to,f,s);}
		bool add(BBuf*,Location to,Location f,uint8_t s){
			constexpr uint8_t opc=0;
			if(to.type==2&&f.type!=0)return op1rm(opc|2,b,to,f,s);
			if((to.type&1)&&f.type==2)return op1rm(opc,b,f,to,s);
		}bool add(BBuf*b,Location to,uint32_t v,uint8_t s){constexpr uint8_t opc=0;return to.type!=0&&((to.type==2&&to.reg==0&&(s==1||(v&~0xff)!=0))?op1i(4|opc,b,v,s):op1mi(0x80,opc,b,to,v,s));}

		bool adox(BBuf*b,uint8_t to,Location f,uint8_t s){return op1rm32(0xf3,15,0x38,0xf6,b,to,f,s);}

		bool and(BBuf*,Location to,Location f,uint8_t s){
			constexpr uint8_t opc=32;
			if(to.type==2&&f.type!=0)return op1rm(opc|2,b,to,f,s);
			if((to.type&1)&&f.type==2)return op1rm(opc,b,f,to,s);
		}bool and(BBuf*b,Location to,uint32_t v,uint8_t s){constexpr uint8_t opc=32;return to.type!=0&&((to.type==2&&to.reg==0&&(s==1||(v&~0xff)!=0))?op1i(4|opc,b,v,s):op1mi(0x80,opc,b,to,v,s));}
		bool andn(BBuf*b,uint8_t to,uint8_t f1,Location f2,uint8_t s){return op1rvm(0xf2,b,to,f1,f2,s);}
		bool bextr(BBuf*b,uint8_t to,Location f2,uint8_t f1,uint8_t s){return op1rvm(0xf7,b,to,f1,f2,s);}
		bool blsi(BBuf*b,uint8_t to,Location f,uint8_t s){return op1vm(24,b,to,f,s);}
	}
}