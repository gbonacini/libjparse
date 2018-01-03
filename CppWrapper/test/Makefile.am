bin_PROGRAMS = jsonCppWrapTest jsonCppWrapTestPar
ACLOCAL_AMFLAGS = -I m4
jsonCppWrapTest_SOURCES = jsonCppWrapTest.cpp
jsonCppWrapTestPar_SOURCES = jsonCppWrapTestPar.cpp

if WITH_TEST
jsonCppWrapTest_CPPFLAGS = -I../../include -I../include
jsonCppWrapTest_LDADD = -L../src -L../../src -ljsonCppWrap -L. -ljparse
jsonCppWrapTestPar_CPPFLAGS = -I../../include -I../include
jsonCppWrapTestPar_LDADD = -L../src -L../../src -ljsonCppWrap -L. -ljparse
else
jsonCppWrapTest_CPPFLAGS = -I/usr/local/include/ 
jsonCppWrapTest_LDADD = -L. -L../src -ljsonCppWrap -ljparse
jsonCppWrapTestPar_CPPFLAGS = -I/usr/local/include/ 
jsonCppWrapTestPar_LDADD = -L. -L../src -ljsonCppWrap -ljparse
endif


