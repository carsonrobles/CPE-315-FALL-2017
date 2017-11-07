CC  = gcc
HDR = mips_asm_header.h mipssim.h
SRC = main.c mipssim.c
OBJ = $(SRC:.c=.o)
EXE = a.out

ASM  = /home/dretz/Share/mips_asm
TSRC = $(wildcard test/*.s)
TOUT = $(TSRC:.s=.mb)

$(EXE): $(HDR) $(SRC)
	$(CC) -c $(SRC)
	$(CC) $(OBJ) -o $(EXE)

.PHONY: test
test: $(TST)
	$(foreach file, $(TSRC), $(ASM) $(file) | grep "Source file\|Error count";)


.PHONY: clean
clean:
	rm -f $(OBJ) $(EXE) $(TOUT)
