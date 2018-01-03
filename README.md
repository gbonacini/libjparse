libjparse
=========

This library is a C library that permits to parse JSON configuration files.
The core functionality is provided by a Bison/Flex (LEX/YACC) parser. This library was tested on OS X Mavericks (10.9)
with LLVM version 6.0 (clang-600.0.51) and on Debian 7.6 (Wheezy) x86_64.
It's my intention extend the support at least for AIX.

build:
======

 autoreconf -ivf
 ./configure
 make install ( or sudo make install)
