.PHONY = all

all:
	ifeq ($(OS),Windows_NT)
		compile\windows.exe
	else
		python3 compile/build.py
	endif
