# === editor 层 ===
include ../rules.mk

all: editor

editor: src/main.c
	$(CC) $(CFLAGS) $< $(LDLIBS) $(LDFLAGS) -o $@

clean:
	rm -f editor

.PHONY: all clean
