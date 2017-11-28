CC   = gcc
CFLG = -g

HDR = mips_asm_header.h mipssim.h
SRC = mipssim.c main.c
OBJ = $(SRC:.c=.o)
EXE = a.out

ASM  = /home/dretz/Share/mips_asm
TDIR = test
TSRC = $(wildcard $(TDIR)/*.s)
TOUT = $(TSRC:.s=.mb)

.PHONY: all
all : $(EXE)

$(EXE) : $(OBJ)
	$(CC) $(CFLG) $^ -o $@

mipssim.o : mipssim.c mipssim.h mips_asm_header.h
	$(CC) $(CFLG) -c $< -o $@

main.o : main.c mipssim.h mips_asm_header.h
	$(CC) $(CFLG) -c $< -o $@

.PHONY: test
test :
	$(foreach file, $(TSRC), $(ASM) $(file) | grep "Source file\|Error count";)

.PHONY : clean
clean :
	rm -f $(EXE) $(OBJ) $(TOUT)
