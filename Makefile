DEPS=gtk+-3.0 rest-0.7 json-glib-1.0 gio-2.0
HEADERS=h2oxs.h h2oxs-win.h h2oxs-oauth.h h2oxs-social.h h2oxs-message.h
FILES=$(HEADERS) main.c h2oxs.c h2oxs-win.c h2oxs-oauth.c h2oxs-social.c h2oxs-message.c

all:
	gcc -g `pkg-config --libs --cflags $(DEPS)` $(FILES) -o h2oxs
