# ===========================================================================
#                             AurorOS Makefile
#
# Do not learn how to build an OS from this fucking file. If you are unsure
#  about something - try `make help`.
#
# Do not build OS on Windows. There are bash scripts that contains Linux
#  command syntax and uses tools from Linux.
# ===========================================================================

ifeq ($(OS),Windows_NT)
$(error Install Linux)
endif

.PHONY: all help install-ubuntu

all:
	./compile/start.sh

help:
	./compile/help.sh

install-ubuntu:
	./compile/packages-ubuntu.sh