#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <atom.h>
#include <readaline.h>


static FILE *open_or_abort(char *fname, char *mode)
{
  FILE *fp = fopen(fname, mode);
  if (fp == NULL) {
    int rc = errno;
    fprintf(stderr, "Could not open file %s with mode %s\n", fname, mode);
    exit(rc);
  }
  return fp;
}

const char *uncorruptLine(int bytes, char **datapp, char **uncorruptedp) {
  // needs to be freed elsewhere
  *uncorruptedp = malloc((bytes + 1) * sizeof(char));
  char *corruptionString = malloc((bytes + 1) * sizeof(char));

  int corruptionNum = 0;

  char currentChar;
  for (int i = 0; i < bytes; i++) {
    currentChar = (*datapp)[i];
    if (isdigit(currentChar)) {
      (*uncorruptedp)[i] = (*datapp)[i];
    } else {
      (*uncorruptedp)[i] = ' ';
      corruptionString[corruptionNum++] = (*datapp)[i];
    }
  }
  corruptionString[corruptionNum] = '\0';
  (*uncorruptedp)[bytes] = '\0';

  // also needs to be freed elsewhere
  const char *corruptionAtom = Atom_new(corruptionString, corruptionNum);
  free(corruptionString);
  return corruptionAtom;
}

int main(int argc, char* argv[])
{
  printf("Hello World\n");

  if (argc > 1){
    printf("%s\n", argv[1]);
    FILE *plainF  = open_or_abort(argv[1], "rb");
    char **datapp = malloc(sizeof(char*));
    size_t bytesRead = readaline(plainF, datapp);
    printf("bytes read: %d, data read: \"%s\"\n", (int) bytesRead, *datapp);


  
    char **uncorruptedp = malloc(sizeof(char*));
    
    const char *corruptionAtomp =
      uncorruptLine(bytesRead, datapp, uncorruptedp);
      
    
    printf("uncorrupted string: \"%s\"\n", *uncorruptedp);
    printf("corruption string:  \"%s\"\n", Atom_string(corruptionAtomp));

    fclose(plainF);
    free(*datapp);
    free(datapp);
    free(*uncorruptedp);
    free(uncorruptedp);
  }
  else {
    fprintf(stderr,"Too few arguments\n");
    exit(1);
  }

  return 0;
}
