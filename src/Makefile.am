lib_LTLIBRARIES = libjparse.la
libjparse_la_SOURCES = json.l jsonGram.y parseJsonConfig.c 
libjparse_la_LDFLAGS = -version-info 1:0:0
include_HEADERS = ../include/jsonParserTypes.h ../include/parseJsonConfig.h

AM_YFLAGS = --defines=$(PWD)/../include/jsonGram.h

clean-local:
	-rm -rf json.c jsonGram.c

