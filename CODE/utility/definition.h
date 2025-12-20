typedef struct Facility{
  char id[81];
  int src;
  float real;
  int max;
} Facility;

typedef struct Instance{
  char id[81];
  int volume;
  float leaks;
  int income; //"src" command result only used for facilities
  float processed; //"real" command result only used for facilites
  struct Instance** downstream;
  int downstreamCount;
} Instance;
