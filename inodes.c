#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

char PNAME[16]; // process name, used for output

void
printFile(struct stat sb, char *name)
{
  printf("Name: %s\n", name);
  printf("Size: %jd bytes\n", (uintmax_t)sb.st_size);
  printf("Number of allocated blocks: %jd bytes\n", (intmax_t)sb.st_blocks);
  printf("I-Node number: %ju\n", (uintmax_t)sb.st_ino);
  printf("Number of links: %ju\n", (uintmax_t)sb.st_nlink);
}

void
printDir(struct stat sb, char *name)
{
  DIR *dir;
  struct dirent *de;
  
  if (stat(name, &sb) == -1){
    fprintf(stderr, "%s: cannot stat %s\n", PNAME, name);
    return;
  }
  
  if ((dir = opendir(name)) == NULL){
    fprintf(stderr, "%s: cannot open directory %s\n", PNAME, name);
    return;
  }

  printFile(sb, name);

  do{
    if ((de = readdir(dir)) != NULL){
      char *fileName = de->d_name;
      if (strcmp(fileName, ".") && strcmp(fileName, "..") != 0){
	char pathName[512];
	strcpy(pathName, name);
        strcat(pathName, "/");
	strcat(pathName, fileName);
	if (stat(pathName, &sb) == -1){
	  fprintf(stderr, "%s: cannot stat %s\n", PNAME, pathName);
	  closedir(dir);
	  return;
	}
	
	if (S_ISDIR(sb.st_mode)){
	  printDir(sb, pathName);
	}
	else{
	  printFile(sb, fileName);
	}
      }
    }
  } while (de != NULL);

  closedir(dir);
  return;
}

int
main(int argc, char *argv[])
{
  char *PNAME = argv[0];
 
  struct stat sb;
  if (argc != 2){
    fprintf(stderr, "Usage: %s <pathname>\n", PNAME);
    return -1;
  }
  
  printDir(sb, argv[1]);
  return 0;
}
