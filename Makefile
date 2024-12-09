# AurorOS Makefile

.PHONY = all shbuild

all:
	make shbuild

shbuild:
	chmod -x compile/build.sh
	./compile/build.sh