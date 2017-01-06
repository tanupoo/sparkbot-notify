
TARGETS= post

LDFLAGS+=-L/usr/local/lib
LDLIBS+=-lcurl

LDFLAGS+=-Lioxutil
LDLIBS+=-lioxutil

OBJS= xcurl_ext_debug.o xcurl_write_data.o

DIRS=ioxutil

include Makefile.common
