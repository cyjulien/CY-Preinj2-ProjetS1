#include "../main.h"
#include "../utility/AVL.h"
#include "../utility/utility.h"
#include "./histo.h"

void AVLToCSV(FILE* file, Node* root) {
  if (root == NULL) return;

  AVLToCSV(file, root->right);

  if (root->address != NULL) {
    if (root->address->leaks > root->address->max) root->address->leaks = root->address->max; //we put this check here to optimize having to traverse the whole AVL
    if (file != NULL) fprintf(file, "%s,%d,%d,%.6f\n", root->address->id, root->address->max, root->address->volume, root->address->leaks);
  }

  AVLToCSV(file, root->left);
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
    int max = atoi(strtok( NULL, ";" ) ?: "0"); //check for NULL
    double leaks = atof(strtok( NULL, ";" ) ?: "0"); //check for NULL

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

  FILE* file = fopen("./DATA/all.csv", "w");
  if (!file) {
    printf("Failed to open %s.\n", "./DATA/all.csv");
    exit(54);
  }
  fprintf(file, "id,max,src,real\n");
  AVLToCSV(file, root);
  fclose(file);

  deleteAllChilds(&root);

  return 0;
}
