#include <os2.h>
#include <stdlib.h>

int zahlen[25]={0};
#define RANGE 20
int err=0;

int getRand()
{
  int zufall;
  DATETIME DT;
  INT  seed, start,  iCheck=0;
  BOOL bFound=FALSE;


  /** Get a random number in the appropriate range                    */  
  DosGetDateTime((PDATETIME)&DT);      /* Get the time               */
  seed = DT.hours*60 + DT.minutes;     /* convert to hundreths       */
  seed = seed*60 + DT.seconds;
  seed = seed*100 + DT.hundredths;
  srand((INT)seed);                    /* Set random number seed     */
  start=zufall;

  while(!bFound) {
    zufall=rand()%RANGE;    /* Range is 0...iNumToPlay-1  */
    iCheck++;

    /* Get a random number in the appropriate range */
    if(zahlen[zufall]==0) {
      zahlen[zufall]=1;
      return zufall+1;
    }

    /* Already played number. Find another */
    if(iCheck>1000) {
      err++;
      break; /* Emergency exit... */
    }
  };

  return -1;
}

int main()
{
  int zufall;
  int a;



  for(a=0;a<20;a++) {
    if(getRand()==-1)
      break;
    // zufall=rand()%20;
    /*  printf("%d\n", zufall);*/
    // zahlen[zufall]+=1;
  }
  for(a=0;a<25;a++) {
    printf("%d: %d\n", a, zahlen[a]);
  }
  printf("\nErr: %d\n", err);

#if 0
  for(a=0;a<1000000;a++) {
    zufall=rand()%20;
    /*  printf("%d\n", zufall);*/
    zahlen[zufall]+=1;
  }
  for(a=0;a<25;a++) {
    printf("%d: %d\n", a, zahlen[a]);
  }
#endif

  return 0;

}
