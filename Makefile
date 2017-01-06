
TARGETS+= post
LDFLAGS=-L/usr/local/lib -lcurl

OBJS= xcurl_ext_debug.o xcurl_write_data.o
OBJS+= ioxutil/cafenv.o ioxutil/tini/tini.o

include Makefile.common
