make: yash.c
	gcc -O2 -pipe -g -Wall -o yash yash.c

clean:
	rm yash
