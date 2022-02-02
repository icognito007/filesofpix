#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <ctype.h>

#include <table.h>
#include <atom.h>
#include <list.h>

#include <readaline.h>
#include <hashStore.h>
#include <p2top5.h>

// This function is an idiom from the COMP 40 website
// https://www.cs.tufts.edu/comp/40/docs/idioms.html
static FILE *open_or_abort(char *fname, char *mode) {
  FILE *fp = fopen(fname, mode);
  if (fp == NULL) {
    int rc = errno;
    fprintf(stderr, "Could not open file %s with mode %s\n", fname, mode);
    exit(rc);
  }
  return fp;
}

// This function uncorrupts a single line of input, replacing the corruption
// characters with spaces and returning the corruption chars as char *.
// bytes is the number of bytes to read
// datapp is the pointer to the data char *
// uncorruptedpp is a passed by reference pointer to the location where the
//   uncorrupted string should be stored
// corruptionNum is a passed by reference pointer to the number of corrupted
//   characters in the line, which is used to create the atoms to store the
//   strings in a table
char *uncorruptLine(int bytes, char **datapp, char **uncorruptedpp,
    int* corruptionNum) {
  // needs to be freed elsewhere
  *uncorruptedpp = malloc((bytes + 1) * sizeof(char));
  char *corruptionString = malloc((bytes + 1) * sizeof(char));

  *corruptionNum = 0;

  char currentChar;
  for (int i = 0; i < bytes; i++) {
    currentChar = (*datapp)[i];
    if (isdigit(currentChar)) {
      (*uncorruptedpp)[i] = (*datapp)[i];
    } else {
      (*uncorruptedpp)[i] = ' ';
      corruptionString[(*corruptionNum)++] = (*datapp)[i];
    }
  }
  corruptionString[(*corruptionNum)] = '\0';
  (*uncorruptedpp)[bytes] = '\0';

  return corruptionString;
}

// function to free the singular char ** corruption strings of corrupted lines
void freeListElement(void **element, void *cl) {
  (void) cl;
  char **charElementpp = (char **) element;
  free(*charElementpp);
}

// function to free the List elements of the table
void freeTableElements(const void *key, void **value, void *cl) {
  List_T *valueListp = (List_T *) value;
  const char *knownGoodAtom = (const char *) cl;

  // check if the current pair is the known good List, which is freed in
  // another part of the codebase
  if (knownGoodAtom != key) {
    List_map((*valueListp), freeListElement, NULL);
    List_free(valueListp);
  }
}

int main(int argc, char* argv[])
{
  if (argc > 1) {
    // Open the file
    FILE *plainF  = open_or_abort(argv[1], "rb");

    // Allocate the data pointer, uncorrupted data pointer and read in the
    // first line
    char **datapp = malloc(sizeof(char*));
    size_t bytesRead = readaline(plainF, datapp);
    char **uncorruptedpp = malloc(sizeof(char*));

    // Create table to hold Lists of uncorrupted strings, keyed by atoms of
    // corrupted strings
    Table_T *hashTablep = malloc(sizeof(Table_T*));
    *hashTablep = Table_new(10, NULL, NULL);
    
    // Track the known good list with a variable to hold it and the necessary
    // variables to recreate the atom that holds it
    List_T knownGoodList = NULL;
    int knownGoodCorruptionNumber;
    // Placeholder data
    char* knownGoodCorruptionString = malloc(1);

    // Loop until we are out of data
    while (*datapp != NULL) {
      // Uncorrupt the current line, then free the current input data
      int corruptionNum;
      char *corruptionString =
        uncorruptLine(bytesRead, datapp, uncorruptedpp, &corruptionNum);
      free(*datapp);

      // Store the current uncorrupted data in the table
      List_T currList = hashStore(
        Atom_new((const char *) corruptionString, corruptionNum), 
        uncorruptedpp,
        *hashTablep);

      // Check to see if the storage attempt returned a list back, indicating
      // that we had already stored a list with the same string of corruption
      // characters, meaning this is in the original file
      if (currList != NULL) {
        // Update the known good list to indicate that we've found 
        knownGoodList = currList;
        free(knownGoodCorruptionString);
        knownGoodCorruptionString = corruptionString;
        knownGoodCorruptionNumber = corruptionNum;
      } else {
        free(corruptionString);
      }

      bytesRead = readaline(plainF, datapp);
    }
  
    knownGoodList = List_reverse(knownGoodList);
    printasp5(knownGoodList);

    Table_map(*hashTablep, freeTableElements,
      (void *) Atom_new(knownGoodCorruptionString, knownGoodCorruptionNumber));
    Table_free(hashTablep);
    free(knownGoodCorruptionString);
    free(hashTablep);
    fclose(plainF);
    free(uncorruptedpp);
    free(datapp);
  }
  else {
    fprintf(stderr,"Too few arguments\n");
    exit(1);
  }

  return 0;
}
