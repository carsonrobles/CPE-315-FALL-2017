CC  = gcc
HDR = mips_asm_header.h mipssim.h
SRC = main.c mipssim.c
OBJ = $(SRC:.c=.o)
EXE = a.out

ASM  = /home/dretz/Share/mips_asm
TSRC = $(wildcard test/*.s)
TOUT = $(TSRC:.s=.mb)

.PHONY: all
all : $(EXE)

$(EXE) : $(OBJ)
	$(CC) $^ -o $@

mipssim.o : mipssim.c mipssim.h mips_asm_header.h
	$(CC) -c $< -o $@

main.o : main.c mipssim.h mips_asm_header.h
	$(CC) -c $< -o $@

.PHONY: test
test :
	$(foreach file, $(TSRC), $(ASM) $(file) | grep "Source file\|Error count";)

.PHONY : clean
clean :
	rm -f $(EXE) $(OBJ) $(TOUT)
