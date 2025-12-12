#include "main.h"

typedef struct Section {
  Section plantSrc;
  char id[128];
  int volumeIn;
  float leaksOut;
  struct Section** upstream;
  struct Section** downstream;
} Section;

typedef struct Node{
  Section* address;
  struct Node* left;
  struct Node* right;
  int balanced;
} Node;

int main(int argc, char const *argv[]) {
  char plantSrc[81];
  char upstreamId[81];
  char downstreamId[81];
  char volume[81];
  char leaks[81];
  int i = 1;
  while (i == 1 && scanf("%80[^;];%80[^;];%80[^;];%d;%f\n", plantSrc, upstreamId, downstreamId, volume, leaks) == 5) {
    i = 0;
    Section* firstSection = malloc(sizeof(Section));
    strcpy(firstSection->id, upstreamId);
    firstSection->volumeIn = volume;
    firstSection->leaksOut = leaks;
    firstSection->plantSrc = findAVL(plantSrc);
    insertTree(firstSection->downstream, downstreamId);
    insertTree(firstSection);
    /*
    Section* up = avl_find(root, upstream);
    if (!up) {
        up = create_section(upstream);
        root = avl_insert(root, up);
    }
    Section* down = avl_find(root, downstream);
    if (!down) {
        down = create_section(downstream);
        root = avl_insert(root, down);
    }
    section_add_downstream(up, down);
    section_add_upstream(down, up);
    up->volume_max = atof(volume);
    up->leaks = atof(leaks);
    */
    printf("Col1 = %s | Col2 = %s | Col3 = %s | Col4 = %d | Col5 = %f\n", plantSrc, upstreamId, downstreamId, volume, leaks);
  }
  return 0;
}
