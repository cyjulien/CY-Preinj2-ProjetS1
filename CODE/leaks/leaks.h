#include "../main.h"

typedef struct Instance {
  struct Instance* plantSrc;
  char id[128];
  int volumeIn;
  float leaksOut;
  struct Instance** upstream;
  struct Instance** downstream;
} Instance;

/* Prototypes for tree/AVL helpers (stubs provided in TreeStubs.c) */
Instance* findAVL(const char* id);
struct Instance** parent(Instance* s);
void insertTree(struct Instance** target, Instance* s);

int main(int argc, char const *argv[]);
