// readaline.c
// Michael Simpson (msimps03) and Ali Khan (mkhan16)

#include <stdlib.h>
#include <readaline.h>

size_t readaline (FILE *inputfd, char **datapp) {
  if (inputfd == NULL || datapp == NULL) {
    if (inputfd == NULL){
      fprintf(stderr, "readaline error: inputfd is a null pointer \n");
      return 0;
    }
    else {
      fprintf(stderr, "readaline error: datapp is a null pointer \n");
      return 0;
    }
  }

  if (feof(inputfd)) {
    *datapp = NULL;
    return 0;
  }

  *datapp = malloc(50 * sizeof(char));
  if (*datapp == NULL) {
    //handle error
    fprintf(stderr, "readline error: could not allocate memory for pointer datapp\n");
    return 0;
  }

  size_t bytesRead = 0;
  int currentChar; // make this an int for the EOF error
  while ((currentChar = getc(inputfd)) != '\n' && currentChar != EOF) {
    (*datapp)[(int) bytesRead] = currentChar;
    bytesRead++;
    if (bytesRead % 50 == 0) {
      *datapp = realloc(*datapp, (bytesRead + 50) * sizeof(char));
      if (*datapp == NULL) {
        fprintf(stderr, "readline error: could not allocate the memory for pointer datapp\n");
        return bytesRead;
      }
    }
  }
  if (currentChar != '\n' && !feof(inputfd)) {
    fprintf(stderr, "readline error: could not read till the end of line\n");
    return bytesRead;
  }

  // free(initialp);
  return bytesRead;
}
