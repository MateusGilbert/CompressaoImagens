CC=g++
LD=g++
CFLAGS= -c -Wall -O3
LFLAGS= -o

.c.o:
	$(CC) $(CFLAGS) $<

.cpp.o:
	$(CC) $(CFLAGS) $<

MOD_OBJS = ppm_model.o #adaptive_model.o
E_OBJS = encode.o bit_output.o ppm_encode.o arithmetic_encode.o $(MOD_OBJS)
D_OBJS = decode.o bit_input.o arithmetic_decode.o $(MOD_OBJS)

EXECS = encode decode

all: $(EXECS)

encode: $(E_OBJS)
	$(LD) $(LFLAGS) $@ $(E_OBJS)

decode: $(D_OBJS)
	$(LD) $(LFLAGS) $@ $(D_OBJS)

clean:
	rm -f *.o $(EXECS)
