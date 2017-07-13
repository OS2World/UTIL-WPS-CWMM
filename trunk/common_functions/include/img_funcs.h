#ifndef IMG_FUNCS_H_INCLUDED
#define IMG_FUNCS_H_INCLUDED

HBITMAP ImgLoadImageFile (  PSZ pszFileName );
HBITMAP ImgLoadImageFileAndHeader (  PSZ pszFileName, PBITMAPINFOHEADER2 pBMPInfoHeader2);
BOOL ImgGetBmpInfoHeader(PSZ pszFileName, PBITMAPINFOHEADER2  bmpih2 /*, char* procName, ULONG ulLength*/);

#endif
