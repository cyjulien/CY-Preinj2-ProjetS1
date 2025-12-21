#include "../main.h"
#include "./definition.h"

void freeInstance(Instance* instance) {
  if (instance == NULL) return;
  if (instance->downstream) {
    free(instance->downstream);
    instance->downstream = NULL;
  }
  free(instance);
}