#include "neo.h"

short *neo_pointer(neo *n, short k) {
  unsigned short i;
  for(i = 0; i < n->ptr_l; i++) {
    if(n->ptr_k[i] == k) return &n->ptr_v[i];
  }
  n->ptr_k = realloc(n->ptr_k, sizeof(short)*(i+1));
  n->ptr_k[i] = k;
  n->ptr_v = realloc(n->ptr_v, sizeof(short)*(i+1));
  n->ptr_v[i] = 0;
  n->ptr_l++;
  return &n->ptr_v[i];
}

neo *neo_create() {
  neo *n = malloc(sizeof(neo));
  n->ptr_l = 0;
  n->prg_l = 0;
  n->prg_c = 0;
  n->ptr_k = NULL;
  n->ptr_v = NULL;
  n->prg = NULL;
  return n;
}

void neo_delete(neo *n) {
  free(n->ptr_k);
  free(n->ptr_v);
  free(n->prg);
  free(n);
}

void neo_new_program(neo *n, char *txt) {
  char c = -1, *str = NULL, jmp = 0;
  unsigned long i = 0, l = 0;
  free(n->prg);
  n->prg = NULL;
  n->prg_l = 0;
  while(c != '\0') {
    c = txt[i];
    if(c == '#') jmp = !jmp;
    else if(c == ';') {
      short i = 0;
      short datcom[4] = {0, 0, 0, 0};
      char *partxt[3] = {NULL, NULL, NULL};
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
        case '!': datcom[0] = O; break;
        case '?': datcom[0] = I; break;
        case '&': datcom[0] = JP; break;
        case '=': datcom[0] = EQ; break;
        case '~': datcom[0] = NE; break;
        case '>': datcom[0] = GT; break;
        case '<': datcom[0] = LT; break;
      }
      if(partxt[1]) {
        datcom[1] = (short) strtol(&partxt[1][partxt[1][0] == '&'], NULL, 10);
        if(partxt[1][0] == '&') datcom[3] = 1;
      }
      if(partxt[2]) {
        datcom[2] = (short) strtol(&partxt[2][partxt[2][0] == '&'], NULL, 10);
        if(partxt[2][0] == '&') {
          if(datcom[3] == 0) datcom[3] = 2;
          else if(datcom[3] == 1) datcom[3] = 3;
        }
      }
      n->prg = realloc(n->prg, sizeof(short)*(n->prg_l+4));
      n->prg[n->prg_l] = datcom[0];
      n->prg[n->prg_l+1] = datcom[1];
      n->prg[n->prg_l+2] = datcom[2];
      n->prg[n->prg_l+3] = datcom[3];
      n->prg_l += 4;
      free(str);
      str = NULL;
    }
    if(!jmp && !(c == '\n' || c == ' ' || c == '#' || c == ';')) {
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
  n->prg_c = 0;
  while(n->prg_c < n->prg_l) {
    short *dat[2] = {NULL, NULL};
    short *com = &n->prg[n->prg_c];
    switch(com[3]) {
      case 0: dat[0] = &com[1];                dat[1] = &com[2]; break;
      case 1: dat[0] = neo_pointer(n, com[1]); dat[1] = &com[2]; break;
      case 2: dat[0] = &com[1];                dat[1] = neo_pointer(n, com[2]); break;
      case 3: dat[0] = neo_pointer(n, com[1]); dat[1] = neo_pointer(n, com[2]); break;
    }
    switch(com[0]) {
      case DF: *dat[0] = *dat[1];           break;
      case AD: *dat[0] = *dat[0] + *dat[1]; break;
      case SB: *dat[0] = *dat[0] - *dat[1]; break;
      case ML: *dat[0] = *dat[0] * *dat[1]; break;
      case DV: *dat[0] = *dat[0] / *dat[1]; break;
      case O:  putchar(*dat[0]);            break;
      case I:  *dat[0] = getchar();         break;
      case JP: n->prg_c = *dat[0]*4-4;      break;
      case EQ: if(*dat[0] == 0) n->prg_c += *dat[1]*4-4; break;
      case NE: if(*dat[0] != 0) n->prg_c += *dat[1]*4-4; break;
      case GT: if(*dat[0] > 0)  n->prg_c += *dat[1]*4-4; break;
      case LT: if(*dat[0] < 0)  n->prg_c += *dat[1]*4-4; break;
    }
    n->prg_c += 4;
  }
}
