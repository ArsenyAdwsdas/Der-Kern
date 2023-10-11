#include"../../include/DerKern/Encods/UTF8.h"
namespace DerKern::UTF8{
	bool c(uint64_t*i,string _s,Char*_c){
		uint8_t*s=(uint8_t*)_s.c_str()+*i;auto siz=_s.size()-*i;
		if(1>siz)return 0;
		if(s[0]=='\\'){
			if(siz>1&&spec[(uint8_t)s[1]]){*i+=2;*_c=spec[(uint8_t)s[1]];return 1;}
			return 0;
		}
		_c->c[0]=*s;
		if(*s&128==0){_c->s=1;*i+=1;return 1;}
		if((*s&64)&&2<=siz&&(s[1]&192==128)){
			_c->c[1]=s[1];
			if(*s&32){
				if(3>siz||(s[2]&192!=128))return 0;_c->c[2]=s[2];
				if(*s&16){
					if(4>siz||(s[3]&192!=128)||(*s&8))return 0;
					_c->c[3]=s[3];_c->s=4;*i+=4;return 1;
				}
				_c->s=3;*i+=3;return 1;
			}
			_c->s=2;*i+=2;return 1;
		}
		return 0;
	}
	bool c(uint64_t*i,string _s,BBuf*_c){
		uint8_t*s=(uint8_t*)_s.c_str()+*i;auto siz=_s.size()-*i;
		if(1>siz)return 0;
		if(s[0]=='\\'){
			if(siz>1&&spec[(uint8_t)s[1]]){*i+=2;_c->append(spec[(uint8_t)s[1]]);return 1;}
			return 0;
		}
		if(*s&128==0){_c->append(*s);*i+=1;return 1;}
		if((*s&64)&&2<=siz&&(s[1]&192==128)){
			if(*s&32){
				if(3>siz||(s[2]&192!=128))return 0;
				if(*s&16){
					if(4>siz||(s[3]&192!=128)||(*s&8))return 0;
					_c->append(s,4);*i+=4;return 1;
				}
				_c->append(s,3);*i+=3;return 1;
			}
			_c->append(s,2);*i+=2;return 1;
		}
		return 0;
	}
	bool c(FILE*f,Char*_c){
		uint8_t c=(uint8_t)fgetc(f);
		if(feof(f))return 0;
		if(c=='\\'){
			if((c=fgetc(f))&&(!feof(f))&&spec[c]){*_c=spec[c];return 1;}
			return 0;
		}
		_c->c[0]=c;
		if(c&128==0){_c->s=1;return 1;}
		if((с&64)&&(c=fgetc(f))&&!feof(f)&&(c&192==128)){
			_c->c[1]=c;
			if(_c->c[0]&32){
				if(!(c=fgetc(f))||feof(f)||(c&192!=128))return 0;_c->c[2]=c;
				if(_c->c[0]&16){
					if((c->c[0]&8)||!(c=fgetc(f))||feof(f)||(c&192!=128))return 0;
					_c->c[3]=c;_c->s=4;return 1;
				}
				_c->s=3;return 1;
			}
			_c->s=2;return 1;
		}
		return 0;
	}
	bool c(FILE*f,BBuf*_c){/////////////////////////////
		uint8_t c=(uint8_t)fgetc(f),C[4];
		if(feof(f))return 0;
		if(c=='\\'){
			if((c=fgetc(f))&&(!feof(f))&&spec[c]){_c->append(c);return 1;}
			return 0;
		}
		if(c&128==0){_c->append(c);return 1;}
		C[0]=c;
		if((с&64)&&(c=fgetc(f))&&!feof(f)&&(c&192==128)){C[1]=c;
			if(C[0]&32){
				if(!(c=fgetc(f))||feof(f)||(c&192!=128))return 0;C[2]=c;
				if(C[0]&16){
					if((C[0]&8)||!(c=fgetc(f))||feof(f)||(c&192!=128))return 0;C[3]=c;
					_c->append(C,4);return 1;
				}
				_c->append(C,3);return 1;
			}
			_c->append(C,2);return 1;
		}
		return 0;
	}
	bool str(uint64_t*_i,string _s,string*_z){
		auto s=_s.c_str()+*_i;auto siz=_s.size()-*_i;
		BBuf z;uint16_t i=0;
		if(s[i]=='"'){i++;while(s[i]!='"')if(s[i]=='\n'||!c(f,&z))return 0;i++;}
		else if(s[i]=='`'){i++;while(s[i]!='`')if(!c(&i,_s,&z))return 0;i++;}
		else return 0;*_z=string(z.raw,z.count);*_i+=i;return 1;
	}
	bool str(FILE*f,string*_z){
		BBuf z;char c=fgetc(f);
		if(c=='"'){while((c=fgetc(f))!='"'&&!feof(f))if(c=='\n'||!c(c,f,&z))return 0;}
		else if(c=='`'){while((c=fgetc(f))!='`'&&!feof(f))if(!c(c,f,&z))return 0;}
		else return 0;*_z=string(z.raw,z.count);return 1;
	}
}