#include "monte.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  FILE *scr; monte *m;
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
  
  m = monte_create();
  monte_new_program(m, buf);
  monte_run_program(m);
  monte_delete(m);
  
  free(buf);
  return 0;
}
