/* For calling the method in the CWAudio class. Linking to the DLL would require the
   new audio classes. By dynamically querying the method the media folder works without
   installing the audio classes. */  
typedef ULONG   SOMLINK somTP_MMAudio_cwmmQueryTrackInfo(MMAudio *somSelf,
		PSZ    chrString,
		ULONG ulSize,
		int iWhich);
typedef somTP_MMAudio_cwmmQueryTrackInfo *somTD_MMAudio_cwmmQueryTrackInfo;

extern somTD_MMAudio_cwmmQueryTrackInfo methodPtr;
