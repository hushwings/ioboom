iotest:ioboom.c
	gcc -D_GNU_SOURCE ioboom.c -o ioboom -g
clean:
	rm ioboom *.o
