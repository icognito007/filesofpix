// readaline.c
// Michael Simpson (msimps03) and Ali Khan (mkhan16)

#include <stdlib.h>
#include <readaline.h>

size_t readaline (FILE *inputfd, char **datapp) {
  if (inputfd == NULL || datapp == NULL) {
    // handle error
  }

  if (feof(inputfd)) {
    *datapp = NULL;
    return 0;
  }

  *datapp = malloc(50 * sizeof(char));
  if (*datapp == NULL) {
    // handle error
  }
  char *initialp = *datapp;

  size_t bytesRead = 0;
  char currentChar;
  while ((currentChar = getc(inputfd)) != '\n' && currentChar != EOF) {
    *initialp = currentChar;
    initialp++;
    bytesRead++;
    if (bytesRead % 50 == 0) {
      *datapp = realloc(*datapp, (bytesRead + 50) * sizeof(char));
      if (*datapp == NULL) {
        // handle error
      }
    }
  }
  if (currentChar != '\n' && !feof(inputfd)) {
    // handle error
  }

  // free(initialp);
  return bytesRead;
}
