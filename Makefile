build:
	gcc -o multiline_cairo cairo.c $$(pkg-config --cflags --libs cairo libconfuse) && ./multiline_cairo

run:
	./multiline_cairo