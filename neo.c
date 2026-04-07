#include "neo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

neo *neo_create() {
  neo *n = malloc(sizeof(neo));
  n->ptr = NULL;
  n->prg = NULL;
  n->prg_l = 0;
  n->prg_c = 0;
  return n;
}

void neo_delete(neo *n) {
  free(n->ptr);
  free(n->prg);
  free(n);
}

void neo_new_program(neo *n, char *txt) {
  char *str = NULL;
  char c = 1, s = 0;
  long i = 0, l = 0;
  free(n->prg); n->prg = NULL;
  while(c != '\0') {
    c = txt[i];
    if(c == '#') s = !s;
    else if(!s && c == '.') {
      signed char insdat[3] = { 0, 0, 0 };
      char *partxt[3] = { NULL, NULL, NULL };
      char *t = strtok(str, ",");
      int i = 0;
      while(t != NULL) {
        partxt[i] = t;
        t = strtok(NULL, ",");
        i++;
      }
      switch(partxt[0][0]) {
        case ':': insdat[0] = DF; break;
        case '+': insdat[0] = AD; break;
        case '-': insdat[0] = SB; break;
        case '*': insdat[0] = ML; break;
        case '/': insdat[0] = DV; break;
        case '@': insdat[0] = JP; break;
        case '=': insdat[0] = EQ; break;
        case '~': insdat[0] = NE; break;
        case '>': insdat[0] = GT; break;
        case '<': insdat[0] = LT; break;
        case '!': insdat[0] = O;  break;
        case '?': insdat[0] = I;  break;
      }
      insdat[0] = (insdat[0] & 0xF) << 4;
      if(partxt[1]) {
        if(partxt[1][0] == '$') {
          insdat[0] |= 1;
          insdat[1]      = (char) strtol(&partxt[1][1], NULL, 10);
        } else insdat[1] = (char) strtol(&partxt[1][0], NULL, 10);
      }
      if(partxt[2]) {
        if(partxt[2][0] == '$') {
          insdat[0] |= 2;
          insdat[2]      = (char) strtol(&partxt[2][1], NULL, 10);
        } else insdat[2] = (char) strtol(&partxt[2][0], NULL, 10);
      }
      if(n->prg == NULL) n->prg_l = 0;
      n->prg = realloc(n->prg, sizeof(char)*(n->prg_l+3));
      n->prg[n->prg_l]   = insdat[0];
      n->prg[n->prg_l+1] = insdat[1];
      n->prg[n->prg_l+2] = insdat[2];
      n->prg_l += 3;
      free(str); str = NULL;
    }
    if(!s && !(c == '\n' || c == '\t' || c == ' ' || c == '#' || c == '.')) {
      if(str == NULL) l = 0;
      str = realloc(str, l+2);
      str[l] = c;
      str[l+1] = '\0';
      l++;
    }
    i++;
  }
}

void neo_run_program(neo *n) {
  free(n->ptr); n->ptr = calloc(1, 0xFF);
  n->prg_c = 0;
  while(n->prg_c < n->prg_l) {
    signed char *ins    = &n->prg[n->prg_c];
    signed char *dat[2] = { NULL, NULL };
    switch(ins[0] & 0xF) {
      case 0: dat[0] = &ins[1];         dat[1] = &ins[2]; break;
      case 1: dat[0] = &n->ptr[ins[1]]; dat[1] = &ins[2]; break;
      case 2: dat[0] = &ins[1];         dat[1] = &n->ptr[ins[2]];  break;
      case 3: dat[0] = &n->ptr[ins[1]]; dat[1] = &n->ptr[ins[2]];  break;
    }
    switch((ins[0] >> 4) & 0xF) {
      case DF: *dat[0] = *dat[1];           break;
      case AD: *dat[0] = *dat[0] + *dat[1]; break;
      case SB: *dat[0] = *dat[0] - *dat[1]; break;
      case ML: *dat[0] = *dat[0] * *dat[1]; break;
      case DV: *dat[0] = *dat[0] / *dat[1]; break;
      case JP: n->prg_c = *dat[0]*3-3;      break;
      case EQ: if(*dat[0] == 0) n->prg_c += *dat[1]*3-3; break;
      case NE: if(*dat[0] != 0) n->prg_c += *dat[1]*3-3; break;
      case GT: if(*dat[0] > 0)  n->prg_c += *dat[1]*3-3; break;
      case LT: if(*dat[0] < 0)  n->prg_c += *dat[1]*3-3; break;
      case O: printf("%c", *dat[0]); break;
      case I: scanf(" %c", dat[0]);  break;
    }
    n->prg_c += 3;
  }
}