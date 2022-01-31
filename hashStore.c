#include <stdlib.h>
#include <stdio.h>

#include <atom.h>
#include <table.h>
#include <list.h>

List_T hashStore(const char *corruptionAtomp, char **uncorruptedpp,
    Table_T hashTable) {
  void *currVal = Table_get(hashTable, corruptionAtomp);

  List_T newVal;
  if (currVal == NULL) {
    newVal = List_list(*uncorruptedpp, NULL);
    Table_put (hashTable, corruptionAtomp, newVal);
    return NULL;
  } else {
    printf("storing! current list length: %d\n", List_length(currVal));
    newVal = List_push(currVal, *uncorruptedpp);
    Table_put (hashTable, corruptionAtomp, newVal);
    return newVal;
  }
}