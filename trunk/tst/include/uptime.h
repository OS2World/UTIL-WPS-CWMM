#ifndef UPTIME_H_INCLUDED
#define UPTIME_H_INCLUDED


#if __cplusplus
extern "C" {
#endif

ULONG SysGetUptimeSecs(BOOL fUseTimer);
char* SysGetUptimeString(BOOL fUseTimer, char * chrBuffer, ULONG ulSize);
#if __cplusplus
}
#endif

#endif

