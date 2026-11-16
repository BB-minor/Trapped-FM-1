/* SPDX-License-Identifier: GPL-3.0-only */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define __attribute__(x)
#include "../firmware/host/preview.c"
/* (INFUR) every roll rate over 24 beats: hit count, and spacing measured in clock units
 * against the ideal u = BEAT_U * beats / hits (jitter can only be the scan's own step). */
int main(void){
  const char *nm[10]={"1/2","1/4","4T","1/8","8T","1/16","16T","1/32","32T","1/64"};
  int bad=0;
  uint32_t step = 441;                        /* 10 us of scan resolution */
  printf("  rate   hits  expect   u (ideal)   worst gap error\n");
  for(int rr=0; rr<10; rr++){
    song.g[G_ROLL]=rr; roll_key_rate=0xFF;
    uint32_t u = BEAT_U*ROLL_BEATS[rr]/ROLL_HITS[rr];
    uint32_t last=0xFFFFFFFFu, n=0; long prev=-1, worst=0;
    for(uint32_t b=0;b<24;b++)
      for(uint32_t pos=0; pos<BEAT_U; pos+=step){
        clk_beat=b; clk_pos=pos;
        uint32_t into,len,abs=grid_roll(0,&into,&len);
        if(abs!=last){
          last=abs; n++;
          long at=(long)b*BEAT_U+pos;
          if(prev>=0){ long e=(at-prev)-(long)u; if(e<0)e=-e; if(e>worst)worst=e; }
          prev=at;
        }
      }
    int exp = 24*ROLL_HITS[rr]/ROLL_BEATS[rr];
    int ok = ((int)n==exp||(int)n==exp+1) && worst <= (long)step;
    if(!ok) bad=1;
    printf("  %-6s %5u %7d %11u %10ld  %s\n", nm[rr], n, exp, u, worst, ok?"ok":"FAIL");
  }
  printf("\n%s\n", bad?"SOME RATES FAILED":"every rate exact to the scan's resolution");
  return bad; }
