# AurorOS Makefile

.PHONY = all shbuild

all:
	make shbuild

shbuild:
	chmod +x compile/build.sh # Making sure build.sh is executable
	./compile/build.sh
