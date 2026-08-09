# === shell 层 ===
include ../rules.mk

all: shell

shell: src/main.c
	$(CC) $(CFLAGS) $< $(LDLIBS) $(LDFLAGS) -o $@

clean:
	rm -f shell

.PHONY: all clean
