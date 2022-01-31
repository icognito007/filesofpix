#include <list.h>

void printasp5(List_T kgl)

void p2top5(List_T kgl) {
  int width = 0;
  int height = 0;

  // process first line
  // reset when we encounter whitespace
  // otherwise, we multiply current value by 10 and add the int at the current index
  int currToken = 0;
}

void convertLineToBin(char **currLine) {
  char *convertedLine = malloc() //manage storage
  int index = 0;

  char currentChar = (*currLine)[index];
  int currentToken = 0;
  bool readingToken = false;
  while (currentChar != '\0') {
    if (isdigit(currentChar)) {
      readingToken = true;
      currentToken = (currentToken * 10) + (currentChar - '0');
    } else {
      if (readingToken) {

      }
      
    }
  }
}