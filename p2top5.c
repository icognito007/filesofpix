#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <ctype.h>

#include <list.h>

#include <p2top5.h>

void printasp5(List_T kgl) {
  int width = 0;
  int height = 0;
  int maxVal = 0;
  kgl = p2top5(kgl, &width, &height, &maxVal);

  printf("P5\n%d %d\n%d\n", width, height, maxVal);

  void **ptr = malloc(sizeof(void *));
  for (int i = 0; i < height; i++) {
    kgl = List_pop(kgl, ptr);
    int **currLine = (int **) ptr;

    int j = 0;
    while ((*currLine)[j] >= 0) {
      printf("%c", (char) ((*currLine)[j++]));
    }

    free(*currLine);
  }
  free(ptr);
}

List_T p2top5(List_T kgl, int *width, int *height, int *maxVal) {
  *height = List_length(kgl);

  List_T convertedKgl = List_list(NULL);

  *maxVal = 0;
  void **ptr = malloc(sizeof(void *));
  for (int i = 0; i < *height; i++) {
    kgl = List_pop(kgl, ptr);
    int lineMaxVal = convertLineToBin(ptr, width);
    *maxVal = lineMaxVal > *maxVal ? lineMaxVal : *maxVal;
    convertedKgl = List_push(convertedKgl, *((int **) ptr));
  }

  // List_free(&kgl);
  free(ptr);
  return List_reverse(convertedKgl);
}

// returns line max val
int convertLineToBin(void **ptr, int *width) {
  char *currLine = *((char **) ptr);
  int *convertedLine = malloc(50 * sizeof(int));
  
  int outputIndex = 0;
  int inputIndex = 0;
  int lineMaxVal = 0;

  char currentChar = currLine[inputIndex];
  int currentToken = 0;
  bool readingToken = false;

  do {
    if (isdigit(currentChar)) {
      readingToken = true;
      currentToken = (currentToken * 10) + (currentChar - '0');
    } else {
      if (readingToken) {
        readingToken = false;

        // check size
        if (outputIndex % 50 == 0) {
          convertedLine =
            realloc(convertedLine, (50 + outputIndex) * sizeof(int));
        }

        // check max val
        lineMaxVal = currentToken > lineMaxVal ? currentToken : lineMaxVal;

        // write char
        convertedLine[outputIndex++] = currentToken;
        currentToken = 0;
      }
    }
    currentChar = currLine[++inputIndex];
  } while (currentChar != '\0');

  // messy, but we need to make sure we read in the last token
  if (readingToken) {
    readingToken = false;

    // check size
    if (outputIndex % 50 == 0) {
      convertedLine =
        realloc(convertedLine, (50 + outputIndex) * sizeof(int));
    }

    // check max val
    lineMaxVal = currentToken > lineMaxVal ? currentToken : lineMaxVal;

    // write char
    convertedLine[outputIndex++] = currentToken;
  }

  // check size
  if (outputIndex % 50 == 0) {
    convertedLine =
      realloc(convertedLine, (51 + outputIndex) * sizeof(int));
  }

  convertedLine[outputIndex] = -1; // NEED TO KEEP TRACK OF LENGTH

  *width = outputIndex;

  *ptr = (void *) convertedLine;
  free(currLine);

  return lineMaxVal;  
}