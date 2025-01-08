# AurorOS Makefile

.PHONY = all shbuild

all:
	make shbuild

shbuild:
	chmod +x compile/build.sh
	chmod +x compile/dep_install.sh
	./compile/build.sh
