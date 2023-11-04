#include<stdio.h>
#include<assert.h>
#include"../../include/DerKern/MyString.h"
using namespace DerKern;
typedef MyString string;
int main(int argc, char const *argv[]){//is basically built into ascii.cpp and type.cpp
	string a="1";printf("%c\n",*a.data());
	string b="2";printf("%c\n",*b.data());
	a=b;printf("%c\n",*a.data());
	a=a;printf("%c\n",*a.data());
	string _;_=a;printf("%c\n",*_.data());
	_="3";printf("%c\n",*_.data());
	{string c="";c=_;printf("%c\n",*c.data());}printf("%c\n",*_.data());
	printf("Done\n");
}