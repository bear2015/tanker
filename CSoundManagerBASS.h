#ifndef __CSOUNDMANAGERBASS__H__
#define __CSOUNDMANAGERBASS__H__

#include "stdafx.h"

class CSoundManagerBASS
{
public:
	CSoundManagerBASS( );
	~CSoundManagerBASS( );

public:
	bool CreateSoundManager( HWND hWnd, DWORD dwMaxChannel );
	HSTREAM AddSound( char *szFileName );
	bool PlaySound( HSTREAM hStream, bool bLoop, float fVol = 0.6 );
	bool PauseSound( HSTREAM hStream );
	bool StopSound( HSTREAM hStream );
	bool FreeSound( HSTREAM hStream );

	bool PlaySoundDirect( char *szFileName ) const;

private:
	DWORD m_dwMaxChannel;
	DWORD m_dwCount;

	HSTREAM *m_pStream;
	SOUNDSTATE *m_pSoundState;
};

#endif