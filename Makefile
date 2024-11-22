.PHONY = all windows other

ifeq ($(OS), Windows_NT)
    TARGET = windows
else
    TARGET = other
endif

all: $(TARGET)

windows:
	compile\windows.exe

other:
	python3 compile/build.py