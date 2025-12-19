#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Usine{

  char amont[32];
  int volume;

}Usine;

int compare_usine(const void *a, const void *b) {
    const Usine *ua = a;
    const Usine *ub = b;
    return ua->volume - ub->volume;
}


int main() {


  char buf[256];
  int n = 0;
  Usine usines[10000];

  while (fgets(buf, sizeof(buf), stdin)) {
    char * usineSrc = strtok( buf, ";" );
    char * amont = strtok( NULL, ";" );
    char * aval = strtok( NULL, ";" );
    char * volume = strtok( NULL, ";" );
    char * fuite = strtok( NULL, ";" );

    if (!amont || !volume) continue;  // sécurité

    strncpy(usines[n].amont, amont, 31);
    usines[n].amont[sizeof(usines[n].amont)-1] = '\0';
    usines[n].volume = atoi(volume);

    n++;


  }

  qsort(usines, n, sizeof(Usine), compare_usine);

  // bottom 50
  FILE *csv1 = fopen("bottom50.csv", "w");
  fprintf(csv1, "identifiant,capacite\n");
  for (int i = 0; i < 50 && i < n; i++) {
      fprintf(csv1, "%s,%d\n", usines[i].amont, usines[i].volume);
  }
  fclose(csv1);

  // top 10
  FILE *csv2 = fopen("top10.csv", "w");
  fprintf(csv2, "identifiant,capacite\n");
  for (int i = n - 10; i < n; i++) {
      if (i >= 0)
          fprintf(csv2, "%s,%d\n", usines[i].amont, usines[i].volume);
  }
  fclose(csv2);

  return 0;
}
