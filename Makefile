LDLIBS=
CFLAGS=-Wall -g3 -march=native -I./
SRCS=$(wildcard examples/*.c)
BINS=$(SRCS:%.c=%)
DEPS=$(SRCS:.c=.d)

all: $(BINS)

%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT '$(@:.d=) $@' >$@

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

-include $(DEPS)

clean:
	rm -f $(DEPS) $(BINS)
