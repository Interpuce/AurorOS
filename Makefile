# AurorOS Makefile

.PHONY = all help shbuild depinstall

all: shbuild

help:
	@echo "To compile the AurorOS, please run `make`."
	@echo "To force installing dependencies, run `make depinstall`."

shbuild:
	chmod +x compile/build.sh
	chmod +x compile/dep_install.sh
	./compile/build.sh

depinstall:
	chmod +x compile/dep_install.sh
	./compile/dep_install.sh