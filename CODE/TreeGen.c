#include "main.h"
#include "./AVL.h"
#include "./TreeGen.h"

Instance* findAVL(const char* id) {
    (void)id;
    return NULL;
}

struct Instance** parent(Instance* s) {
    (void)s;
    return NULL;
}

void insertTree(struct Instance** target, Instance* s) {
    (void)target;
    (void)s;
    /* no-op stub */
}

void printInstance(Instance* instance) {
  printf("SRC = %s | ID = %s | IN = %d | OUT = %f\n", instance->plantSrc != NULL ? instance->plantSrc->id : NULL, instance->id, instance->volumeIn, instance->leaksOut);
}

int main(int argc, char const *argv[]) {
  printf("Program started");
  char plantSrc[81];
  char upstreamId[81];
  char downstreamId[81];
  char volume[81]; //read as string for '-' case
  char leaks[81]; //read as string for '-' case
  int i = 1;
  while (i == 1 && scanf("%80[^;];%80[^;];%80[^;];%80[^;];%80[^\n]\n", plantSrc, upstreamId, downstreamId, volume, leaks) == 5) {
    Instance* firstInstance = malloc(sizeof(Instance));
    i = 0;
    strcpy(firstInstance->id, upstreamId);
    firstInstance->volumeIn = strcmp(volume, "0") == 0 ? 0 : atoi(volume) == 0 ? -1 : atoi(volume);
    firstInstance->leaksOut = strcmp(leaks, "0") == 0 ? 0 : atof(leaks) ==0 ? -1 : atof(leaks);
    firstInstance->plantSrc = findAVL(plantSrc);
    insertTree(firstInstance->downstream, findAVL(downstreamId));
    insertTree(parent(findAVL(upstreamId)), firstInstance);
    //Insert in AVL before or after maybe avoid doing findAVL by inserting in AVL first
    /*
      If upsteam != '-'
      -Check upstream in AVL
      -Doesn't exist? Create new treeNode and add reference to AVL
      -add child downstream and add downstream to AVL
      If upstream == '-'
      -Create new treeNode and add reference to AVL
    */
    printInstance(firstInstance);
  }
  printf("Program Ended");
  return 0;
}
