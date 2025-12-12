#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {


  char buf[256];
  FILE *csv = fopen("usines.csv", "w");

  fprintf(csv, "identifiant,capacite\n");

  while (fgets(buf, sizeof(buf), stdin)) {
  	// utiliser fscanfd avec regex
   	//printf("Usine : %s", buf);

    char * usineSrc = strtok( buf, ";" );
    char * amont = strtok( NULL, ";" );
    char * aval = strtok( NULL, ";" );
    char * volume = strtok( NULL, ";" );
    char * fuite = strtok( NULL, ";" );

    fprintf(csv, "%s,%s\n", amont, volume);

  }

  fclose(csv);

}
