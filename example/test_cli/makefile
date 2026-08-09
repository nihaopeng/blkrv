# === test_cli 层 ===
include ../rules.mk

all: test_cli

test_cli: src/main.c
	$(CC) $(CFLAGS) $< $(LDLIBS) $(LDFLAGS) -o $@

clean:
	rm -f test_cli

.PHONY: all clean
