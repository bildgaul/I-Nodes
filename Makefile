all: inodes.c
	gcc -pthread -Wall -o inodes inodes.c

clean:
	rm -f *~ inodes
