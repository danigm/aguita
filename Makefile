DEPS=gtk+-3.0 rest-0.7 json-glib-1.0
HEADERS=h2oxs.h h2oxs-win.h h2oxs-oauth.h h2oxs-social.h
FILES=$(HEADERS) main.c h2oxs.c h2oxs-win.c h2oxs-oauth.c h2oxs-social.c

all:
	gcc -g `pkg-config --libs --cflags $(DEPS)` $(FILES) -o h2oxs
