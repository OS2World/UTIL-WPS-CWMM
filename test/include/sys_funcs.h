#ifndef SYS_H_INCLUDED
#define SYS_H_INCLUDED

#ifdef __CPLUSPLUS__
extern "C" {
#endif

char SysQueryBootDriveLetter(void);
ULONG SysQueryOSRelease(void);
void SysWriteToTrapLog(const char* chrFormat, ...);
/* To be moved in clib */
char* strlcpy(char* string1, char* string2, size_t count);

#ifdef __CPLUSPLUS__
}
#endif

#endif

