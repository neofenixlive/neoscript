#ifndef __NEOSCRIPT
#define __NEOSCRIPT

#include <stdio.h>
#include <stdlib.h>

enum neo_command_enumerate {DF, AD, SB, ML, DV, O, I, JP, EQ, NE, GT, LT};
typedef struct {
  long ptr_l, prg_l, prg_c;
  short *ptr_k, *ptr_v, *prg;
} neo;

short *neo_pointer(neo*, short);
neo *neo_create();
void neo_delete(neo*);
void neo_new_program(neo*, char*);
void neo_run_program(neo*);

#endif
