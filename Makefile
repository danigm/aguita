DEPS=gtk+-3.0
HEADERS=h2oxs.h h2oxs-win.h
FILES=$(HEADERS) main.c h2oxs.c h2oxs-win.c

all:
	gcc `pkg-config --libs --cflags $(DEPS)` $(FILES) -o main
