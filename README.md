# CY-Preinj2-ProjetS1
Projet de S1 pre-ing 2


#include "../main.h"
#include "./histo.h"
#include "./AVL.h"

int compare_facility(const void *a, const void *b) {
  const Facility *ua = a;
  const Facility *ub = b;
  return ua->max - ub->max;
}

int main(int argc, char const *argv[]) {

  char buf[256];
  int n = 0;
  Facility facilities[10000]; //Liste chainée
  while (fgets(buf, sizeof(buf), stdin)) {
    if (n>=10000) break;
    char * facilitySrc = strtok( buf, ";" );
    char * id = strtok( NULL, ";" );
    char * aval = strtok( NULL, ";" );
    char * max = strtok( NULL, ";" );
    char * fuite = strtok( NULL, ";" );
    if (n<10) printf("Reading: %s\n", id);

    if (!id || !max) continue;  // sécurité

    strncpy(facilities[n].id, id, 31);
    facilities[n].id[sizeof(facilities[n].id)-1] = '\0';
    facilities[n].max = atoi(max);

    //AVL d'abord (créer dans la boucle)
    //Créer une struct Facility
    //Check la commande
    //Convertir en tableau
    //En fonction de la commande trier les éléments

    n++;
  }
  if (argv[1] == "max") {
    /* code */
  }
  printf("Count: %d\n", n);

  qsort(facilities, n, sizeof(Facility), compare_facility);
  int h = 0;
  Node* root = NULL;
  for (size_t i = 0; i < 100; i++) {
    h = 0;
    addChildAVL(&root, &facilities[i], &h);
  }
  printTree(root, 0);
  printf("\n");
  deleteAllChilds(&root);

  // bottom 50
  FILE *csv1 = fopen("./histo/bottom50.csv", "w");
  fprintf(csv1, "identifiant,capacite\n");
  for (int i = 0; i < 50 && i < n; i++) {
      fprintf(csv1, "%s,%d\n", facilities[i].id, facilities[i].max);
  }
  fclose(csv1);

  // top 10
  FILE *csv2 = fopen("./histo/top10.csv", "w");
  fprintf(csv2, "identifiant,capacite\n");
  for (int i = n - 10; i < n; i++) {
      if (i >= 0)
          fprintf(csv2, "%s,%d\n", facilities[i].id, facilities[i].max);
  }
  fclose(csv2);

  return 0;
}
