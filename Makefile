DEPS=gtk+-3.0 oauth
HEADERS=h2oxs.h h2oxs-win.h h2oxs-oauth.h
FILES=$(HEADERS) main.c h2oxs.c h2oxs-win.c h2oxs-oauth.c

TEST=testtwitt.c h2oxs-oauth.c

all:
	gcc `pkg-config --libs --cflags $(DEPS)` $(FILES) -o h2oxs

test:
	gcc `pkg-config --libs --cflags $(DEPS)` $(TEST) -o test
