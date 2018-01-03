lib_LTLIBRARIES = libjsonCppWrap.la
libjsonCppWrap_la_SOURCES = jsonCppWrap.cpp 
libjsonCppWrap_la_LDFLAGS = -version-info 1:0:0
include_HEADERS = ../include/jsonCppWrap.hpp

ACLOCAL_AMFLAGS = -I m4

AM_YFLAGS = --defines=$(PWD)/../include/jsonCppWrap.hpp

if WITH_TEST
libjsonCppWrap_la_CPPFLAGS = $(CPPFLAGS) -I../include 
else
libjsonCppWrap_la_CPPFLAGS = $(CPPFLAGS) -I/usr/local/include/
endif

