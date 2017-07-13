
#define FCTL_LEFT   1
#define FCTL_RIGHT  2
#define FCTL_TOP    4
#define FCTL_BOTTOM 8

#define FCTL_POSABOVE  1
#define FCTL_POSBELOW  2

/* Structure which holds information about our
   frame controls */
typedef struct {
  HWND hwndCtlLeft;
  BOOL bLeft;
  ULONG ulFlagsLeft;
  SIZEL sizelLeftFrame;
  
  HWND hwndCtlRight;
  BOOL bRight;
  ULONG ulFlagsRight;
  SIZEL sizelRightFrame;
  
  HWND hwndCtlTop;
  BOOL bTop;
  ULONG ulFlagsTop;
  SIZEL sizelTopFrame;
  char chrCtlTopScript[CCHMAXPATH];
  
  HWND hwndCtlBottom;
  BOOL bBottom;
  ULONG ulFlagsBottom;
  SIZEL sizelBottomFrame;
  char chrCtlBottomScript[CCHMAXPATH];

  BOOL bCnrAdjusted;
  //  CWFolder * cwFolder;
}FRAMECTRLDATA;
