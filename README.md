How to run: First run "make" in order to create the inodes file. Then run "./inodes <pathname>", where pathname is the path of the file you wish to make a graph of. To create the PDF, run "dot -Tpdf:cairo inodes.gv -o inodes.pdf".

Challenges faced: I had a bit of trouble figuring out how to access the directory's children. I originally thought I could read the directory file, accessing the names from there. But eventually, I found a library that allowed me to access a directory's files more directly. Once I solved that, I needed to figure out how to get the name of the path, and then pass it as an argument. I had trouble creating the path, since after the first file it would just add on to the path name instead of starting from scratch. I eventually managed to fix it though.

Resources:
https://man7.org/linux/man-pages/man2/lstat.2.html
https://man7.org/linux/man-pages/man0/dirent.h.0p.html
