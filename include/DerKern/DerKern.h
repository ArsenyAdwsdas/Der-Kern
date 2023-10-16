#pragma once
#define FASTER //makes it faster but also uses more RAM
#define HASTE //"value" will expect that some characters can only be used as Unary Operators (-+=$#!~) and not parts for _value
#define NO_SANITY //disables "Sanity Error"(for those who want "<Type>****" or worse)

#include<assert.h>
#include<stdlib.h>
#include<string.h>

#include"parse.h"
#include"compile.h"
