#include "../main.h"
#include "../utility/AVL.h"
#include "../utility/utility.h"
#include "./histo.h"

int compareMax(const void *a, const void *b) {
    const Instance *ua = *(const Instance **)a;
    const Instance *ub = *(const Instance **)b;
    return ua->max - ub->max;
}

int compareSrc(const void *a, const void *b) {
    const Instance *ua = *(const Instance **)a;
    const Instance *ub = *(const Instance **)b;
    return ua->volume - ub->volume;
}

int compareReal(const void *a, const void *b) {
    const Instance *ua = *(const Instance **)a;
    const Instance *ub = *(const Instance **)b;
    return (ua->leaks > ub->leaks) - (ua->leaks < ub->leaks);
}

int main(int argc, char const *argv[]) {
  char input[256];
  int n = 0;
  int h = 0;
  Node* root = NULL;
  while (fgets(input, sizeof(input), stdin)) {
    char* facilitySrc = strtok( input, ";" );
    char* id = strtok( NULL, ";" );
    char* downstream = strtok( NULL, ";" );
    char* tmp = strtok( NULL, ";" ); //temporary variable to check for NULL
    int max = tmp ? atoi(tmp) : 0;
    tmp = strtok( NULL, ";" ); //temporary variable to check for NULL
    double leaks = tmp ? atof(tmp) : 0.0;

    if (!facilitySrc || !id || !downstream) continue;
    
    if (strcmp(facilitySrc, "-") == 0 && strcmp(downstream, "-") == 0) { //FACILITY
      Node* exist = searchAVL(root, id);
      if (!exist || exist->address == NULL) {
        printf("Facility %s not found\n", id);
        continue;
      }
      exist->address->max = max;
    } else { //SOURCE -> FACILITY
      Node* node = searchAVL(root, downstream);
      Instance* facility = NULL;
      if (node == NULL) {
        facility = malloc(sizeof(Instance));
        if (!facility) exit(88);
        strncpy(facility->id, downstream, sizeof(facility->id)-1);
        facility->id[sizeof(facility->id)-1] = '\0';
        facility->max = 0;
        facility->volume = 0;
        facility->leaks = 0;
        facility->downstream = NULL;
        facility->downstreamCount = 0;
        h = 0;
        addChildAVL(&root, facility, &h);
        n++;
      } else facility = node->address;
      if (facility == NULL) {
        printf("Facility %s not found\n", downstream);
        continue;
      }

      facility->volume += max;
      facility->leaks += max*(1-(leaks/100.0));
    }
  }

  int index = 0;
  Instance** facilities = malloc(n * sizeof(Instance*));
  if (!facilities) exit(88);
  AVLToList(root, facilities, &index);
  for (int i = 0; i < n; i++) {
    if (facilities[i]->leaks > facilities[i]->max) facilities[i]->leaks = facilities[i]->max;
  }
  
  if (argc < 2) {
    deleteAllChilds(&root);
    printf("No command was passed to: histo.c (only %d/2 arguments were read).\n", argc);
    exit(122);
  }
  
  if (strcmp(argv[1], "max") == 0) {
    qsort(facilities, n, sizeof(Instance*), compareMax);
  } else if (strcmp(argv[1], "src") == 0) {
    qsort(facilities, n, sizeof(Instance*), compareSrc);
  } else if (strcmp(argv[1], "real") == 0) {
    qsort(facilities, n, sizeof(Instance*), compareReal);
  }

  // bottom 50
  FILE *csv1 = fopen("./DATA/bottom50.csv", "w");
  if (!csv1) {
    printf("Failed to open bottom50.csv.\n");
    exit(54);
  }
  
  fprintf(csv1, "id,max,src,real\n");
  for (int i = 0; i < 50 && i < n; i++) {
    fprintf(csv1, "%s,%d,%d,%.6f\n", facilities[i]->id, facilities[i]->max, facilities[i]->volume, facilities[i]->leaks);
  }
  fclose(csv1);

  // top 10
  FILE *csv2 = fopen("./DATA/top10.csv", "w");
  if (!csv2) {
    printf("Failed to open top10.csv.\n");
    exit(54);
  }
  fprintf(csv2, "id,max,src,real\n");
  for (int i = max2(n - 10, 0); i < n; i++) {
    fprintf(csv2, "%s,%d,%d,%.6f\n", facilities[i]->id, facilities[i]->max, facilities[i]->volume, facilities[i]->leaks);
  }
  fclose(csv2);

  deleteAllChilds(&root);
  
  free(facilities);

  return 0;
}
