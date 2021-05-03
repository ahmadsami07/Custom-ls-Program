all: myls

myls: myls.c
	gcc -g -o myls myls.c options.h secret_headers.h

clean:
	rm -f *.o myls
