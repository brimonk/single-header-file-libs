LDLIBS=
CFLAGS=-Wall -g3 -march=native -I./ -MMD -MP
SRCS=$(wildcard examples/*.c)
BINS=$(SRCS:%.c=%)
DEPS=$(SRCS:.c=.d)
OBJS=$(SRCS:.c=.o)

all: $(BINS)

%: %.o
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $

-include $(DEPS)

clean:
	rm -f $(DEPS) $(OBJS) $(BINS)
