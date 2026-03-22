TARGET=pru_squarewave
OUT=$(TARGET).out
OBJ=$(TARGET).obj

PRU_CGT=/usr/share/ti/cgt-pru
PRU_SUPPORT=/usr/lib/ti/pru-software-support-package

CC=$(PRU_CGT)/bin/clpru
LD=$(PRU_CGT)/bin/clpru

CFLAGS=--include_path=$(PRU_CGT)/include \
       --hardware_mac=on \
       --endian=little \
       -v3 \
       --display_error_number \
       --diag_wrap=off \
       -O2

LDFLAGS=--run_linker \
        --warn_sections \
        --stack_size=0x100 \
        --heap_size=0x100 \
        --library=$(PRU_CGT)/lib/rtspru.lib \
        -i$(PRU_CGT)/lib

all: $(OUT)

$(OUT): $(OBJ)
	$(LD) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(OBJ): $(TARGET).c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.obj *.out *.map *.asm