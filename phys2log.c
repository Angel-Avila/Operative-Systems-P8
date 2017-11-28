#include <stdio.h>
#include "vdisk.h"
#include "phys2log.h"

int getSecFis(int seclog) {
  return ((seclog + 1) % 27) + 1;
}

int getSuperficie(int seclog) {
  return ((seclog + 1) / 27) % 8;
}

int getCilindro(int seclog) {
  return (seclog + 1) / (27 * 8);
}

int vdreadseclog(int seclog, char *buffer) {
  int sf = getSecFis(seclog);
  int s = getSuperficie(seclog);
  int c  = getCilindro(seclog);
  return vdreadsector(0, s, c, sf, 1, buffer);
}

int vdwriteseclog(int seclog, char *buffer) {
  int sf = getSecFis(seclog);
  int s = getSuperficie(seclog);
  int c  = getCilindro(seclog);
  return vdwritesector(0, s, c, sf, 1, buffer);
}
