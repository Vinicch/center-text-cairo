build:
	gcc -o cairo main.c $$(pkg-config --cflags --libs cairo pangocairo)

run:
	make build && ./cairo