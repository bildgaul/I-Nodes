#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

char PNAME[16]; // process name, used for output

void
printFile(const struct stat sb, const char *name, FILE *fin)
{
  if (S_ISDIR(sb.st_mode))
      fprintf(fin, "DIR%s [label = \"", name);
  else
    fprintf(fin, "%s [label = \"", name);
  
  fprintf(fin, "Name: %s\\n", name);
  fprintf(fin, "Size: %jd bytes\\n", (uintmax_t)sb.st_size);
  fprintf(fin, "Number of allocated blocks: %jd bytes\\n", (intmax_t)sb.st_blocks);
  fprintf(fin, "I-Node number: %ju\\n", (uintmax_t)sb.st_ino);
  fprintf(fin, "Number of links: %ju", (uintmax_t)sb.st_nlink);
  fprintf(fin, "\"]\n");
}

void
printDir(struct stat sb, char *name, FILE *fin)
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

  char *p;
  for (p = name + strlen(name); p >= name && *p != '/'; p--);
  p++;
  printFile(sb, p, fin);
  
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
	  fprintf(fin, "\tDIR%s -> ", p);
	  printDir(sb, pathName, fin);
	}
	else{
	  fprintf(fin, "\tDIR%s -> ", p);
	  printFile(sb, fileName, fin);
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
  
  FILE *fin;
  fin = fopen("inodes.gv", "w");
  fprintf(fin, "digraph inodes{\n");
  fprintf(fin, "\tnode [shape=folder];\n\t");
  printDir(sb, argv[1], fin);
  fprintf(fin, "}");
  fclose(fin);
  return 0;
}
