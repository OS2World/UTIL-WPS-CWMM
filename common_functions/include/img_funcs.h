#ifndef IMG_FUNCS_H_INCLUDED
#define IMG_FUNCS_H_INCLUDED

HBITMAP ImgLoadImageFile (  PSZ pszFileName );
BOOL ImgGetBmpInfoHeader(PBITMAPINFOHEADER2  bmpih2, PSZ pszFileName /*, char* procName, ULONG ulLength*/);

#endif
