# === ppt_player 层 ===
include ../rules.mk

all: ppt_player

ppt_player: src/main.c
	$(CC) $(CFLAGS) $< $(LDLIBS) $(LDFLAGS) -o $@

clean:
	rm -f ppt_player

.PHONY: all clean
