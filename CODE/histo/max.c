#include "../main.h"
#include "./max.h"
#include "./AVL.h"

int compare_facility(const void *a, const void *b) {
  const Facility *ua = a;
  const Facility *ub = b;
  return ua->volume - ub->volume;
}

int main() {

  char buf[256];
  int n = 0;
  Facility facilities[10000];

  while (fgets(buf, sizeof(buf), stdin)) {
    char * facilitySrc = strtok( buf, ";" );
    char * id = strtok( NULL, ";" );
    char * aval = strtok( NULL, ";" );
    char * volume = strtok( NULL, ";" );
    char * fuite = strtok( NULL, ";" );

    if (!id || !volume) continue;  // sécurité

    strncpy(facilities[n].id, id, 31);
    facilities[n].id[sizeof(facilities[n].id)-1] = '\0';
    facilities[n].volume = atoi(volume);

    n++;


  }
  printf("Count: %d\n", n);

  qsort(facilities, n, sizeof(Facility), compare_facility);
  int h;
  Node* root = makeNodeAVL(&facilities[0]);
  addChildAVL(&root, &facilities[1], &h);
  addChildAVL(&root, &facilities[2], &h);
  printTree(root, 0);
  printf("\n");

  // bottom 50
  FILE *csv1 = fopen("./histo/bottom50.csv", "w");
  fprintf(csv1, "identifiant,capacite\n");
  for (int i = 0; i < 50 && i < n; i++) {
      fprintf(csv1, "%s,%d\n", facilities[i].id, facilities[i].volume);
  }
  fclose(csv1);

  // top 10
  FILE *csv2 = fopen("./histo/top10.csv", "w");
  fprintf(csv2, "identifiant,capacite\n");
  for (int i = n - 10; i < n; i++) {
      if (i >= 0)
          fprintf(csv2, "%s,%d\n", facilities[i].id, facilities[i].volume);
  }
  fclose(csv2);

  return 0;
}
