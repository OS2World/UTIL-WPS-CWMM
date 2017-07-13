#include <os2.h>

#define HASHSIZE 101
ULONG calculateHash(char * theString)
{
  ULONG ulHash=0;

  if(theString)
    for(ulHash=0;*theString!='\0'; theString++)
      ulHash=*theString+31*ulHash;
  
  return ulHash%HASHSIZE;
}
//#define HASHSIZE2 123
ULONG calculateHash2(char * theString)
{
  ULONG ulHash=0;

  if(theString)
    for(ulHash=0;*theString!='\0'; theString++)
      ulHash=*theString+37*ulHash;
  
  return ulHash%HASHSIZE;
}

USHORT calculateTheHash(char *chrCommand)
{
  return ((calculateHash(chrCommand)<<8) + calculateHash2(chrCommand));
}
