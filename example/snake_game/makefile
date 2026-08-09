# === snake_game 层 ===
include ../rules.mk

all: snake

snake: src/main.c
	$(CC) $(CFLAGS) $< $(LDLIBS) $(LDFLAGS) -o $@

clean:
	rm -f snake

.PHONY: all clean
