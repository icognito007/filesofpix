// readaline.c
// Michael Simpson (msimps03) and Ali Khan (mkhan16)

#include <stdlib.h>
#include <readaline.h>

size_t readaline (FILE *inputfd, char **datapp) {
  if (inputfd == NULL || datapp == NULL) {
    fprintf(stderr, "Invalid ");
  }

  if (feof(inputfd)) {
    *datapp = NULL;
    return 0;
  }

  *datapp = malloc(50 * sizeof(char));
  if (*datapp == NULL) {
    //handle error
    fprintf(stderr, "Invalid input 1\n");
  }

  size_t bytesRead = 0;
  int currentChar; // make this an int for the EOF error
  while ((currentChar = getc(inputfd)) != '\n' && currentChar != EOF) {
    (*datapp)[(int) bytesRead] = currentChar;
    bytesRead++;
    if (bytesRead % 50 == 0) {
      *datapp = realloc(*datapp, (bytesRead + 50) * sizeof(char));
      if (*datapp == NULL) {
        // handle error
        fprintf(stderr, "Invalid input 2\n");
      }
    }
  }
  if (currentChar != '\n' && !feof(inputfd)) {
    // handle error
    fprintf(stderr, "Invalid input 3\n");
  }

  // free(initialp);
  return bytesRead;
}
