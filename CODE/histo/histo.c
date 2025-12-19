#include "../main.h"
#include "./histo.h"
#include "./AVL.h"

int compare_max(const void *a, const void *b) {
  const Facility *ua = a;
  const Facility *ub = b;
  return ua->max - ub->max;
}

int compare_src(const void *a, const void *b) {
  const Facility *ua = a;
  const Facility *ub = b;
  return ua->src - ub->src;
}

int compare_real(const void *a, const void *b) {
  const Facility *ua = a;
  const Facility *ub = b;
  return ua->real - ub->real;
}

int main(int argc, char const *argv[]) {

  printf("\n %s \n ", argv[1]);


  char buf[256];
  int n = 0;
  int h = 0;
  Node* root = NULL;
  while (fgets(buf, sizeof(buf), stdin)) {
    char * facilitySrc = strtok( buf, ";" );
    char * id = strtok( NULL, ";" );
    char * aval = strtok( NULL, ";" );
    char * max = atoi(strtok( NULL, ";" ));
    char * fuite = atof(strtok( NULL, ";" ));
    if (n<10) printf("Reading: %s\n", id);

    if (!id || !max) continue;  // sécurité



    if (max == 0 && strcmp(facilitySrc, "-") == 0) { // We check if it's a facility
      Node* exist = searchAVL(root, id);
      if (!exist) {
        printf("\nData is problematic\n");
        exit(12);
      }
      exist->address->max = max;
    } else {
      Node* node_avl = searchAVL(root, aval);
      Facility facility = malloc(sizeof(Facility));
      if (node_avl == NULL) {
        facility->id = id;
        facility->max = 0;
        facility->src = 0;
        facility->real = 0;
        int h = 0;
        addChildAVL(&root, &facility, &h);
        n++;
      } else facility = node_avl->address;

      facility->src += max;
      facility->real += max*fuite;
    }

    facility->src = min2(facility->src, max);
    facility->real = facility->src >= max*fuite ? facility->src : max*fuite;

    // Facility facility = malloc(sizeof(Facility));
    // facility->id = id;
    // facility->max = 0;
    // facility->src = 0;
    // facility->real = 0;
    //
    // int h=0;
    // Node* existing_facility = findAVL(root, &facility);
    // addChildAVL(&root, &facility, &h);


    // strncpy(facilities[n].id, id, 31);
    // facilities[n].id[sizeof(facilities[n].id)-1] = '\0';
    // facilities[n].max = atoi(max);
    //
    // h = 0;
    // addChildAVL(&root, &facilities[n], &h);
    // n++;

    //AVL d'abord (créer dans la boucle)
    //Créer une struct Facility
    //Check la commande
    //Convertir en tableau
    //En fonction de la commande trier les éléments

  }

  printTree(root, 0);
  printf("\n");
  printf("Count: %d\n", n);

  // if (argv[1] == "max") {
  //   /* code */
  // }

  Facility facilities[n]; //Liste chainée
  qsort(facilities, n, sizeof(Facility), compare_facility);

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
