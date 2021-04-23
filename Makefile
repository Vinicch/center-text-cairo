build:
	gcc -o cairo main.c $$(pkg-config --cflags --libs cairo)