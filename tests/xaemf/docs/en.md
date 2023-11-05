# What it is
X86-64 Asm Except More Fun
This is my idea to make a more comfy and understandable for me Assembler.

# Syntax basics
"\" does the same as ";" in C
";" and "//" are one-line comments
"/*" is a flexible comment that requires "*/" to mark the end of it
"#" is either a compilation command or a one-line comment, depends on whether there is such a command
[Instructions](#instructions) may have a comma between any 2 arguments and any spaces
# Registers
[Register names](https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf) are recognized as variables(unsigned integers) located at CPU registers
There are also variables recognized as entire(all 4/8 bytes depending on the launch environment) 8 first registers
- vax=eax/rax, vcx=ecx/rcx, vdx=edx/rdx, vbx=ebx/rbx, vsp=esp/rsp, vbp=ebp/rbp, vsi=esi/rsi, vdi=edi/rdi


# Instructions
arguments will be written as `<w>`, for example `<a>` is an argument "a"
## add
arguments: `<a> <b>`
requirements: `<a>` is an editable variable
action: adds `<b> to <a>`
"C" version: `<a>+=<b>`
## mov
arguments: `<a> <b>`
requirements: `<a>` is an editable variable
action: rewrites `<a>'s value to <b>`
"C" version: `<a>=<b>`
## ret
arguments: NONE
requirements: vsp!=0; points to a memory that can be executed, "call" takes care of that as long as you don't fool around with vsp
action: pops a pointer from vsp and jumps there
"C" version: return
## xchg
arguments: `<a> <b>`
requirements: `<a> and <b>` are editable variables
action: swaps values of `<a> and <b>`
"C" version: `_=<a>;<a>=<b>;<b>=_`