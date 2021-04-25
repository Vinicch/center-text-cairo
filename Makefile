cairo-build:
	gcc -o watermark cairo.c $$(pkg-config --cflags --libs cairo)

cairo:
	make cairo-build && ./watermark

pango-build:
	gcc -o watermark pango.c $$(pkg-config --cflags --libs cairo pangocairo)

pango:
	make pango-build && ./watermark