# === baseutils 层: 一组基础命令 (cat/clear/echo/ls/mkdir/rm/touch) ===
include ../rules.mk

APPS := cat clear echo ls mkdir rm touch
BINS := $(foreach a,$(APPS),$(a)/$(a))

all: $(BINS)

# 每个二进制依赖其目录下的 src/main.c, 例如 cat/cat <- cat/src/main.c
define APP_RULE
$(1)/$(1): $(1)/src/main.c
	$(CC) $(CFLAGS) $$< $(LDLIBS) $(LDFLAGS) -o $$@
endef
$(foreach a,$(APPS),$(eval $(call APP_RULE,$(a))))

clean:
	rm -f $(BINS)

.PHONY: all clean
