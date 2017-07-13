#define INCL_OS2MM

#include <os2.h>
#include "os2me.h"

#if  __cplusplus
extern "C" {
#endif

/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This funktion returns the mediatype for the      */
/* given file.                                      */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* ULONG MMIO_MEDIATYPE_                            */
/*                                                  */
/* MMIO mediatype as identified by MMOS2.           */
/*                                                  */
/*!!*************************************************/
ULONG MMIdentifyFile(char* chrFile)
{
  MMIOINFO mmioInfo={0};
  MMFORMATINFO mmFormatInfo={0};
  FOURCC fourCC;

  if(MMIO_SUCCESS!=mmioIdentifyFile(chrFile, &mmioInfo, &mmFormatInfo, &fourCC, 0,0))
    {
      return MMIO_MEDIATYPE_UNKNOWN;
    }

  switch(mmFormatInfo.ulMediaType)
    {
    case MMIO_MEDIATYPE_MIDI:  /* Midi */
      return MMIO_MEDIATYPE_MIDI;
    case MMIO_MEDIATYPE_AUDIO: /* Any other audio file */
      return MMIO_MEDIATYPE_AUDIO;
    case MMIO_MEDIATYPE_IMAGE:
      return MMIO_MEDIATYPE_IMAGE;
    case MMIO_MEDIATYPE_DIGITALVIDEO:
      return MMIO_MEDIATYPE_DIGITALVIDEO;
    case MMIO_MEDIATYPE_MOVIE:
      return MMIO_MEDIATYPE_MOVIE;
    default:
      return MMIO_MEDIATYPE_UNKNOWN;
    }

  return 0;
}

#if  __cplusplus
}
#endif

