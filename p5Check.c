#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pnmrdr.h>

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

int main(int argc, char* argv[])
{
  if (argc > 1){
    printf("%s\n", argv[1]);
    FILE *plainF  = open_or_abort(argv[1], "rb");
    Pnmrdr_T reader  = Pnmrdr_new(plainF);
    Pnmrdr_mapdata mapData = Pnmrdr_data(reader);
    printf("The width %u, height %u and maxVal are %u\n", mapData.width, mapData.height, mapData.denominator);
    printf("The total number of pixels in the file are %u\n", mapData.width * mapData.height);
    printf("Reading in all pixels in the file\n");


    for (int i = 0; i <= mapData.width * mapData.height; i++) {
        Pnmrdr_get(reader));
        }

    printf("All pixels have been read sucessfully\n");
    fclose(plainF);
  }
  else{
    fprintf(stderr,"Too few arguments\n");
    exit(1);
  }

  return 0;
}
