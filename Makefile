CC?=gcc
AR?=ar
BUILD_DIR?=$(shell pwd)/build/

CFLAGS=-O3
LDFLAGS=
LIBS=

LIBSRCS=heap.c
APPSRCS=test.c $(LIBSRCS)

all:$(BUILD_DIR)app lib

lib:$(BUILD_DIR)libheap.a

$(BUILD_DIR)%.o:%.c $(BUILD_DIR)%.d | $(BUILD_DIR)
	$(CC) -MT $@ -MMD -MP -MF $(BUILD_DIR)$*.d $(CFLAGS) -o $@ -c $<

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)app:$(patsubst %.c,$(BUILD_DIR)%.o,$(APPSRCS))
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)libheap.a:$(patsubst %.c,$(BUILD_DIR)%.o,$(LIBSRCS))
	$(AR) -rcs -o $@ $^ $(LIBS)

$(patsubst %.c,$(BUILD_DIR)%.d,$(APPSRCS)):
include $(patsubst %.c,$(BUILD_DIR)%.d,$(APPSRCS))

clean:
	rm -rf $(BUILD_DIR)