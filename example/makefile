# === BLKRv example 顶层 Makefile: 分层构建, 一键编译全部子目录 ===
#
# 层级:
#   example/makefile  (顶层编排)
#   example/rules.mk  (公共编译规则层)
#   <app>/makefile    (各应用层)
#
# 用法:
#   make          一键编译 lib + 所有用户程序子目录 + hello/server
#   make -C <app> 单独编译某个子目录
#   make clean    递归清理所有子目录 + 顶层产物

LIBPATH := lib
include rules.mk

# 子目录构建顺序: lib 必须先于所有依赖它的应用
SUBDIRS := lib baseutils shell test_cli editor snake_game ppt_player

all: subdirs hello server

# 递归进入每个子目录执行 make
subdirs:
	@for d in $(SUBDIRS); do \
		echo "==> make -C $$d"; \
		$(MAKE) -C $$d; \
	done

# lib 必须先构建, hello/server 依赖它
lib:
	$(MAKE) -C lib

hello: src/hello.c lib
	$(CC) $(CFLAGS) $< $(LDLIBS) $(LDFLAGS) -o $@

server: server.cpp
	g++ server.cpp -o $@ -lpthread

clean:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d clean; \
	done
	rm -f hello server

.PHONY: all subdirs lib clean
