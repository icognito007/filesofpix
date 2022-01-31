#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <ctype.h>

#include <table.h>
#include <atom.h>
#include <list.h>

#include <readaline.h>
#include <hashStore.h>

static FILE *open_or_abort(char *fname, char *mode) {
  FILE *fp = fopen(fname, mode);
  if (fp == NULL) {
    int rc = errno;
    fprintf(stderr, "Could not open file %s with mode %s\n", fname, mode);
    exit(rc);
  }
  return fp;
}

const char *uncorruptLine(int bytes, char **datapp, char **uncorruptedpp) {
  // needs to be freed elsewhere
  *uncorruptedpp = malloc((bytes + 1) * sizeof(char));
  char *corruptionString = malloc((bytes + 1) * sizeof(char));

  int corruptionNum = 0;

  char currentChar;
  for (int i = 0; i < bytes; i++) {
    currentChar = (*datapp)[i];
    if (isdigit(currentChar)) {
      (*uncorruptedpp)[i] = (*datapp)[i];
    } else {
      (*uncorruptedpp)[i] = ' ';
      corruptionString[corruptionNum++] = (*datapp)[i];
    }
  }
  corruptionString[corruptionNum] = '\0';
  (*uncorruptedpp)[bytes] = '\0';

  // also needs to be freed elsewhere
  const char *corruptionAtom = Atom_new(corruptionString, corruptionNum);
  free(corruptionString);
  return corruptionAtom;
}

int main(int argc, char* argv[])
{
  printf("Hello World\n");

  if (argc > 1) {
    printf("%s\n", argv[1]);
    FILE *plainF  = open_or_abort(argv[1], "rb");

    char **datapp = malloc(sizeof(char*));
    size_t bytesRead = readaline(plainF, datapp);

    char **uncorruptedpp = malloc(sizeof(char*));

    Table_T *hashTablep = malloc(sizeof(Table_T*));
    *hashTablep = Table_new(10, NULL, NULL);
    List_T knownGoodList = NULL;

    while (*datapp != NULL) {
      printf("bytes read: %d, data read: \"%s\"\n", (int) bytesRead, *datapp);
      const char *corruptionAtomp =
        uncorruptLine(bytesRead, datapp, uncorruptedpp);
      free(*datapp);

      printf("uncorrupted string: \"%s\"\n", *uncorruptedpp);
      printf("corruption string:  \"%s\"\n", Atom_string(corruptionAtomp));

      List_T currList = hashStore(corruptionAtomp, uncorruptedpp, *hashTablep);
      if (currList == NULL) {
        printf("null pointer\n\n");
      } else {
        printf("good list!\n\n");
        knownGoodList = currList;
      }

      bytesRead = readaline(plainF, datapp);
    }

    printf("known good list length: %d\n", List_length(knownGoodList));
    knownGoodList = List_reverse(knownGoodList);
    int knownGoodListLength = List_length(knownGoodList);
    for (int i = 0; i < knownGoodListLength; i++) {
      void **ptr = malloc(sizeof(void *));
      knownGoodList = List_pop(knownGoodList, ptr);
      char **charp = (char **) ptr;
      printf("\"%s\"\n", *charp);
    }

    Table_free(hashTablep);
    free(hashTablep);
    fclose(plainF);
    free(*uncorruptedpp);
    free(uncorruptedpp);
    free(datapp);
  }
  else {
    fprintf(stderr,"Too few arguments\n");
    exit(1);
  }

  return 0;
}
