all: a.out

a.out: main.c imp.c myhead.h
	gcc -o a.out main.c imp.c ./lib/sllist.c

clean:
	rm a.out
