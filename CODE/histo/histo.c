#include "../main.h"
#include "../utility/AVL.h"
#include "./histo.h"

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
  char buf[256];
  int n = 0;
  int h = 0;
  Node* root = NULL;
  while (fgets(buf, sizeof(buf), stdin)) {
    char * facilitySrc = strtok( buf, ";" );
    char * id = strtok( NULL, ";" );
    char * aval = strtok( NULL, ";" );
    int max = atoi(strtok( NULL, ";" ));
    float fuite = atof(strtok( NULL, ";" ));
    if (n<10) printf("Reading: %s\n", id);

    if (!id || !max) continue;  // sécurité

    if (strcmp(facilitySrc, "-") == 0 && strcmp(aval, "-") == 0) { //FACILITY
      Node* exist = searchAVL(root, id);
      if (!exist) {
        printf("Facility %s not found\n", id);
        exit(12);
      }
      exist->address->max = max;
    } else { //SOURCE -> FACILITY
      Node* node = searchAVL(root, aval);
      Facility* facility = NULL;
      if (node == NULL) {
        facility = malloc(sizeof(Facility));
        if (!facility) exit(88);
        strncpy(facility->id, aval, sizeof(facility->id)-1);
        facility->id[sizeof(facility->id)-1] = '\0';
        facility->max = 0;
        facility->src = 0;
        facility->real = 0;
        int h = 0;
        addChildAVL(&root, facility, &h);
        n++;
      } else facility = node->address;

      facility->src += max;
      facility->real += max*(1-(fuite/100.0));
      facility->src = min2(facility->src, max);
      facility->real = facility->real < max ? facility->real : max;
    }
  }

  printTree(root, 0);
  printf("\n");
  printf("Count: %d\n", n);

  void avl_to_list(Node* root, Facility** list, int* index) {
    if (root == NULL)
        return;

    avl_to_list(root->left, list, index);

    list[*index] = root->address;
    (*index)++;

    avl_to_list(root->right, list, index);
  }

  int index = 0;
  Facility** list = malloc(n * sizeof(Facility*));
  avl_to_list(root, list, &index);



  if (strcmp(argv[1], "max") == 0) {
    qsort(list, n, sizeof(Facility*), compare_max);
  } else if (strcmp(argv[1], "src") == 0) {
      qsort(list, n, sizeof(Facility*), compare_src);
  } else if (strcmp(argv[1], "real") == 0) {
      qsort(list, n, sizeof(Facility*), compare_real);
  }

  deleteAllChilds(&root);

  // bottom 50
  FILE *csv1 = fopen("../DATA/bottom50.csv", "w");
  fprintf(csv1, "id,max,src,real\n");
  for (int i = 0; i < 50 && i < n; i++) {
      fprintf(csv1, "%s,%d\n", list[i]->id, list[i]->max, list[i]->src, list[i]->real);
  }
  fclose(csv1);

  // top 10
  FILE *csv2 = fopen("../DATA/top10.csv", "w");
  fprintf(csv2, "id,max,src,real\n");
  for (int i = n - 10; i < n; i++) {
      if (i >= 0)
          fprintf(csv1, "%s,%d\n", list[i]->id, list[i]->max, list[i]->src, list[i]->real);
  }
  fclose(csv2);

  return 0;
}
