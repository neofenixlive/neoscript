#include "neo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

short *neo_pointer(neo *n, short k) {
  unsigned short i;
  for(i = 0; i < n->ptr_c; i++) {
    if(n->ptr_k[i] == k) return &n->ptr_v[i];
  }
  n->ptr_k = realloc(n->ptr_k, sizeof(short)*(n->ptr_c+1));
  n->ptr_v = realloc(n->ptr_v, sizeof(short)*(n->ptr_c+1));
  n->ptr_k[i] = k; n->ptr_v[i] = 0; n->ptr_c++;
  return &n->ptr_v[i];
}

neo *neo_create() {
  neo *n = malloc(sizeof(neo));
  n->ptr_c = 0; n->prg_l = 0; n->prg_c = 0;
  n->ptr_k = NULL; n->ptr_v = NULL; n->prg = NULL;
  return n;
}

void neo_delete(neo *n) {
  free(n->ptr_k); free(n->ptr_v);
  free(n->prg); free(n);
}

void neo_new_program(neo *n, char *txt) {
  char *str = NULL;
  long str_l = 0;
  char c = 255, s = 0;
  long i = 0;
  free(n->prg); n->prg = NULL; n->prg_l = 0;
  while(c != '\0') {
    c = txt[i];
    if(c == '#') s = !s;
    else if(!s && c == ';') {
      short i = 0;
      short datcom[3] = { 0, 0, 0 };
      char *partxt[3] = { NULL, NULL, NULL };
      char *t = strtok(str, ",");
      while(t != NULL) {
        partxt[i] = t;
        t = strtok(NULL, ",");
        i++;
      }
      switch(partxt[0][0]) {
        case ':': datcom[0] = DF; break;
        case '+': datcom[0] = AD; break;
        case '-': datcom[0] = SB; break;
        case '*': datcom[0] = ML; break;
        case '/': datcom[0] = DV; break;
        case '@': datcom[0] = JP; break;
        case '=': datcom[0] = EQ; break;
        case '~': datcom[0] = NE; break;
        case '>': datcom[0] = GT; break;
        case '<': datcom[0] = LT; break;
        case '!': datcom[0] = O;  break;
        case '?': datcom[0] = I;  break;
      }
      datcom[0] = (datcom[0] & 0xFF) << 8;
      if(partxt[1]) {
        if(partxt[1][0] == '$') {
          datcom[0] |= 1;
          datcom[1]      = (short) strtol(&partxt[1][1], NULL, 10);
        } else datcom[1] = (short) strtol(&partxt[1][0], NULL, 10);
      }
      if(partxt[2]) {
        if(partxt[2][0] == '$') {
          datcom[0] |= 2;
          datcom[2]      = (short) strtol(&partxt[2][1], NULL, 10);
        } else datcom[2] = (short) strtol(&partxt[2][0], NULL, 10);
      }
      n->prg = realloc(n->prg, sizeof(short)*(n->prg_l+3));
      n->prg[n->prg_l]   = datcom[0];
      n->prg[n->prg_l+1] = datcom[1];
      n->prg[n->prg_l+2] = datcom[2];
      n->prg_l += 3;
      free(str); str = NULL; str_l = 0;
    }
    if(!s && !(c == '\n' || c == ' ' || c == '#' || c == ';')) {
      str = realloc(str, str_l+2);
      str[str_l] = c;
      str[str_l+1] = '\0';
      str_l++;
    }
    i++;
  }
}

void neo_run_program(neo *n) {
  n->prg_c = 0;
  while(n->prg_c < n->prg_l) {
    short *com    = &n->prg[n->prg_c];
    short *dat[2] = { NULL, NULL };
    switch(com[0] & 0xFF) {
      case 0: dat[0] = &com[1];                dat[1] = &com[2]; break;
      case 1: dat[0] = neo_pointer(n, com[1]); dat[1] = &com[2]; break;
      case 2: dat[0] = &com[1];                dat[1] = neo_pointer(n, com[2]); break;
      case 3: dat[0] = neo_pointer(n, com[1]); dat[1] = neo_pointer(n, com[2]); break;
    }
    switch((com[0] >> 8) & 0xFF) {
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
      case O: printf("%hd\n", *dat[0]); break;
      case I: scanf("%hd", dat[0]);     break;
    }
    n->prg_c += 3;
  }
}