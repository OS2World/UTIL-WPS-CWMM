/**********************************************

	Utility to patch the dll MMPARTS.DLL so the
	IBM MM classes are not longer found.

	(c) Chris Wohlgemuth 07.06.2003

	christopher.wohlgemuth@bch.siemens.de


	>>>> USE IT ON YOUR OWN RISK!!! <<<<

	(and remember to do a backup of the original
	file)


	This program is released under the GPL.

	Use IBM VAC++ V3 to compile: icc patch.dll
	*********************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <io.h>
#include <os2.h> 

#define NUMSTRINGS 90
/*
  ,"MM NewClass",
  "MM ClassData",
  "MM CClassData"
  */

char * strings[NUMSTRINGS]= {"MMDataFileNewClass",
                             "MMDataFileClassData",
                             "MMDataFileCClassData",

                             "MMAudioClassData",
                             "MMAudioCClassData",
                             "MMAudioNewClass",

                             "MMWAVClassData",
                             "MMWAVCClassData",
                             "MMWAVNewClass",
                             
                             "MMAIFClassData",
                             "MMAIFCClassData",
                             "MMAIFNewClass",
                             
                             "MMAUClassData",
                             "MMAUCClassData",
                             "MMAUNewClass",
                             
                             "MM_AUClassData",
                             "MM_AUCClassData",
                             "MM_AUNewClass",
                             
                             "MMIFFClassData",
                             "MMIFFCClassData",
                             "MMIFFNewClass",                             
                             
                             "MMSNDFClassData",
                             "MMSNDFCClassData",
                             "MMSNDFNewClass",                             
                             
                             "MMVOCClassData",
                             "MMVOCCClassData",
                             "MMVOCNewClass",
                             
                             "MMMIDIClassData",
                             "MMMIDICClassData",
                             "MMMIDINewClass",



                             "MMVideoClassData",
                             "MMVideoCClassData",
                             "MMVideoNewClass",

                             "MMAVIClassData",
                             "MMAVICClassData",
                             "MMAVINewClass",

                             "MMFLIClassData",
                             "MMFLICClassData",
                             "MMFLINewClass",

                             "MMMOVClassData",
                             "MMMOVCClassData",
                             "MMMOVNewClass",

                             "MMMPGClassData",
                             "MMMPGCClassData",
                             "MMMPGNewClass",

                             "MMImageClassData",
                             "MMImageCClassData",
                             "MMImageNewClass",
                             
                             "MMAVCClassData",
                             "MMAVCCClassData",
                             "MMAVCNewClass",

                             "MMDIBClassData",
                             "MMDIBCClassData",
                             "MMDIBNewClass",

                             "MMPhotoCDClassData",
                             "MMPhotoCDCClassData",
                             "MMPhotoCDNewClass",

                             "MMPCXClassData",
                             "MMPCXCClassData",
                             "MMPCXNewClass",



                             "MMTargaClassData",
                             "MMTargaCClassData",
                             "MMTargaNewClass",

                             "MMTIFFClassData",
                             "MMTIFFCClassData",
                             "MMTIFFNewClass",

                             "MMMMotionClassData",
                             "MMMMotionCClassData",
                             "MMMMotionNewClass",

                             "MMJPGNewClass",
                             "MMJPGClassData",
                             "MMJPGCClassData",

                             "MMGIFNewClass",
                             "MMGIFClassData",
                             "MMGIFCClassData",

                             "MMFolderClassData",
                             "MMFolderCClassData",
                             "MMFolderNewClass",

                             "MMCDDeviceClassData",
                             "MMCDDeviceCClassData",
                             "MMCDDeviceNewClass",

                             "MMLVDDeviceClassData",
                             "MMLVDDeviceCClassData",
                             "MMLVDDeviceNewClass",

                             "MMDeviceClassData",
                             "MMDeviceCClassData",
                             "MMDeviceNewClass",

                             "MMBitmapClassData",
                             "MMBitmapCClassData",
                             "MMBitmapNewClass"

};

 int main(int argc, char *argv[] )
 {
    FILE *stream;
		struct stat buf;
		char * memptr;
		char * tempPtr;
		BOOL found=FALSE;
		char * testString="DOSCALLS";
		char * testString2="doscalls";
        int a, b, c;		
		
		a=0;
if(argc!=2)
	{
	fprintf(stderr, "No DLL path given. Aborting...\n");
	exit(1);
	}
		printf("\nPatching the file MMPARTS.DLL \n\n");
		
		/* Open file */
    if (NULL == (stream = fopen(argv[1], "rb+"))) {
			printf("Could not open dll\n");
			return -1;
		}
		/* Get File size */
		if(fstat(fileno(stream),&buf)) {
			printf("Can't determine file size.   Exiting...\n");
			fclose(stream);
			return -1;
		}

		/* Get memory */
		memptr=malloc(	buf.st_size);
		
			if(!memptr) {
				printf("Can't get memory.   Exiting...\n");
				fclose(stream);
				return -1;
			}
			/* load file */	
			if(buf.st_size!=read(fileno(stream),memptr,buf.st_size))
				{
					printf("Can't load file into memory.   Exiting...\n");
					free(memptr);
					fclose(stream);
					return -1;
				}
            for(c=0;c<2;c++)
              {
                for(b=0;b<NUMSTRINGS;b++)
                  {			
                    found=FALSE;
                    tempPtr=memptr;
                    testString=strings[b];
                    /* Find string in file */
                    while(tempPtr<memptr+buf.st_size && !found) {
                      if(*tempPtr==*testString ) {
                        /* First valid char found. Check the rest */
                        found=TRUE;
                        for(a=0;a<strlen(strings[b]);a++) {
                          if(*(tempPtr+a)!=*(testString+a))
                            found=FALSE;
                        }
                      }
                      tempPtr++;
                    }
                    
                    if(found){
                      tempPtr--;
                      printf("%s found at file position: %X\n", strings[b], tempPtr-memptr);
                      printf("Changing it to XX...\n");
                      *((char*)(tempPtr))='X';
                      *((char*)(tempPtr+1))='X';
                    }
                    else {
                      printf("Can't find strings in file. So can't rename it.   Exiting...\n");
                      free(memptr);
                      fclose(stream);
                      return -1;
                    }
                  }/* b */
              }

			/* seek to beginning of file */
			if(lseek(fileno(stream),0,SEEK_SET)) {
				printf("Can't seek to beginning of file.   Exiting...\n");
				free(memptr);
				fclose(stream);
				return -1;
			}
			/* Write modified file */	
			if(buf.st_size!=write(fileno(stream),memptr,buf.st_size)) {
				printf("Error while writing file.\n");
				printf("WARNING!!   File MMPARTS.DLL may be corrupted!   Exiting...\n");
				free(memptr);
				fclose(stream);
				return -1;
			}
			
			free(memptr);
			fclose(stream);
            printf("All IBM mm classes renamed...\n\n");
			return 0;
 }
 




