# MAKEFILE for CMDboot

all : cmdboot.exe

cmdboot.exe : cmdboot.c cmdboot.def
	gcc cmdboot.c cmdboot.def -Zomf -Zmtd -s -O2 -o cmdboot.exe

# end of cmdboot.mak #

