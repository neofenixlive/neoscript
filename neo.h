#ifndef __NEOSCRIPT
#define __NEOSCRIPT

enum neo_commands { DF, AD, SB, ML, DV, JP, EQ, NE, GT, LT, O, I };
typedef struct {
  unsigned short ptr_c, prg_l, prg_c;
  short *ptr_k, *ptr_v, *prg;
} neo;

short *neo_pointer(neo*, short);
neo *neo_create();
void neo_delete(neo*);
void neo_new_program(neo*, char*);
void neo_run_program(neo*);

#endif