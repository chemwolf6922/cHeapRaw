CFLAGS?=-O3
override CFLAGS+=-MMD -MP
LDFLAGS?=
LIB_SRC=heap.c
STATIC_LIB=libheap.a
TEST_SRC=test.c $(LIB_SRC)

all:test lib

test:$(patsubst %.c,%.o,$(TEST_SRC))
	$(CC) $(LDFLAGS) -o $@ $^

lib:$(STATIC_LIB)

$(STATIC_LIB):$(patsubst %.c,%.o,$(LIB_SRC))
	$(AR) -rcs -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c $<

-include $(TEST_SRC:.c=.d)

clean:
	rm -f *.o *.d test $(STATIC_LIB) 
