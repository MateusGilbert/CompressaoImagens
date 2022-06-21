CC=g++
LD=g++
CFLAGS= -c -Wall --std=c++17 -O3 #-g
LFLAGS= -o

.c.o:
	$(CC) $(CFLAGS) $<

.cpp.o:
	$(CC) $(CFLAGS) $<

Q_OBJS = quantize.o hand_bands.o
COM_OBJS = hand_img.o sub.o fqmf16AA.o fqmf16AS.o
Tr_OBJS = gen_codebooks.o vectorize.o lbg.o $(Q_OBJS) $(COM_OBJS)
EV_OBJS = eval_cdbook.o $(Q_OBJS) $(COM_OBJS)
E_OBJS = encode.o $(COM_OBJS) #bit_output.o arithmetic_encode.o adaptive_model.o
#D_OBJS = decode.o #bit_input.o arithmetic_decode.o adaptive_model.o
H_OBJS = helper.o $(COM_OBJS)

EXECS = encode eval_cdbook train test_subc

all: $(EXECS)

encode: $(E_OBJS)
	$(LD) $(LFLAGS) $@ $^

decode: $(D_OBJS)
	$(LD) $(LFLAGS) $@ $^

eval_cdbook: $(EV_OBJS)
	$(LD) $(LFLAGS) $@ $^

train: $(Tr_OBJS)
	$(LD) $(LFLAGS) $@ $^

test_subc: $(H_OBJS)
	$(LD) $(LFLAGS) $@ $^

#programs inside other directories
vectorize.o: lbg_vq/vectorize.cpp
	$(CC) $(CFLAGS) $^ -o $@

lbg.o: lbg_vq/lbg.cpp
	$(CC) $(CFLAGS) $^ -o $@


clean:
	rm -f *.o $(EXECS)
