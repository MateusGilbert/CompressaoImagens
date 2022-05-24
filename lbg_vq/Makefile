CC=g++
LD=g++
CFLAGS= -c -Wall --std=c++17 -O3
LFLAGS= -o

.cpp.o:
	$(CC) $(CFLAGS) $<

COM_OBJS = read_pgm.o vectorize.o lbg.o
Tr_OBJS = train.o $(COM_OBJS)
Te_OBJS = test.o $(COM_OBJS)

EXECS = train test

all: $(EXECS)

train: $(Tr_OBJS)
	$(LD) $(LFLAGS) $@ $(Tr_OBJS)

test: $(Te_OBJS)
	$(LD) $(LFLAGS) $@ $(Te_OBJS)

clean:
	rm -f *.o $(EXECS)
