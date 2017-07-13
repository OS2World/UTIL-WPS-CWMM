#ifndef SYS_H_INCLUDED
#define SYS_H_INCLUDED

/* For checking drives */
#define QDT_FIXED_DISK  0x01
#define QDT_REMOTE_DRIVE 0x02
#define QDT_DISKETTE_DRIVE 0x03
#define QDT_NO_DRIVE     0x04
#define QDT_CDROM_DRIVE   0x05
#define QDT_REMOVABLE_DISK 0
#define QDT_ERROR_DISK   0xF0

#if __cplusplus
extern "C" {
#endif

size_t
strlcpy(char *dst, const char *src, size_t siz); /* From OpenBSD */
size_t
strlcat(char *dst, const char *src, size_t siz); /* From OpenBSD */

char SysQueryBootDriveLetter(void);
ULONG SysQueryOSRelease(void);
void SysWriteToTrapLog(const char* chrFormat, ...);
BOOL SysCheckFileExists(char* chrFileName);
ULONG SysQueryCurrentTID(void);
ULONG SysQueryFileSize(char* chrFileName);
ULONG SysQueryDriveType(ULONG ulDriveNum);
ULONG SysQueryFreeDriveSpace(ULONG ulDriveNum, ULONG *ulTotal, ULONG * ulFree, ULONG* ulBytesUnit);
ULONG SysQueryDiskName(ULONG ulDriveNum, char *chrBuffer, ULONG ulSize);
BOOL SysQueryHomeDir(char* chrBuf, ULONG ulSize);

/* To be moved in clib */
char* my_strlcpy(char* string1, char* string2, size_t count);

#if __cplusplus
}
#endif

#endif

