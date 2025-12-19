# CY-Preinj2-ProjetS1
Projet de S1 pre-ing 2

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
