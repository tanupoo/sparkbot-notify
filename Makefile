
TARGETS= sparkbot-notify

ifndef LDFLAGS
LDFLAGS+=-L/usr/local/lib
LDFLAGS+=-Lioxutil
endif

LDLIBS+=-lcurl
LDLIBS+=-lioxutil

OBJS= xcurl_ext_debug.o xcurl_write_data.o

DIRS=ioxutil

include Makefile.common
