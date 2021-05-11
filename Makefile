make: colsh.c
	gcc -O2 -pipe -g -Wall -o colsh colsh.c

clean:
	rm colsh 
