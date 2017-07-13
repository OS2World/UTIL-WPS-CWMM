/* Program:   XUPTIME OS/2 1.1
   Platform:  OS/2
   Compilers: Borland C, EMX GNU C, IBM C-Set
   Compile:   $(CC) uptime.c
   Purpose:   Print the uptime of the machine based on the creation date
              of the swap file. In contrast too tools querying the uptime
              timer of OS/2, this can also cover uptimes > ~ 50 days.
   Author:    Tobias Ernst, tobi@bland.fido.de
   License:   Public Domain
*/

#define INCL_DOSFILEMGR
#define INCL_DOSMISC
#define INCL_DOSDATETIME
#define INCL_DOSERRORS
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* get_boot_drive: get the drive letter of this OS/2 system's boot drive */
#include <sys_funcs.h>

#define eatwhite(cp) { while (*(cp) == ' ' || *(cp) == '\n' || *(cp) == '\t') { (cp)++; } }

/* get_swap_file: get the name of the swap file of this OS/2 system by
                  parsing it's config.sys file.  */

#define buffersize 256+50
static BOOL get_swap_file(char *chrSwapFile, ULONG ulSize)
{
  char configsys[] = "X:\\CONFIG.SYS";
  char buffer[buffersize], *cp;
  FILE *f;
  int i;
  
  configsys[0] = SysQueryBootDriveLetter();//get_boot_drive();
  if ((f = fopen(configsys, "r")) == NULL)
    return FALSE;
    
  while ( (cp = fgets(buffer, buffersize, f)) != NULL)
    {
      eatwhite(cp);
      if (!strnicmp(cp, "SWAPPATH", 8))
        {
          cp += 8;
          eatwhite(cp);
          if (*cp == '=')
            {
              cp++;
              eatwhite(cp);
              for (i = 0;
                   cp[i] != ' ' && cp[i] != '\t'
                     && cp[i] != '\n' && cp[i] != '\0';
                   i++);
              strncpy(chrSwapFile, cp, i);
              if (chrSwapFile[i - 1] != '\\')
                {
                  chrSwapFile[i++] = '\\';
                }
              strcpy(chrSwapFile + i, "SWAPPER.DAT");
              fclose(f);
              return (TRUE);
            }
        }

      /* eat rest of line if it was > 256 chars */
      while (*buffer && buffer[strlen(buffer) - 1] != '\n')
        {
          if (fgets(buffer, buffersize, f) == NULL)
            {
              break;
            }
        }
    }
  //  fprintf (stderr, "%s does not contain a SWAPPATH statement.\n", configsys);
  fclose(f);
  return 0;
}


/* is_leap_year: tests if the specified year (4 digit) is a leap year */

static int is_leap_year(unsigned year)
{
  if (!((year % 100) % 4))
    {
      if (!(year % 100))
        {
          if (!(year % 400))
            {
              return 1;
            }
          else
            {
              return 0;
            }
        }
      else
        {
          return 1;
        }
    }
  else
    {
      return 0;
    }
}


/* n_days_in_month: returns the number of days in a certain month */

static int n_days_in_month(unsigned month, unsigned year)
{
  switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      return 31;
    case 4:
    case 6:
    case 9:
    case 11:
      return 30;
    case 2:
      return (is_leap_year(year)) ? 29 : 28;
    }
  return 0;
}



/* n_days_since_1900: returns the number of days that have passed since
                      1/1/1900 */

unsigned long n_days_since_1900(int dd, int md, int yd)
{
  int d=1,m,y;
  unsigned long ds=0;

  for (y = 1900; y<yd; y++)
  {
      ds += is_leap_year(y) ? 366 : 365;
  }
  for (m=1; m<md; m++)
  {
      ds += n_days_in_month(m, y);
  }
  ds += dd - d;
  return ds;
}


/* get_uptime: returns the age of the swap file in seconds */

unsigned long get_uptime_from_swapfile(void)
{
  static char chrSwapFile[CCHMAXPATH]={0};
  FILEFINDBUF3 InfoBuf;
  HDIR hDir = 0x0001;
  ULONG cSearch = 1;
  ULONG usAttrib;
  APIRET rc;
  DATETIME DateTime;
  unsigned long uptime;

  if(chrSwapFile[0]==0)
    get_swap_file(chrSwapFile, sizeof(chrSwapFile));

   
  rc = DosFindFirst((UCHAR *)chrSwapFile,
                    &hDir,
                    FILE_SYSTEM | FILE_HIDDEN | FILE_READONLY,
                    &InfoBuf,
                    sizeof InfoBuf,
                    &cSearch,
                    FIL_STANDARD);

  if (rc != NO_ERROR)
    {
      fprintf(stderr, "DosFindFirst returned %ld\n", rc);
      return(0);
    }

  DosFindClose(hDir);

  rc = DosGetDateTime(&DateTime);

  if (rc != 0)
    {
      fprintf(stderr, "DosGetDateTime returned %ld\n", rc);
      return(0);
    }

  uptime  = n_days_since_1900(DateTime.day, DateTime.month, DateTime.year);
  uptime -= n_days_since_1900(InfoBuf.fdateCreation.day,
                              InfoBuf.fdateCreation.month,
                              InfoBuf.fdateCreation.year + 1980);
  uptime *= (60L * 60L * 24L);

  uptime +=   (ULONG)DateTime.hours * 60L * 60L
    + (ULONG)DateTime.minutes * 60L
    + (ULONG)DateTime.seconds;
  uptime -=    (ULONG)InfoBuf.ftimeCreation.hours * 60L * 60L
    + (ULONG)InfoBuf.ftimeCreation.minutes * 60L
    + (ULONG)InfoBuf.ftimeCreation.twosecs * 2L;

#if 0
  if (verbose)
    {
      printf ("OS/2 swap file used:    %s\n", swapfile);
      printf ("Swapfile creation date: %02d/%02d/%04d, %02d:%02d\n",
              InfoBuf.fdateCreation.month, InfoBuf.fdateCreation.day,
              InfoBuf.fdateCreation.year + 1980,
              InfoBuf.ftimeCreation.hours, InfoBuf.ftimeCreation.minutes);
      printf ("Last modification:      %02d/%02d/%04d, %02d:%02d\n",
              InfoBuf.fdateLastWrite.month, InfoBuf.fdateLastWrite.day,
              InfoBuf.fdateLastWrite.year + 1980,
              InfoBuf.ftimeLastWrite.hours, InfoBuf.ftimeLastWrite.minutes);
      printf ("Last read access:       %02d/%02d/%04d, %02d:%02d\n",
              InfoBuf.fdateLastAccess.month, InfoBuf.fdateLastAccess.day,
              InfoBuf.fdateLastAccess.year + 1980,
              InfoBuf.ftimeLastAccess.hours, InfoBuf.ftimeLastAccess.minutes);
      printf ("Current date is:        %02d/%02d/%04d, %02d:%02d\n",
              DateTime.month, DateTime.day, DateTime.year,
              DateTime.hours, DateTime.minutes);
      printf ("-------------------------------------------------\n");
    }
#endif

  return uptime;
}

/* get_uptime_from_os2_timer: get the "uptime" from QSV_MS_COUNT */
unsigned long get_uptime_from_os2_timer(void)
{
  ULONG ms_count;
  ULONG DataBufLen = 4;

  if (DosQuerySysInfo(QSV_MS_COUNT, QSV_MS_COUNT, (UCHAR *)(&ms_count), DataBufLen) != NO_ERROR)
    return(0);

  return ms_count / (ULONG)1000; /* Return seconds */
}


void print_help(void)
{
    printf ("         -2   Query uptime from OS/2 system timer. This does only work if the\n");
    printf ("              uptime is < 50 days. If you do NOT specify this parameter, the\n");
    printf ("              uptime is calculated from the creation date of the swap file,\n");
    printf ("              which will also work for uptimes >= 50 days.\n");
}

/*!***********************************************************/
/*                                                           */
/* @@DESC                                                    */
/*                                                           */
/* To be written...                                          */
/*                                                           */
/*!!**********************************************************/
ULONG SysGetUptimeSecs(BOOL fUseTimer)
{
  if(fUseTimer)
    return get_uptime_from_os2_timer();
  else
    return get_uptime_from_swapfile();
}

/*!***********************************************************/
/*                                                           */
/* @@DESC                                                    */
/*                                                           */
/* To be written...                                          */
/*                                                           */
/*!!**********************************************************/
char* SysGetUptimeString(BOOL fUseTimer, char * chrBuffer, ULONG ulSize)
{
  unsigned long uptime, days, hours, minutes, seconds;
  char chrTemp[30];

  if(!ulSize)
    return NULLHANDLE;

  uptime=SysGetUptimeSecs(fUseTimer);

  days    = uptime / (60L * 60L * 24L);
  uptime  = uptime % (60L * 60L * 24L);
  hours   = uptime / (60L * 60L);
  uptime  = uptime % (60L * 60L);
  minutes = uptime / (60L);
  seconds = uptime % (60L);

  sprintf (chrTemp, "%ld %02ld:%02ld:%02ld",
           days, hours,
          minutes, seconds);
  strncpy(chrBuffer, chrTemp, ulSize);
  chrBuffer[ulSize-1]=0;
  return chrBuffer;
}

int main__(int argc, char **argv)
{
  int i, verbose = 0, use_os2_timer = 0;
  char *s;
  unsigned long uptime, days, hours, minutes, seconds;
    
  for (i = 1; i < argc; i++)
    {
      if (argv[i][0] == '-' || argv[i][0] == '/')
        {
          switch(argv[i][1])
            {
            case 'h':
            case '?':
              print_help();
              return(0);
            case 'v':
                verbose = 1;
                break;
            case '2':
                use_os2_timer = 1;
                break;
            }
        }
    }

  if (use_os2_timer)
    uptime = get_uptime_from_os2_timer();
  else
    uptime = get_uptime_from_swapfile();
  
  days    = uptime / (60L * 60L * 24L);
  uptime  = uptime % (60L * 60L * 24L);
  hours   = uptime / (60L * 60L);
  uptime  = uptime % (60L * 60L);
  minutes = uptime / (60L);
  seconds = uptime % (60L);
  
#if 0
  printf ("%s: uptime is %ld day%s, %02ld:%02ld hours and %02ld second%s\n",
          hostname, days, "s" + (days == 1), hours,
          minutes, seconds, "s" + (seconds == 1));
#endif

  return days;
}

