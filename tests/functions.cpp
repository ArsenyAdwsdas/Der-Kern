#include"../src/Functions.cpp"
#include"../src/Values.cpp"
#include"../src/Instruction.cpp"
using namespace DerKern;

bool DerKern::Parse1(uint64_t*i,string S,ParseResult*r){throw std::exception();}
bool TypeCanBe(const Type*expected,const Type*got){
	#warning I am NOT morally ready for all this "const" stuff... It's nightmare fuel. Too much checking for proper support. This'll do it for now.
	if(expected->typeID==Type::IDs::Const){return expected==got||TypeCanBe(((const ConstType*)expected)->orig,got);}
	return expected==got;
}


Function*GetFunction(string name,Type**argv,uint8_t argc,const Scope*scop=0){
	FuncArgs args={argv,argc};
	for(uint8_t i=0;i<argc;i++){
		auto z=argv[i]->Static(name);
		if(z&&TypeCanBe(Type::overloadery.Const(),z->type)){
			OverloadedFunction*f=0;
			if(z->val.type==1)f=(OverloadedFunction*)z->val.imov;
			if(z->val.type==7)f=(OverloadedFunction*)z->val.ptr;
			if(f){auto _=f->tryGet(args);if(_)return _;}
		}
	}
	if(!scop)return 0;
	auto z=scop->get(name);
	if(z&&TypeCanBe(Type::overloadery.Const(),z->type)){
		OverloadedFunction*f=0;
		if(z->val.type==1)f=(OverloadedFunction*)z->val.imov;
		if(z->val.type==4)f=(OverloadedFunction*)z->val.ptr;
		if(f){auto _=f->tryGet(args);if(_)return _;}
	}
	return 0;
}
void eval(Function&f, EvalState&e, Location ret, const Location*argv, uint8_t argc){
	if(f.type==0){
		bool nul[256]={0};
		if(f.retsLoc.resolve()==ret.resolve());
		else if(f.retsLoc.type!=0){f.retsLoc=ret;nul[255]=1;}
		else{

		}
		for(uint8_t i=0;i<f.argc;i++){
			if(f.argvLoc[i].resolve()==argv[i].resolve());
			else if(f.argvLoc[i].type!=0){f.argvLoc[i]=argv[i];nul[i]=1;}
			else{

			}
		}
		f.inl->eval(e);
		if(nul[255])f.retsLoc.type=0;
		for(uint8_t i=0;i<argc;i++)if(nul[i])f.argvLoc[i].type=0;
	}
}
inline void eval(Function&f,Environment*E,Location ret,const Location*argv,uint8_t argc){EvalState e(E);return eval(f,e,ret,argv,argc);}
template<uint8_t argc>inline void eval(Function&f,EvalState&e,Location ret,const Location argv[argc]){eval(f,e,ret,argv,argc);}
template<uint8_t argc>inline void eval(Function&f,Environment*e,Location ret,const Location argv[argc]){eval(f,e,ret,argv,argc);}
int main(){
	Scope scp;
	Type*_[2]={&Type::u64,&Type::u64};
	OverloadedFunction*overy;
	scp.set("+",{&Type::overloadery,overy=new OverloadedFunction()});
	auto op=overy->typery.set(new Function(&Type::u64,_,2,0));
	Function*z;{z=GetFunction((string)"+",_,2,&scp);}if(!z)printf("GetFunction FAIL\n");
	op->inl=new Instructions::Add(&Type::u64,&op->rets,&op->argvLoc[0],&op->argvLoc[1]);


	Environment env=Environment(0);
	//printf("?..\n");if(1)return 0;
	uint64_t a[]={1,2006};
	Location ret=a;
	const Location args[2]={&a[0],&a[1]};
	printf("?...\n");
	eval(*z,&env,ret,args,2);
	printf("!?...\n");
	return 0;
}