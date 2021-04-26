build:
	gcc -o multiline_cairo cairo.c $$(pkg-config --cflags --libs cairo) && ./multiline_cairo

run:
	./multiline_cairo