#include "../main.h"
#include "../utility/AVL.h"
#include "./leaks.h"

int compareID(const char* a, const char* b) {
  char* parsedIdA = strchr(a, '#');
  char* parsedIdB = strchr(b, '#');
  if (!parsedIdA || !parsedIdB) return 0;
  
  return strcmp(parsedIdA, parsedIdB) == 0;
}

double calculateLeak(Instance* node, double volume, double* maxLeak, char** mostLeaksUp, char** mostLeaksDown) {
  if (!node || node->downstreamCount == 0) return 0.0;
  
  //We cap the water distributed by facilities here
  if (node->max != 0 && volume > node->max) volume = node->max;
  
  double leakSum = 0.0;
  
  for (int i = 0; i < node->downstreamCount; i++) {
    node->downstream[i]->volume = volume / node->downstreamCount;
    
    double leak = node->downstream[i]->volume * (node->downstream[i]->leaks / 100.0);
    if (leak > *maxLeak) {
      *maxLeak = leak;
      *mostLeaksUp = node->id;
      *mostLeaksDown = node->downstream[i]->id;
    }
    node->downstream[i]->volume -= leak;
    leakSum += leak;
    leakSum += calculateLeak(node->downstream[i], node->downstream[i]->volume, maxLeak, mostLeaksUp, mostLeaksDown);
  }
  return leakSum;
}


int main(int argc, char const *argv[]) {
  int e = 0; //count errors

  if (argc < 2) {
    printf("Error: no Facility ID provided.");
    e++;
    exit(e);
  }
  char input[256];
  int h = 0;
  Node* root = NULL;
  Instance* plant = NULL;
  double maxLeak = 0.0;
  char* mostLeaksUp = NULL;
  char* mostLeaksDown = NULL;
  while (fgets(input, sizeof(input), stdin)) {
    char* facilitySrc = strtok( input, ";" );
    char* id = strtok( NULL, ";" );
    char* downstream = strtok( NULL, ";" );
    int volume = atoi(strtok( NULL, ";" ) ?: "0"); //check for NULL
    double leaks = atof(strtok( NULL, ";" ) ?: "0"); //check for NULL

    if (!facilitySrc || !id || !downstream) {
      e++;
      continue;
    }

    Node* node = searchAVL(root, id);
    Instance* instance = NULL;
    if (strcmp(facilitySrc, "-") != 0 || !compareID(downstream, argv[1])) { //Don't add sources to AVL or TREE
      if (node == NULL) {
        instance = malloc(sizeof(Instance));
        if (!instance) {
          printf("Error: Memory allocation failed.\n");
          e++;
          exit(e);
        }
        strncpy(instance->id, id, sizeof(instance->id)-1);
        instance->id[sizeof(instance->id)-1] = '\0';
        instance->max = 0;
        instance->volume = 0;
        instance->leaks = 0;
        instance->downstream = NULL;
        instance->downstreamCount = 0;
        h = 0;
        addChildAVL(&root, instance, &h);
      } else instance = node->address;
      if (instance == NULL) {
        printf("Error: AVL element lacks an address field\n");
        e++;
        continue;
      }
      if (strcmp(facilitySrc, "-") == 0 && strcmp(downstream, "-") == 0 && compareID(id, argv[1])) {
        plant = instance;
        instance->max = volume;
      }
    }
    
    if (strcmp(downstream, "-") != 0) {
        Node* downNode = searchAVL(root, downstream);
        Instance* downInst = NULL;
        if (!downNode) {
            downInst = malloc(sizeof(Instance));
            if (!downInst) {
              printf("Error: Memory allocation failed.\n");
              e++;
              exit(e);
            }
            strncpy(downInst->id, downstream, sizeof(downInst->id)-1);
            downInst->id[sizeof(downInst->id)-1] = '\0';
            downInst->max = 0;
            downInst->volume = 0;
            downInst->leaks = 0;
            downInst->downstream = NULL;
            downInst->downstreamCount = 0;
            int h = 0;
            addChildAVL(&root, downInst, &h);
        } else downInst = downNode->address;
        if (downInst == NULL) {
          printf("Error: AVL element lacks an address field\n");
          e++;
          continue;
        }
        if (strcmp(facilitySrc, "-") == 0 && compareID(downInst->id, argv[1])) {
          downInst->volume += volume * (1.0 - leaks/100.0);
        }
        
        if (instance) {
          downInst->leaks += leaks;
          Instance** reallocMemory = realloc(instance->downstream, sizeof(Instance*) * (instance->downstreamCount + 1));
          if (!reallocMemory) {
            printf("Error: Memory allocation failed.\n");
            e++;
            exit(e);
          }
          instance->downstream = reallocMemory;
          instance->downstream[instance->downstreamCount++] = downInst;
        }
    }
  }

  printf("leaks: data parsed, computing leaks...\n");
  if (!plant) {
    printf("Could not find a facility with id: %s\n", argv[1]);
  } else {
    plant->leaks = calculateLeak(plant, plant->volume, &maxLeak, &mostLeaksUp, &mostLeaksDown);
  }

  printf("leaks: values computed, generating .dat file...\n");
  FILE* file = fopen("./DATA/leaks_log.dat", "a");
  if (!file) {
    printf("Failed to open %s.\n", "./DATA/all.csv");
    e++;
    exit(e);
  }
  if (plant) {
    fprintf(file, "%s;%.3f k.m³;%.3f;%s;%s\n", plant->id, plant->leaks, plant->leaks/plant->volume, mostLeaksUp ?: "-", mostLeaksDown ?: "-");
  } else {
    fprintf(file, "%s;-1 k.m³;-1;-;-\n", argv[1]);
  }
  
  fclose(file);

  deleteAllChilds(&root);

  return e;
}
