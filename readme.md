FLETCH16
========

This is a tiny implementation of the Fletcher16 checksum for files. It is designed to be able to be run from the DOS
command line, since I need to be able to verify file integrity on a circa 1995 laptop that I'm currently trying to
get set up.

The batch files within `build` are set up to allow this program to be compiled on FreDOS using various compilers
(at the moment, DJGPP and OpenWatcom). The program should be compilable on pretty much any other platform for which
a C compiler exists, but you'll have to execute the command line yourself.
