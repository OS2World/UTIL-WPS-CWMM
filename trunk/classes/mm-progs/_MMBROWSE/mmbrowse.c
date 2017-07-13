/**************************************************************************
 *  File name  :  MMBrowse.c
 *
 *  Description:  This application serves as a template than can be use to demonstrate the
 *                    use of the MultiMedia I/O subsytem (MMIO).
 *
 *                This source file contains the following functions:
 *
 *                GetBitMap(HWND, PSZ)
 *                GetFOURCCDialogProc ( HWND, ULONG, MPARAM, MPARAM )
 *                FixSysMenu(HWND);
 *                AboutDlgProc(HWND, ULONG, MPARAM, MPARAM);
 *                DisplayImageFile ( HWND, PSZ );
 *                DisplayMessageBox ( HWND, USHORT, USHORT, USHORT );
 *                OpenFileDialog ( HWND, PSZ, PSZ, PSZ, PSZ );
 *                FourccToString( FOURCC, PSZ );
 *                MakeItBusy ( BOOL );
 *                Main ( int argc, char *argv[], char *envp[] );
 *                DrawBitMap ( HWND );
 *                CenterDialog ( HWND );
 *                FixSysMenu (HWND);
 *                LoadBitmap (HAB, HDC, HPS *, PSZ );
 *                InitializeHelp ( HWND);
 *                MyWindowProc(hwnd, msg, mp1, mp2)
 *                MessageBox(hwnd idMsg, fsStyle, fBeep)
 *                MainCommand(mp1, mp2)
 *
 *  Concepts   :  MMIO IOProcs
 *
 *  API's      :   mmioClose
 *                 mmioWrite
 *                 mmioRead
 *                 mmioGetHeader
 *                 mmioSetHeader
 *                 mmioQueryHeaderLength
 *                 mmioSendMessage
 *                 mmioOpen
 *                 mmioGetFormats
 *                 mmioQueryFormatCount
 *                 mmioInstallIOProc
 *                 mmioStringToFOURCC
 *                 mmioIdentifyFile
 *                 mmioIniFileHandler
 *                 mmioFOURCC
 *
 *    Files    :  MMBrowse.c  MMBrowse.h  MMBrowse.rc MMbrowse.ipf
 *
 *  Copyright (C) 1991 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    *
 *************************************************************************/
#define INCL_DOSERRORS
#define INCL_DOSMODULEMGR
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#define INCL_DOSMEMMGR
#define INCL_GPI
#define INCL_GPICONTROL
#define INCL_GPILCIDS
#define INCL_GPIMETAFILES
#define INCL_GPIPRIMITIVES
#define INCL_GPIREGIONS
#define INCL_PM
#define INCL_WIN
#define INCL_WINDIALOGS
#define INCL_WINERRORS
#define INCL_WINFRAMEMGR
#define INCL_WINHELP
#define INCL_WINMENUS
#define INCL_WINMESSAGEMGR
#define INCL_WINMLE
#define INCL_WINPOINTERS
#define INCL_WINSTDDLGS
#define INCL_WINSTDFILE
#define INCL_WINSTDSLIDER
#define INCL_WINSWITCHLIST
#define INCL_WINSYS
#define INCL_WINTIMER
#define INCL_WINWINDOWMGR
#define INCL_REXXSAA

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <rexxsaa.h>                   /* needed for Rexx */
#include "mmbrowse.h"
#include "framecontrol.h"
#include "mmioos2.h"

extern PFNWP g_pfnwpOrgFrameProc;
extern FRAMECTRLDATA fcd;
char g_binDir[CCHMAXPATH]={0};
ULONG ulHeight=40; /* Height of top frame control */

MMIMAGEHEADER mmImgHdr; /* Header of loaded image */

#if 0
char chrMMImageKnownExt[320]={0};/* Array holding the extensions we have a class for e.g. JPG. This
                                    array will be filled using an external REXX script. Space for
                                    80 ext should be enough for now (4 bytes per ext e.g.: "JPG "<- note the space) */
char chrMMImageExt[200]={0};/* Array holding the extensions for wpclsQueryInstanceFilter(). Will be
                               filled during processing of wpclsInitData() */
#endif

char pszImageFile[CCHMAXPATH]={0};
HBITMAP hbmBitmap=NULLHANDLE;

MRESULT EXPENTRY fnwpFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

BOOL getStringFromRexxScript(PSZ rexxFile, char* chrResult, ULONG ulSize)
{

  RXSTRING arg[1];                       /* argument string for REXX  */
  RXSTRING rexxretval;                /* return value from REXX    */
  APIRET   rc;                        /* return code from REXX     */
  SHORT    rexxrc = 0;                /* return code from function */
  char theScript[CCHMAXPATH];  
  /* By setting the strlength of the output RXSTRING to zero, we   */
  /* force the interpreter to allocate memory and return it to us. */
  /* We could provide a buffer for the interpreter to use instead. */
  rexxretval.strlength = 0L;          /* initialize return to empty*/
  
  
  sprintf(theScript, "%s\\%s", g_binDir, rexxFile);

  /*TRY_LOUD(RX_START) {*/
    /* Here we call the interpreter.  We don't really need to use    */
    /* all the casts in this call; they just help illustrate         */
    /* the data types used.                                          */
    rc=RexxStart((LONG)       0,             /* number of arguments   */
                 (PRXSTRING)  &arg,          /* array of arguments    */
                 (PSZ)        theScript,/* name of REXX file     */
                 (PRXSTRING)  0,             /* No INSTORE used       */
                 (PSZ)        "CWRXX",         /* Command env. name     */
                 (LONG)       RXSUBROUTINE,  /* Code for how invoked  */
                 (PRXSYSEXIT) 0,             /* No EXITs on this call */
                 (PSHORT)     &rexxrc,       /* Rexx program output   */
                 (PRXSTRING)  &rexxretval ); /* Rexx program output   */


#if 0
    if(rc) {
      sprintf(text,"Error in the Rexx skript %s\n\n Get more information with 'help REX%04d'.\n", 
              tPt->rexxSkript, rc*-1);
      WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "", 1234, MB_OK|MB_MOVEABLE|MB_ERROR);
    }
#endif
    if(!rc) {
      if(ulSize>rexxretval.strlength) {
        strncpy(chrResult, rexxretval.strptr, rexxretval.strlength);
        chrResult[rexxretval.strlength]=0;
      }
      else
        strncpy(chrResult, rexxretval.strptr, ulSize);
      chrResult[ulSize-1]=0;
    }
    if(rexxretval.strptr)
      DosFreeMem(rexxretval.strptr);          /* Release storage given to us by REXX.  */      
    /*  }
  CATCH(RX_START)
    {}END_CATCH;*/
    return TRUE;
}


/****************************************************************************/
/*                                                                          */
/* This procedure handles the frame extension of the frame at the top       */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
MRESULT EXPENTRY topCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
    
  switch(msg)
    {
      /*****************************************************/
      /* Init the dialog                                   */
      /*****************************************************/
    case WM_INITDLG :
      {
        ULONG ulStyle;
        SWP swp;
        
        WinSetWindowULong(hwnd,QWL_USER,(ULONG)PVOIDFROMMP(mp2));//Save object ptr.
        WinQueryWindowPos(WinQueryWindow(hwnd,QW_PARENT),&swp);
#if 0
        /* Set dialog font to WarpSans for Warp 4 and above */
        if(cwQueryOSRelease()>=40) {
          WinSetPresParam(hwnd,
                          PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                          DEFAULT_DIALOG_FONT );
        }
#endif
        return (MRESULT)FALSE;
      }
#if 0
    case WM_CHAR:
      // Set focus to container so the key will be processed as usual
      WinSetFocus(HWND_DESKTOP, WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_CLIENT));
      return WinSendMsg(WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_CLIENT),msg,mp1,mp2);

      /*****************************************************/
      /* Paint the background.                             */
      /*****************************************************/
    case WM_PAINT:
      {
        HPS hps;
        RECTL rcl, rclSave, rclSource;
        SWP swp;
        
        hps=WinBeginPaint(hwnd, NULLHANDLE,NULL);
        /* Get Window size */
        WinQueryWindowRect(hwnd, &rcl);
        rclSave=rcl;
        /* Get size and pos of control area */
        WinQueryWindowPos(WinWindowFromID(hwnd, IDDLG_TOPMIDDLE),&swp);
        /* Left area */
        rcl.xRight=swp.x;
        rclSource.xLeft=0;
        rclSource.yBottom=0;
        rclSource.yTop=allBMPs[CTRLIDX_TOPLEFT].bmpInfoHdr.cy;
        rclSource.xRight=allBMPs[CTRLIDX_TOPLEFT].bmpInfoHdr.cx;
        WinDrawBitmap(hps, allBMPs[CTRLIDX_TOPLEFT].hbm,
                      &rclSource, 
                      (PPOINTL)&rclSave,
                      0, 0,
                      DBM_IMAGEATTRS | DBM_STRETCH);
        
        /* Right area */
        rclSave.xLeft=swp.x+swp.cx;
        rclSource.xLeft=0;
        rclSource.yBottom=0;
        rclSource.yTop=allBMPs[CTRLIDX_TOPRIGHT].bmpInfoHdr.cy;
        rclSource.xRight=allBMPs[CTRLIDX_TOPRIGHT].bmpInfoHdr.cx;
        WinDrawBitmap(hps, allBMPs[CTRLIDX_TOPRIGHT].hbm,
                      &rclSource, 
                      (PPOINTL)&rclSave,
                      0, 0,
                      DBM_IMAGEATTRS | DBM_STRETCH);
        
        WinEndPaint(hps);
        return (MRESULT) 0;
      }
#endif 
    default:
      break;
    }
  return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


VOID main ( int argc, char *argv[], char *envp[] )
{
    HMQ   hmq;                             /* Message queue handle         */
    HWND  hwndClient;                      /* Client area window handle    */
    HWND  hwndFrame;                       /* Frame window handle          */
    QMSG  qmsg;                            /* Message from message queue   */
    ULONG flCreate;                        /* Window creation control flags*/
    BOOL  bReturnCode;
    SWCNTRL swcntrl;                       /* struct with Task Mgr info */
    FRAMECTRLDATA *pfcd;
    char * chrPtr;

    FRAMECDATA framecd={0};

    strcpy(g_binDir, argv[0]);
    if((chrPtr=strrchr(g_binDir, '\\'))!=NULLHANDLE)
       *chrPtr=0;

    if(argc >1)
      strcpy(pszImageFile, argv[1]);

    hab = WinInitialize( 0 );          /* Initialize PM                */

    hmq = WinCreateMsgQueue( hab, 0 );    /* Create a message queue       */

    bReturnCode = WinRegisterClass( hab,
                                    "MMBrowser",
                                    (PFNWP) MyWindowProc,
                                    CS_SIZEREDRAW,
                                    0 );

    flCreate = (FCF_HORZSCROLL| FCF_VERTSCROLL| FCF_STANDARD) & ~FCF_SHELLPOSITION;

    hwndFrame = WinCreateStdWindow ( HWND_DESKTOP,
                                     0L,
                                     &flCreate,
                                     "MMBrowser",
                                     "",
                                     0L,
                                     0,
                                     ID_WINDOW,
                                     &hwndClient );

    g_pfnwpOrgFrameProc=WinSubclassWindow(hwndFrame, fnwpFrameProc);

    pfcd=&fcd;
    pfcd->hwndCtlTop=WinLoadDlg(hwndFrame, hwndFrame, topCtrlDialogProc, hResource, IDDLG_TOPFRAME ,0);
    
    if(pfcd->hwndCtlTop)
      {
        RECTL rectl;
        
        /* Query the size of the dialog */
        WinQueryWindowRect(pfcd->hwndCtlTop,&rectl);
        
        pfcd->bTop=TRUE;
        pfcd->sizelTopFrame.cx=100;
        //              pfcd->sizelTopFrame.cy=rectl.yTop;
        pfcd->sizelTopFrame.cy=ulHeight;
        pfcd->ulFlagsTop=0;
        //        WinSendMsg(hwndFolder,WM_UPDATEFRAME,0,0);
      }
#if 0
    /* Scrollbars */
    framecd.cb=sizeof(FRAMECDATA);
    framecd.flCreateFlags=FCF_HORZSCROLL;
    WinCreateFrameControls(hwndFrame, &framecd, NULLHANDLE);
#endif

    bReturnCode = WinSetWindowPos( hwndFrame,
                                   HWND_TOP,
                                   100, 100, 400, 280,
                                   SWP_SIZE | SWP_MOVE | SWP_ACTIVATE | SWP_SHOW );

    memset(&swcntrl, 0, sizeof(SWCNTRL));                  /* init structure */

    swcntrl.hwnd = hwndFrame;
    strcpy(swcntrl.szSwtitle, "MMBrowser" );

    WinAddSwitchEntry((PSWCNTRL)&swcntrl);            /* add app to Task Mgr */
    InitializeHelp (hwndFrame );

    /*
     *  Now, dispatch all messages...
     */
    while( WinGetMsg( hab, &qmsg, 0, 0, 0 ) )
    {
         WinDispatchMsg( hab, &qmsg );
    }

    WinDestroyHelpInstance(hwndHelpInstance);
    WinDestroyWindow( hwndFrame );
    WinDestroyMsgQueue( hmq );
    WinTerminate( hab );
}

/*********************************************************/
/*                                                       */
/*********************************************************/
VOID MakeItBusy ( BOOL BusyStatus )
{
    if( BusyStatus == TRUE )
    {
         WinSetPointer ( HWND_DESKTOP,
                         WinQuerySysPointer ( HWND_DESKTOP,
                                              SPTR_WAIT,
                                              FALSE));
    }
    else
    {
         WinSetPointer ( HWND_DESKTOP,
                         WinQuerySysPointer ( HWND_DESKTOP,
                                              SPTR_ARROW,
                                              FALSE));
    }
}



/*********************************************************/
/*                                                       */
/*********************************************************/
MRESULT EXPENTRY MyWindowProc ( HWND hwnd,
                                USHORT msg,
                                MPARAM mp1,
                                MPARAM mp2 )
{
  USHORT command;
  BOOL   bReturnCode;
  ULONG  ReturnCode;
  PMMINIFILEINFO  pmmIniFileInfo;
  FOURCC IOProcFourCC;
  FOURCC IOProcFourSource;
  FOURCC IOProcFourTarget;
  ULONG  ulIndex = 0;
  CHAR   szTempBuffer1[STRING_LENGTH];
  CHAR   szTempBuffer2[STRING_LENGTH];
  FOURCC fccIOProc;
  FOURCC fccSourceIOProc;
  MMFORMATINFO mmFormatInfoSource;
  MMFORMATINFO mmFormatInfoTarget;

  switch( msg )
    {
#if 0
    case DM_DRAGOVER:
      {
        PDRAGINFO pdi=PVOIDFROMMP(mp1);

        if(DrgAccessDraginfo(pdi))
          {
            USHORT usOp;
            USHORT usItems=DrgQueryDragitemCount(pdi);
            int i;
            PDRAGITEM dragItem;

            for(i=0;i<usItems; i++)
              {
                char text[300];
                dragItem=DrgQueryDragitemPtr(pdi, i);
               DrgQueryStrName(dragItem->hstrType,sizeof(text), text); 
               WinSetWindowText(WinQueryWindow(hwnd, QW_PARENT), text);
                if(!DrgVerifyRMF(dragItem, "DRM_OS2FILE", NULLHANDLE)) {
                  //DosBeep(5000, 100);
                  break;
                }
              }


#if 0
            switch(pdi->usOperation)
              {

              }
#endif
            //DosBeep(500, 100);
            DrgFreeDraginfo(pdi);
          }
        return MRFROM2SHORT(DOR_NEVERDROP, DO_COPY);
      }
#endif
    case WM_CREATE:

      //      _getMMImageExt();
      //      WinSetWindowText(WinQueryWindow(hwnd, QW_PARENT), chrMMImageKnownExt);

      /*
       * Get the open dialog title from resource file.
       */
      WinLoadString( hab,
                     0,
                     IDS_OPEN_DIALOG_TITLE,
                     STRING_LENGTH,
                     szOpenDialogTitle ) ;
      
      /*
       * Get the open dialog file extension from resource file.
       */
      WinLoadString( hab,
                     0,
                     IDS_OPEN_DIALOG_FILE_EXT,
                     STRING_LENGTH,
                     szOpenDialogFileExt );

      /*
       * Get the open ok button from resource file.
       */
      WinLoadString( hab,
                     0,
                     IDS_OPEN_OK_BUTTON,
                     STRING_LENGTH,
                     szOpenOKButton );

      /*
       * Get the install ok button from resource file.
       */
      WinLoadString( hab,
                     0,
                     IDS_INSTALL_OK_BUTTON,
                     STRING_LENGTH,
                     szInstallOKButton );

      /*
       * Get the install dialog box title from resource file.
       */
      WinLoadString( hab,
                     0,
                     IDS_INSTALL_TITLE,
                     STRING_LENGTH,
                     szInstallTitle );

      /*
       * Get the install dialog box file extension from resource file.
       */
      WinLoadString( hab,
                     0,
                     IDS_INSTALL_FILE_EXT,
                     STRING_LENGTH,
                     szInstallFileExt );

      /*
       * Copy the IO Proc standard name from resource file.
       */
      WinLoadString( hab,
                     0,
                     IDS_IOPROC_NAME,
                     STRING_LENGTH,
                     szIOProcName );

      hbmBitmap = GetBitMap ( hwnd, pszImageFile );

      break;  // end of WM_CREATE

    case WM_HELP :
      WinSendMsg ( hwndHelpInstance,
                   HM_DISPLAY_HELP,
                   MPFROMLONG ( 1 ),
                   MPFROMSHORT ( HM_RESOURCEID ) );

      return( 0 );
      break;

    case WM_ERASEBACKGROUND:
      return ( MRFROMLONG ( TRUE ) );
      break;

    case WM_COMMAND:
      command = SHORT1FROMMP(mp1);      /* Extract the command value    */
      switch (command)
        {
          /*
           * Open a file (read and view)
           */
        case ID_FILE_OPEN:
          /*
           * Get filename from user to open
           */
          if( pszImageFile == NULL )
            {
              break;
            }
          if( OpenFileDialog ( hwnd,   // handle of owner window
                               szOpenDialogTitle,        // title of open dialog box
                               "*.*",                    // file extension
                               szOpenOKButton,
                               pszImageFile ) == FALSE )  // return filename here.
            {
              break;
              // error.
            }

          hbmBitmap = (BOOL) GetBitMap ( hwnd, pszImageFile );

          WinInvalidateRegion( hwnd, 0, FALSE );
          break;
        case ID_FILE_SAVEAS:
          if( mmioIdentifyFile(pszImageFile,
                               0L,
                               &mmFormatInfoSource,
                               &IOProcFourSource,
                               0L,
                               0L))
            {
              WinMessageBox( HWND_DESKTOP,
                             HWND_DESKTOP,
                             "Please Open an Image File",
                             "Save as .....",
                             (HMODULE) NULL,
                             (ULONG) MB_OK | MB_MOVEABLE |
                             MB_ERROR );
              break;
            }
          fccSourceIOProc = mmFormatInfoSource.fccIOProc;
          SaveFile( hwnd, pszImageFile, fccSourceIOProc);
          WinInvalidateRegion( hwnd, 0, FALSE );
          break;

        case IDM_HELPUSINGHELP:
          if ( WinSendMsg ( hwndHelpInstance,
                            HM_DISPLAY_HELP,
                            MPVOID,
                            MPVOID ) )
            {
              WinMessageBox( HWND_DESKTOP,
                             hwnd,
                             "Cannot display Help For Help.",
                             "Help For Help",
                             (HMODULE) NULL,
                             MB_OK | MB_ERROR );
            }
          WinInvalidateRegion( hwnd, 0, FALSE );
          break;

        case IDM_HELPGENERAL:
          if ( WinSendMsg ( hwndHelpInstance,
                            HM_DISPLAY_HELP,
                            MPFROM2SHORT(PANEL_HELPGENERAL,NULL), MPFROMSHORT(HM_RESOURCEID)))
            {
              WinInvalidateRegion( hwnd, 0, FALSE );
              break;
            }
          break;
        case IDM_HELPINDEX:
          if (WinSendMsg ( hwndHelpInstance,
                           HM_HELP_INDEX,
                           MPVOID,
                           MPVOID))
            {
              WinMessageBox( HWND_DESKTOP,
                             hwnd,
                             "Cannot display Help Index.",
                             "Help Index",
                             (HMODULE) NULL,
                             MB_OK | MB_ERROR );
            }
          WinInvalidateRegion( hwnd, 0, FALSE );
          break;

        case IDM_HELPPRODUCTINFO:

          WinDlgBox(HWND_DESKTOP,
                    hwnd,
                    (PFNWP)AboutDlgProc,
                    0,
                    IDD_PRODUCTINFO,
                    (PVOID)NULL);

          WinInvalidateRegion( hwnd, 0, FALSE );
          break;

          /*
           * Exit from program.
           */

        case ID_EXITPROG:
          WinPostMsg( hwnd, WM_CLOSE, 0L, 0L );
          break;

        default:
          return WinDefWindowProc( hwnd, msg, mp1, mp2 );
        }

      break; // end of WM_COMMAND


    case WM_PAINT:

      if (!WinIsWindowVisible(hwnd))
        {
          return((MRESULT)NULL);
        }

      /*
       * Check our Presentation Space. If a bitmap exists,
       * then display it. Otherwise, do our normal stuff.
       */

      if (hpsMemory)
        {
          MakeItBusy ( TRUE );
          DrawBitMap(hwnd);
          MakeItBusy ( FALSE );
        }

      WinDefWindowProc ( hwnd, msg, mp1, mp2 );
      break;
      return((MRESULT)TRUE);

    case WM_CLOSE:
      WinPostMsg( hwnd, WM_QUIT, 0L, 0L );  /* Cause termination */
      break;
      /*
       *  Everything else comes here.
       */

    default:
      return WinDefWindowProc( hwnd, msg, mp1, mp2 );
    }

  return FALSE;
}




static char* _getExtFromImageIOProc(BOOL bWrite)
{
    CHAR          szBuffer[ sizeof( FOURCC ) + CCHMAXPATH + 4 ];
    MMFORMATINFO  mmFormatInfo;
    PMMFORMATINFO pmmFormatInfoArray;
    void * memPtr;
    ULONG         ulReturnCode;
    LONG          lFormatsRead;
    LONG          index;
    LONG          lBytesRead;
    HWND hwndSubMenu;
    MENUITEM mi;
    LONG lNumIOProcs;
    char * chrMMImageKnownExt=NULLHANDLE;

    memset( &mmFormatInfo,
            '\0',
            sizeof(MMFORMATINFO) );
    
    mmFormatInfo.ulMediaType |= MMIO_MEDIATYPE_IMAGE;
    if(!bWrite)
      mmFormatInfo.ulFlags|=MMIO_CANREADTRANSLATED;/* Read !!!*/    
    else
      mmFormatInfo.ulFlags|=MMIO_CANWRITETRANSLATED;/* Read !!!*/    
    ulReturnCode = mmioQueryFormatCount ( &mmFormatInfo,
                                          &lNumIOProcs,
                                          0,
                                          0 );
    
    if( ulReturnCode != MMIO_SUCCESS )
      {
        /*
         * Error - mmioQueryFormatCount failed.
         */
        return NULLHANDLE;
      }

    /*
     * Allocate enough memory for n number of FormatInfo blocks
     */
    pmmFormatInfoArray = malloc (lNumIOProcs * sizeof( MMFORMATINFO ) );
    memPtr=pmmFormatInfoArray;
    if( pmmFormatInfoArray == NULL )
      {
        /*
         * Could not allocate enough memory for mmFormatInfo array.
         */
        return NULLHANDLE;
      }
    
    /*
     * call mmioGetFormats to get info on the formats supported.
     */
    ulReturnCode = mmioGetFormats( &mmFormatInfo,
                                   lNumIOProcs,
                                   pmmFormatInfoArray,
                                   &lFormatsRead,
                                   0,
                                   0 );
    if( ulReturnCode != MMIO_SUCCESS )
      {
        /*
         *  mmioGetFormats failed.
         */
        free(pmmFormatInfoArray);
        return NULLHANDLE;
      }
    
    if( lFormatsRead != lNumIOProcs )
      {
        /*
         * Error in MMIO - number of formats read in by
         * mmioGetFormats is not equal to number of formats
         * found by mmioQueryFormatCount.
         */
        free(pmmFormatInfoArray);
        return NULLHANDLE;
      }

#define EXT_MEM_SIZE 10000

    /* Get extension string. I'm lazy here and just allocate a huge array. */
    if((chrMMImageKnownExt=malloc(EXT_MEM_SIZE))==NULLHANDLE)
      return NULLHANDLE;

    memset(chrMMImageKnownExt, 0, EXT_MEM_SIZE);


    for ( index = 0; index <lNumIOProcs; index++ )
      {
        /* Now scan the whole image proc array */
        if(chrMMImageKnownExt[0]==0)
          strcpy(chrMMImageKnownExt, "*.");/* First extension */
        else
          strcat(chrMMImageKnownExt, ";*.");
        
        strcat(chrMMImageKnownExt, pmmFormatInfoArray->szDefaultFormatExt);
        
        //          HlpWriteToTrapLog("Known Extension: %s\n", chrMMImageKnownExt);
        //          HlpWriteToTrapLog("Extension: %s\n", chrMMImageExt);
        
        /*
         *  advance to next entry in mmFormatInfo array
         */        
        pmmFormatInfoArray++;
      }

    free(memPtr);
    
    return chrMMImageKnownExt;
}





/**********************************************************************/
/*                                                                    */
/*  Name:   OpenFileDialog                                            */
/*                                                                    */
/*  Purpose: open the standard file open dialog as file extention     */
/*           and return the filename                                  */
/*                                                                    */
/*  Usage:   called when the user needs to supply a name for          */
/*           the file to be opened                                    */
/*                                                                    */
/*  Method:  calls the standard file open dialog to get the           */
/*           file name.                                               */
/*                                                                    */
/*  Parameters: HWD hwnd         Handle of the owner window.          */
/*              PSZ szTitle      Title of open dialog.                */
/*              PSZ pszFileExt   File extention. (for example : *.txt)*/
/*              PSZ pszFullPath  PSZ for returning the file name.     */
/*                                                                    */
/*  Returns: TRUE if successful in getting a file name, FALSE         */
/*              if not in pushing CANCEL                              */
/*           PSZ pszFullPath pointer to filename (full path)          */
/*                                                                    */
/**********************************************************************/

BOOL OpenFileDialog(HWND hwndOwner,
                    PSZ szTitle,
                    PSZ szFileExt,
                    PSZ szButton,
                    PSZ szFullPath )
{
    FILEDLG fdg;
    char *chrExt=NULLHANDLE;

    memset(&fdg, 0, sizeof(FILEDLG));
    fdg.cbSize = sizeof(FILEDLG);          /* Size of FILEDLG.                */
    fdg.pszTitle = szTitle;                /* String to display in title bar. */
    fdg.pszOKButton = szButton ;
    fdg.ulUser = 0L;                       /* User defined field.             */
    fdg.fl = FDS_CENTER |
             FDS_OPEN_DIALOG;
    fdg.pfnDlgProc = NULL;
    fdg.lReturn = 0L;                      /* Result code from dialog dismissal. */
    fdg.lSRC = 0L;                         /* System return code.          */
    fdg.hMod = 0;                          /* Custom file dialog template. */
    fdg.usDlgId = 0;                       /* Custom file dialog ID.       */
    fdg.x = 100;                           /* X coordinate of the dialog.  */
    fdg.y = 100;                           /* Y coordinate of the dialog.  */

    chrExt=_getExtFromImageIOProc(FALSE);       /* Get image extensions         */

    strcpy( fdg.szFullFile, chrExt);  
    /* set selected fully qualified path */
    //    strcpy( fdg.szFullFile, szFileExt);

    /* get the file */
    if ( !WinFileDlg ( HWND_DESKTOP,
                       hwndOwner,
                       (PFILEDLG)&fdg ) ) {
      if(chrExt)
        free(chrExt);
       return FALSE;
    }

    if(chrExt)
      free(chrExt);
    
    /* copy file name into file name buffer */
    strcpy ( szFullPath, fdg.szFullFile );
    if (fdg.lReturn == DID_CANCEL)
      return FALSE;
    else
      return(TRUE);
}




//PSZ pszTypes[]= {"Bitmap",0};

VOID SaveFile (HWND hwndClient, PSZ pszSource, FOURCC fccSourceIOProc )
{
  FILEDLG fdg;
  HWND hwndDlg;
  INT    Result;
  PSZ    Result1;
  char *chrExt=NULLHANDLE;

  CHAR Test[STRING_LENGTH];
  CHAR VID[]=".VID";
  CHAR DIB[]=".DIB";
  CHAR PCX[]=".PCX";
  CHAR TGA[]=".TGA";
  CHAR TIF[]=".TIF";
  CHAR RDI[]=".RDI";
  CHAR BMP[]=".BMP";
  FOURCC fccTargetIOProc;

  chrExt=_getExtFromImageIOProc(TRUE);       /* Get image extensions         */

  memset(&fdg, 0, sizeof(FILEDLG));
  fdg.cbSize = sizeof(FILEDLG);
  fdg.fl = FDS_SAVEAS_DIALOG | FDS_CENTER;
  fdg.pszTitle = "The Save File Dialog";
  //fdg.papszITypeList=(PAPSZ)pszTypes;
  /*  strcpy(fdg.szFullFile, "*.bmp");*/
  strcpy( fdg.szFullFile, chrExt);

  hwndDlg= WinFileDlg(HWND_DESKTOP, hwndClient, (PFILEDLG) &fdg);
  if (hwndDlg && (fdg.lReturn == DID_OK))
    {
      Result1 = strchr(fdg.szFullFile,'.');
      if(Result1 = "NULL")
        Result1 = "BMP";
      strcpy(Test,Result1);

      strupr(Test);
      if(strcmp(Test, VID) == 0 && mmioFOURCC('M','M','O','T') > 0)
        fccTargetIOProc = mmioFOURCC('M','M','O','T');
      else if(strcmp(Test, RDI) == 0 && mmioFOURCC('R','D','I','B') > 0)
        fccTargetIOProc = mmioFOURCC('R','D','I','B');
      else if(strcmp(Test, PCX) == 0 && mmioFOURCC('P','C','X','C') > 0)
        fccTargetIOProc = mmioFOURCC('P','C','X','C');
      else if(strcmp(Test, BMP) == 0 && mmioFOURCC('O','S','2','0') > 0)
        fccTargetIOProc = mmioFOURCC('O','S','2','0');
      else if(strcmp(Test, DIB) == 0 && mmioFOURCC('W','1','3','0') > 0)
        fccTargetIOProc = mmioFOURCC('W','1','3','0');
      else if(strcmp(Test, TGA) == 0 && mmioFOURCC('T','G','A','U') > 0)
        fccTargetIOProc = mmioFOURCC('T','G','A','U');
      else if(strcmp(Test, TIF) == 0 && mmioFOURCC('T','I','F','U') > 0)
        fccTargetIOProc = mmioFOURCC('T','I','F','U');
      else
        {
          WinMessageBox( HWND_DESKTOP,
                         HWND_DESKTOP,
                         "Will Save As a OS2.BMP",
                         "Save As....",
                         (HMODULE) NULL,
                         (ULONG) MB_OK | MB_MOVEABLE );
          fccTargetIOProc = mmioFOURCC('O','S','2','0');
        }
      MakeItBusy(TRUE);
      DoConvert(pszSource, fccSourceIOProc, fdg.szFullFile, fccTargetIOProc);
      MakeItBusy(FALSE);
    }
  
  if(chrExt)
    free(chrExt);

}

/*************************************************************************
 * Name        :  DisplayMessageBox
 *
 * Description :  This procedure will display messages for the application
 *                based upon string IDs passed in.  The actual text will be
 *                loaded from the string table in the resource.
 *
 * Parameters  :  usTitleID   - ID of the title string
 *                usMessageID - ID of the message string
 *                usStyle     - Style of the message box (WinMessageBox)
 *
 * Return      :  none.
 *
 *************************************************************************/

VOID DisplayMessageBox ( HWND   hwnd,
                         USHORT usTitleID,
                         USHORT usMessageID,
                         USHORT usStyle)
{
   CHAR     achTitle[STRING_LENGTH];
   CHAR     achMessage[STRING_LENGTH];
   USHORT   usResult;

   /*
    * Get the string from the Resource defined string table and show it
    * in the message box.
    */
   WinLoadString(
      hab,                          /* HAB for this dialog box.            */
      (HMODULE) NULL,               /* Get the string from the .exe file.  */
      usTitleID,                    /* Which string to get.                */
      (SHORT) sizeof( achTitle),    /* The size of the buffer.             */
      achTitle );                   /* The buffer to place the string.     */

   WinLoadString(
      hab,                          /* HAB for this dialog box.            */
      (HMODULE) NULL,               /* Get the string from the .exe file.  */
      usMessageID,                  /* Which string to get.                */
      (SHORT) sizeof( achMessage),  /* The size of the buffer.             */
      achMessage );                 /* The buffer to place the string.     */

   usResult =
      WinMessageBox(
         HWND_DESKTOP,              /* Parent handle of the message box.   */
         hwnd,                      /* Owner handle of the message box.    */
         achMessage,                /* String to show in the message box.  */
         achTitle,                  /* Title to shown in the message box.  */
         (HMODULE) NULL,            /* Message Box Id.  Not Needed.        */
         (ULONG) usStyle );         /* The style of the message box.       */
}  /* End of DisplayMessageBox */


VOID FourccToString( FOURCC fcc, PSZ pszString )
{
    pszString[0] = (BYTE) fcc;
    pszString[1] = (BYTE) (fcc >> 8);
    pszString[2] = (BYTE) (fcc >> 16);
    pszString[3] = (BYTE) (fcc >> 24);
    pszString[4] = 0;
}

HBITMAP LoadBitmap ( HAB hab,
                     HDC hdc,
                     HPS *hps,
                     PSZ pszFileName )
{
    HBITMAP       hbm;
    MMIOINFO      mmioinfo;
    MMFORMATINFO  mmFormatInfo;
    HMMIO         hmmio;
    ULONG         ulImageHeaderLength;
    ULONG         ulBytesRead;
    ULONG         dwNumRowBytes;
    PBYTE         pRowBuffer;
    ULONG         dwRowCount;
    SIZEL         ImageSize;
    ULONG         dwHeight, dwWidth;
    SHORT          wBitCount;
    FOURCC        fccStorageSystem;
    ULONG         dwPadBytes;
    ULONG         dwRowBits;
    ULONG         ulReturnCode;
    ULONG         dwReturnCode;
    HBITMAP       hbReturnCode;
    LONG          lReturnCode;
    FOURCC        fccIOProc;

    ulReturnCode = mmioIdentifyFile ( pszFileName,
                                      0L,
                                      &mmFormatInfo,
                                      &fccStorageSystem,
                                      0L,
                                      0L);

    /*
     *  If this file was NOT identified, then this function won't
     *  work, so return an error by indicating an empty bitmap.
     */
    if ( ulReturnCode == MMIO_ERROR )
    {
         return (0L);
    }

    /*
     *  If mmioIdentifyFile did not find a custom-written IO proc which
     *  can understand the image file, then it will return the DOS IO Proc
     *  info because the image file IS a DOS file.
     */
    if( mmFormatInfo.fccIOProc == FOURCC_DOS )
    {

         WinMessageBox( HWND_DESKTOP,
                        HWND_DESKTOP,
                        "Image file could not be interpreted by any permanently or temporarily installed IO procedures.",
                        "Open Image File",
                        (HMODULE) NULL,
                        (ULONG) MB_OK | MB_MOVEABLE |
                                MB_ERROR );
         return ( 0L );
    }

    /*
     *  Ensure this is an IMAGE IOproc, and that it can read
     *  translated data
     */
    if ( (mmFormatInfo.ulMediaType != MMIO_MEDIATYPE_IMAGE) ||
         ((mmFormatInfo.ulFlags & MMIO_CANREADTRANSLATED) == 0) )
    {
         WinMessageBox( HWND_DESKTOP,
                        HWND_DESKTOP,
                        "No IMAGE IO Procedures exist which can translate the data in the image file specified.",
                        "Open Image File",
                        (HMODULE) NULL,
                        (ULONG) MB_OK | MB_MOVEABLE |
                                MB_ERROR );
         return (0L);
    }
    else
    {
         fccIOProc = mmFormatInfo.fccIOProc;
    }

    /* Clear out and initialize mminfo structure */
    memset ( &mmioinfo,
             0L,
             sizeof ( MMIOINFO ) );

    mmioinfo.fccIOProc = fccIOProc;
    mmioinfo.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;

    hmmio = mmioOpen ( (PSZ) pszFileName,
                       &mmioinfo,
                       MMIO_READ | MMIO_DENYWRITE | MMIO_NOIDENTIFY );

    if ( ! hmmio )
    {
         // If file could not be opened, return with error
         return (0L);
    }

    dwReturnCode = mmioQueryHeaderLength ( hmmio,
                                         (PLONG)&ulImageHeaderLength,
                                           0L,
                                           0L);

    if ( ulImageHeaderLength != sizeof ( MMIMAGEHEADER ) )
    {
         /* We have a problem.....possibly incompatible versions */
         ulReturnCode = mmioClose (hmmio, 0L);

         return (0L);
    }

    ulReturnCode = mmioGetHeader ( hmmio,
                                   &mmImgHdr,
                                   (LONG) sizeof ( MMIMAGEHEADER ),
                                   (PLONG)&ulBytesRead,
                                   0L,
                                   0L);

    if ( ulReturnCode != MMIO_SUCCESS )
    {
         /* Header unavailable */

         ulReturnCode = mmioClose (hmmio, 0L);

         return (0L);
    }

    /*
     *  Determine the number of bytes required, per row.
     *      PLANES MUST ALWAYS BE = 1
     */
    dwHeight = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy;
    dwWidth = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cx;
    wBitCount = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
    dwRowBits = dwWidth * mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
    dwNumRowBytes = dwRowBits >> 3;

    /*
     *  Account for odd bits used in 1bpp or 4bpp images that are
     *  NOT on byte boundaries.
     */
    if ( dwRowBits % 8 )
    {
         dwNumRowBytes++;
    }

    /*
     *  Ensure the row length in bytes accounts for byte padding.
     *  All bitmap data rows must are aligned on LONG/4-BYTE boundaries.
     *  The data FROM an IOProc should always appear in this form.
     */
    dwPadBytes = ( dwNumRowBytes % 4 );

    if ( dwPadBytes )
    {
         dwNumRowBytes += 4 - dwPadBytes;
    }

    /* Allocate space for ONE row of pels */
    if ( DosAllocMem( (PPVOID)&pRowBuffer,
                      (ULONG)dwNumRowBytes,
                      fALLOC))
    {
         ulReturnCode = mmioClose (hmmio, 0L);

         return(0L);
    }

    // ***************************************************
    // Create a memory presentation space that includes
    // the memory device context obtained above.
    // ***************************************************
    ImageSize.cx = dwWidth;
    ImageSize.cy = dwHeight;

    *hps = GpiCreatePS ( hab,
                         hdc,
                         &ImageSize,
                         PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC );

    if ( !*hps )
    {
         mmioClose (hmmio, 0L);
         return(0L);
    }

    // ***************************************************
    // Create an uninitialized bitmap.  This is where we
    // will put all of the bits once we read them in.
    // ***************************************************
    hbm = GpiCreateBitmap ( *hps,
                            &mmImgHdr.mmXDIBHeader.BMPInfoHeader2,
                            0L,
                            NULL,
                            NULL);
    if ( !hbm )
    {
         ulReturnCode = mmioClose (hmmio, 0L);
         return(0L);
    }

    // ***************************************************
    // Select the bitmap into the memory device context.
    // ***************************************************
    hbReturnCode = GpiSetBitmap ( *hps,
                                  hbm );

    //***************************************************************
    //  LOAD THE BITMAP DATA FROM THE FILE
    //      One line at a time, starting from the BOTTOM
    //*************************************************************** */
    for ( dwRowCount = 0; dwRowCount < dwHeight; dwRowCount++ )
    {
         ulBytesRead = (ULONG) mmioRead ( hmmio,
                                          pRowBuffer,
                                          dwNumRowBytes );
         if ( !ulBytesRead )
         {
              break;
         }

         /*
          *  Allow context switching while previewing.. Couldn't get
          *  it to work. Perhaps will get to it when time is available...
          */
         lReturnCode = GpiSetBitmapBits ( *hps,
                                          (LONG) dwRowCount,
                                          (LONG) 1,
                                          (PBYTE) pRowBuffer,
                                          (PBITMAPINFO2) &mmImgHdr.mmXDIBHeader.BMPInfoHeader2);

    }
    ulReturnCode = mmioClose (hmmio, 0L);
    DosFreeMem(pRowBuffer);
    return(hbm);
}


/*
 *  GetBitMap
 *
 *  Get a bitmap for the specified file
 */
HBITMAP GetBitMap ( HWND hwnd,
                    PSZ  pszFileName)
{
    HBITMAP hBmp;
    BOOL    bReturnCode;

    /*
     *  Load Bitmap, which will then be drawn during WM_PAINT processing
     *
     *  First, we need a memory device context. We'll keep this around
     *  to reuse for subsequent preview operations
     */
    if ( !hdcMemory )
    {
         hdcMemory = DevOpenDC ( WinQueryAnchorBlock(hwnd),
                                 OD_MEMORY,
                                 "*",
                                 0L,
                                 NULL,
                                 0);
    }

    /*
     *  Discard previous memory presentation space if present
     */
    if ( hpsMemory )
    {
         hBmp = GpiSetBitmap ( hpsMemory,
                               0);
         bReturnCode = GpiDestroyPS ( hpsMemory );
         hpsMemory = 0;
    }

    /*
     * Delete previously loaded bitmap if present
     */
    if ( hBmp && hBmp != HBM_ERROR )
    {
         bReturnCode = GpiDeleteBitmap ( hBmp );
    }

    MakeItBusy ( TRUE );

    hBmp = LoadBitmap ( WinQueryAnchorBlock(hwnd),
                        hdcMemory,
                        &hpsMemory,
                        pszFileName );

    MakeItBusy ( FALSE );
    if (!hBmp)
    {
          // error in loading bitmap
    }

    /*
     *  Be sure that the image gets repainted
     */
    bReturnCode = WinInvalidateRect(hwnd, NULL, FALSE);

    return hBmp;
}

/*
 *  DrawBitMap
 *
 *  Draw the previously loaded bitmap in the rectangle occupied by the
 *  frame ID_WINDOW.
 */

VOID DrawBitMap ( HWND hwnd )
{
    SWP    swp;
    POINTL aptl[4];
    HPS    hps;
    SIZEL  ImageSize;
    BOOL   bReturnCode;
    ULONG  ulReturnCode;
    LONG   lReturnCode;
    ULONG  ulHeight;
    ULONG  ulWidth;

    /*
     *  Validate memory presentation space before attempting to draw bitmap
     */
    ulReturnCode = GpiQueryPS ( hpsMemory, &ImageSize) ;
    if ( ! ulReturnCode )
    {
         return;
    }

    hps = WinBeginPaint ( hwnd,
                          0,
                          NULL);
    /*
     *  Get position of image frame
     */
    bReturnCode = WinQueryWindowPos ( hwnd, &swp);

    /* Image size */

    ulHeight = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy;
    ulWidth = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cx;

#if 0
    ulHeight = ImageSize.cy;
    ulWidth =  ImageSize.cx;
    /* Calculate draw position and size. Aspect ratio should be preserved. */
#endif

    if(ulWidth <=swp.cx && ulHeight <= swp.cy)
      {
        aptl[0].x=(swp.cx-ulWidth)/2;
        aptl[1].x=aptl[0].x+ulWidth;
        
        aptl[0].y=(swp.cy-ulHeight)/2;
        aptl[1].y=aptl[0].y+ulHeight;
      }
    else {
      float fWidth, fHeight, fRes;

      fWidth=(float)swp.cx/(float)ulWidth;
      fHeight=(float)swp.cy/(float)ulHeight;
      fRes=( fWidth>fHeight ? fHeight : fWidth);


      aptl[0].x=(swp.cx-ulWidth*fRes)/2;
      aptl[1].x=aptl[0].x+ulWidth*fRes;
      
      aptl[0].y=(swp.cy-ulHeight*fRes)/2;
      aptl[1].y=aptl[0].y+ulHeight*fRes;
      
#if 0
      aptl[0].x = 0;              // target lower left
      aptl[0].y = 0;
      
      aptl[1].x = swp.cx;         // target upper right
      aptl[1].y = swp.cy;
#endif
    }
    
    aptl[2].x = 0;              // source lower left
    aptl[2].y = 0;
    
    aptl[3].x = ulWidth;   // source upper right
    aptl[3].y = ulHeight;

    /*
     *  Call GpiBitBlt and supply 4 aptl structures.  This tells
     *  it to stretch or compress the bitmap depending on what is
     *  in the aptl structures.  See above lines for their current
     *  settings.
     */
    bReturnCode = GpiSetColor (hps, GpiQueryColor (hpsMemory));
    bReturnCode = GpiSetBackColor (hps, GpiQueryBackColor (hpsMemory));

    lReturnCode = GpiBitBlt (hps, hpsMemory, 4L, aptl, ROP_SRCCOPY, BBO_IGNORE);

    bReturnCode = WinEndPaint (hps);
}


/*************************************************************************
 * Name        : InitializeHelp
 *
 * Description : This procedure will set up the initial values in the
 *               global help structure.  This help structure will then
 *               be passed on to the Help Manager when the Help Instance
 *               is created.  The handle to the Help Instance will be
 *               kept for later use.
 *
 * Concepts    : None.
 *
 * MME API's   : None.
 *
 * Parameters  : None.
 *
 * Return      : TRUE - error encountered
 *               FALSE - no errors encountered - help instance created!
 *
 *************************************************************************/
VOID InitializeHelp( HWND hwndFrame)
{
  HELPINIT hini;
  fHelpEnabled = FALSE;
  /* if we return because of an error, Help will be disabled */

  /* initialize help init structure */
  hini.cb = sizeof(HELPINIT);
  hini.ulReturnCode = 0;
  hini.pszTutorialName = (PSZ)NULL;   /* if tutorial added, add name here */
  hini.phtHelpTable = (PHELPTABLE)MAKELONG(MMBROWSER_HELP_TABLE, 0xFFFF);
  hini.hmodHelpTableModule = 0L;
  hini.hmodAccelActionBarModule = 0L;
  hini.idAccelTable = 0L;
  hini.idActionBar = 0L;
  hini.pszHelpWindowTitle = "MMBrowser Help";
  hini.fShowPanelId = CMIC_HIDE_PANEL_ID;
  hini.pszHelpLibraryName = "MMBROWSE.HLP";

  /* creating help instance */

  hwndHelpInstance = WinCreateHelpInstance(hab, &hini);

  if(!hwndHelpInstance || hini.ulReturnCode)
    {
      return;
    }

  /* associate help instance with main frame */

  if(!WinAssociateHelpInstance(hwndHelpInstance, hwndFrame))
    {
      return;

    }

  fHelpEnabled = TRUE;
}

/****************************************************************
 *  Dialog procedure for the About dialog box
 *--------------------------------------------------------------
 *
 *  Name:    AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
 *
 *  Purpose: Handles all the messages associated with the About Box
 *
 *  Usage:   Called for each message sent to the About Box dialog box.
 *
 *  Method:  The about box only has a button control so this routine
 *           only processes WM_COMMAND messages. Any WM_COMMAND
 *           posted must have come from the Ok button so we dismiss
 *           the dialog upon receiving it.
 *
 *  Returns: Dependent upon message sent
 *
 ****************************************************************/
MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   MRESULT sRC;

   switch(msg)
   {
      case WM_INITDLG:
//         FixSysMenu(hwnd);
         return (MRESULT)0L;

      case WM_COMMAND:
         WinDismissDlg(hwnd, TRUE);
         return (MRESULT)0L;

      default:
         sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
         return sRC;
   }
   return (MRESULT)0L;
}

/****************************************************************
 *  FixSysMenu procedure
 *--------------------------------------------------------------
 *
 *  Name:    FixSysMenu(HWND hwndDlg)
 *
 *  Purpose: This routine removes the Restore, Size, Minimize, and
 *           Maximize options from the system menu of a dialog.
 *
 *  Usage
 *
 *  Method:  Called during the WM_INITDLG of a dialog procedure.
 *
 *  Returns: VOID
 *
 ****************************************************************/
VOID FixSysMenu(HWND hwndDlg)
{
  HWND hwndMenu;                      /* Handle to system menu */
  
  hwndMenu = WinWindowFromID(hwndDlg, FID_SYSMENU);
  
  WinSendMsg(hwndMenu,  /* Delete Restore from the system menu */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_RESTORE, TRUE),
             MPFROMP(NULL));
  
  WinSendMsg(hwndMenu,     /* Delete Size from the system menu */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_SIZE, TRUE),
             MPFROMP(NULL));
  
  WinSendMsg(hwndMenu, /* Delete Minimize from the system menu */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_MINIMIZE, TRUE),
             MPFROMP(NULL));
  
  WinSendMsg(hwndMenu, /* Delete Maximize from the system menu */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_MAXIMIZE, TRUE),
             MPFROMP(NULL));
  return;
}

//*******************************************************************
// Function : DoConvert
//
//
// Description :
//    This function provides a simple method of converting an
//    image from one format to another, so long as both are
//    supported by OS/2 MME MMIO IOProcs.  It uses MMIO
//    functions to transparently open, read and write file headers,
//    read and write image data, and close the files.
//
//    This function will also signal the progress of the conversion
//    so that this progress can illustrated to a user.
//
// Parameters (all input, unchanged by function):
//      Source file name
//      Source file IOProc handler
//      Target file name
//      Target file IOProc handler
//
// Return Code:
//      0 if successful, or
//        ERR_CANCELLED
//        ERR_SOURCEOPEN
//        ERR_TARGETOPEN
//        ERR_SOURCEHEADER
//        ERR_TARGETHEADER
//        ERR_SOURCEWRITE
//        ERR_TARGETWRITE
//        ERR_SOURCECLOSE
//        ERR_TARGETCLOSE
//        ERR_MEMORY
//
//********************************************************************
BOOL DoConvert(
               PSZ    pszSource,          // Source file name
               FOURCC fccSourceIOProc,
               PSZ    pszTarget,
               FOURCC fccTargetIOProc)    //
   {
   MMIOINFO      mmioinfoSource;
   MMIOINFO      mmioinfoTarget;
   HMMIO         hmmioSource;
   HMMIO         hmmioTarget;
   ULONG         ulImageHeaderLength;
   MMIMAGEHEADER mmImgHdr;
   ULONG         ulBytesRead;
   ULONG         dwNumRowBytes;
   PBYTE         pRowBuffer;
   ULONG         dwRowCount;
   ULONG         dwWidth;
   ULONG         dwHeight;
   ULONG         dwPadBytes;
   ULONG         dwRowBits;
   LONG          rc;
   LONG          rcSrcQueryCount = 0;
   LONG          rcTrgQueryCount = 0;
   LONG          rcTrgSetImage   = 0;
   ULONG         iIndex, iCount, iCount2;



   /*******************************/
   /* Set up/open the SOURCE file */
   /*******************************/
   memset (&mmioinfoSource, 0L, sizeof (MMIOINFO));
   mmioinfoSource.fccIOProc = fccSourceIOProc;
   mmioinfoSource.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
   memset (&mmioinfoTarget, 0L, sizeof (MMIOINFO));
   mmioinfoTarget.fccIOProc = fccTargetIOProc;
   mmioinfoTarget.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;

   hmmioSource = mmioOpen ((PSZ)pszSource, &mmioinfoSource,
                          MMIO_READ | MMIO_DENYWRITE
                        | MMIO_NOIDENTIFY);

   if (!hmmioSource)
      return (ERR_SOURCEOPEN);

   /*******************************/
   /* Set up/open the TARGET file */
   /*******************************/


   hmmioTarget = mmioOpen ((PSZ)pszTarget,
                           &mmioinfoTarget,
                           MMIO_CREATE | MMIO_WRITE |
                           MMIO_DENYWRITE | MMIO_NOIDENTIFY);

   if (!hmmioTarget)
      {
      mmioClose (hmmioSource, 0L);
      return (ERR_TARGETOPEN);
      }

   // find out if source has multiple image support
   rcSrcQueryCount = mmioSendMessage(hmmioSource, MMIOM_QUERYIMAGECOUNT, (LONG)&iCount, (LONG)0);
   if (rcSrcQueryCount) iCount = 1;

   // find out if the target has multiple image support
   rcTrgQueryCount = mmioSendMessage(hmmioTarget, MMIOM_QUERYIMAGECOUNT, (LONG)&iCount2, (LONG)0);

   for (iIndex=0; iIndex<iCount; iIndex++) { /* loop through known images */

      if (!rcSrcQueryCount && !rcTrgQueryCount) {  /* if Both support images */

         /* Determine if the target can write arrays, and if not */
         /* then write the the default image from the source     */

         if (rcTrgSetImage && iIndex > 0) break;  /* Target Can't Write array */

         /* Now, determine if the target can write arrays */
         rcTrgSetImage = mmioSendMessage (hmmioTarget, MMIOM_SETIMAGE, (LONG)iIndex, (LONG)0);

         if (!rcTrgSetImage) mmioSendMessage (hmmioSource, MMIOM_SETIMAGE, (LONG)iIndex, (LONG)0);

      } else if (!rcSrcQueryCount) {      /* Source does but target doesn't */
         /* Use the default image from source to copy to target */
         /* so do set the index of the first, let it default    */
         /* get the base photo cd image (2 of 5)                */
         if (iIndex > 0) break;
      } else if (!rcTrgQueryCount) {      /* Target does but source doesn't */
         /* Use the only image to do a default write to target */
      } else {
         /* neither do: just write one image from here to there */
      }

      /****************************/
      /* Obtain the SOURCE HEADER */
      /****************************/
      mmioQueryHeaderLength (hmmioSource, (PLONG)&ulImageHeaderLength, 0L, 0L);
      if (ulImageHeaderLength != sizeof (MMIMAGEHEADER))
         /* We have a problem.....possibly incompatible versions */
         {
         mmioClose (hmmioSource, 0L);
         mmioClose (hmmioTarget, 0L);
         return (ERR_SOURCEHEADER);
         }

      rc = (LONG)mmioGetHeader (hmmioSource, &mmImgHdr,
                     (LONG)sizeof (MMIMAGEHEADER), (PLONG)&ulBytesRead,
                     0L, 0L);

      if (rc != MMIO_SUCCESS)
         /* Header unavailable */
         {
         mmioClose (hmmioSource, 0L);
         mmioClose (hmmioTarget, 0L);
         return (ERR_SOURCEHEADER);
         }


      /*************************/
      /* Set the TARGET HEADER */
      /*************************/
      mmioQueryHeaderLength (hmmioTarget, (PLONG)&ulImageHeaderLength, 0L, 0L);
      if (ulImageHeaderLength != sizeof (MMIMAGEHEADER))
         {
         /* We have a problem.....possibly incompatible versions */
         mmioClose (hmmioSource, 0L);
         mmioClose (hmmioTarget, 0L);
         return (ERR_TARGETHEADER);
         }

      /* Use the SAME data as came from the SOURCE FILE.  It must be
         compatible with the OS/2 bitmaps, etc.   */
      rc = (LONG)mmioSetHeader (hmmioTarget, &mmImgHdr,
                     (LONG)sizeof (MMIMAGEHEADER), (PLONG)&ulBytesRead,
                     0L, 0L);

      if (rc != MMIO_SUCCESS)
         /* Header unavailable */
         {
         mmioClose (hmmioSource, 0L);
         mmioClose (hmmioTarget, 0L);
         return (ERR_TARGETHEADER);
         }


      /* Determine the number of bytes required, per row */
      /*      PLANES MUST ALWAYS BE = 1 */
      dwHeight = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy;
      dwWidth  = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cx;
      dwRowBits = dwWidth * mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
      dwNumRowBytes = dwRowBits >> 3;

      /* Account for odd bits used in 1bpp or 4bpp images that are NOT on byte boundaries. */
      if (dwRowBits % 8)
         dwNumRowBytes++;

      /* Ensure the row length in bytes accounts for byte padding.  All bitmap data rows
         must are aligned on LONG/4-BYTE boundaries.   The data FROM an IOProc
         should always appear in this form. */
      dwPadBytes = (dwNumRowBytes % 4);
      if (dwPadBytes)
         dwNumRowBytes += 4 - dwPadBytes;

      /* Allocate space for one row */
      if (DosAllocMem((PVOID)&pRowBuffer, (ULONG)dwNumRowBytes, fALLOC))
         {
         mmioClose (hmmioSource, 0L);
         mmioClose (hmmioTarget, 0L);
         return(ERR_MEMORY);
         }

      for (dwRowCount = 0;
           dwRowCount < dwHeight;
           dwRowCount++)
         {
         ulBytesRead = (ULONG)mmioRead (hmmioSource, pRowBuffer, dwNumRowBytes);

         if (ulBytesRead)
            mmioWrite (hmmioTarget, pRowBuffer, (ULONG)ulBytesRead);
         else
            break;

                                 }
   }

   mmioClose (hmmioSource, 0L);
   mmioClose (hmmioTarget, 0L);

   DosFreeMem(pRowBuffer);
   return(0);
   }

