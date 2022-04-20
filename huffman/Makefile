CC=g++
LD=g++
CFLAGS= -c -Wall -O3
LFLAGS= -o

.cpp.o:
	$(CC) $(CFLAGS) $<

EXECS = huff_enc huff_dec

all: $(EXECS)

OBJS = get_prob.o gen_codebook.o hand_bin.o

E_OBJS = encoder.o $(OBJS)

D_OBJS = decoder.o $(OBJS)

huff_enc: $(E_OBJS)
	$(LD) $(LFLAGS) $@ $(E_OBJS)

huff_dec: $(D_OBJS)
	$(LD) $(LFLAGS) $@ $(D_OBJS)

clean:
	rm -f *.o $(EXECS)
