CC=g++
LD=g++
CFLAGS= -c -Wall --std=c++17 -O3 #-g
LFLAGS= -o

.c.o:
	$(CC) $(CFLAGS) $<

.cpp.o:
	$(CC) $(CFLAGS) $<

COM_OBJS = hand_pgm.o sub.o fqmf16AA.o fqmf16AS.o #vectorize.o lbg.o
#Tr_OBJS = train.o $(COM_OBJS)
#Te_OBJS = test.o $(COM_OBJS)
E_OBJS = encode.o $(COM_OBJS) #bit_output.o arithmetic_encode.o adaptive_model.o
#D_OBJS = decode.o #bit_input.o arithmetic_decode.o adaptive_model.o
H_OBJS = helper.o $(COM_OBJS)

EXECS = encode

all: $(EXECS)

encode: $(E_OBJS)
	$(LD) $(LFLAGS) $@ $(E_OBJS)

decode: $(D_OBJS)
	$(LD) $(LFLAGS) $@ $(D_OBJS)

test_subc: $(H_OBJS)
	$(LD) $(LFLAGS) $@ $(H_OBJS)

clean:
	rm -f *.o $(EXECS)
