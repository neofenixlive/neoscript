#include "neo.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  FILE *scr; neo *n;
  char *buf; unsigned long len;
  
  scr = fopen(argv[1], "rb");
  if (!scr) return 0;
  fseek(scr, 0, SEEK_END);
  len = ftell(scr);
  fseek(scr, 0, SEEK_SET);
  buf = malloc(len + 1);
  fread(buf, 1, len, scr);
  buf[len] = '\0';
  fclose(scr);
  
  n = neo_create();
  neo_new_program(n, buf);
  neo_run_program(n);
  neo_delete(n);
  
  free(buf);
  return 0;
}