typedef struct Instance{
  char id[81];
  int max; //Used only in histo commands
  int volume; //Corresponds to "src" in histo commands
  double leaks; //Corresponds to "real" in histo commands
  struct Instance** downstream;
  int downstreamCount;
} Instance;

void freeInstance(Instance* instance);