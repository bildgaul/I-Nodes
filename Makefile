all: inodes.c
	gcc -pthread -Wall -o inodes inodes.c

clean:
	rm -f *~ inodes inodes.gv inodes.pdf
