#include <os2.h>
#include <stdio.h>
#include <stdlib.h>

APIRET APIENTRY DosReplaceModule(PSZ old, PSZ new, PSZ backup);

int main(int argc, char *argv[])
{
	APIRET rc;

	if(argc<2)
	{
		printf("%s - Ersetzt gesperrte DLLs \n\n", argv[0]);
		printf("Benutzung: %s altDLL neuDLL backup\n",argv[0]);
		printf("            altDLL - alte DLL, die ersetzt werden soll\n");
		printf("            neuDLL - neue DLL\n");
		printf("            backup - Name der Backupdatei\n\n");
		exit(1);
	}
    if(argc>2)
      rc=DosCopy(argv[1],argv[2],DCPY_EXISTING|DCPY_FAILEAS);

	if(rc==32 || argc==2)
      {
		rc=DosReplaceModule(argv[1], argv[2], argv[3]);
		if(rc)
          printf("DosReplaceModule: %u\n", rc);
      }
	else if (rc) printf("DosCopy: %u\n",rc);
    return 0;
}
