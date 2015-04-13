#include "CSoundManagerBASS.h"

CSoundManagerBASS::CSoundManagerBASS( )
{
	m_dwMaxChannel = 0;
	m_dwCount = 0;
	m_pStream = NULL;
	m_pSoundState = NULL;;

}

CSoundManagerBASS::~CSoundManagerBASS( )
{
	if( NULL != m_pSoundState )
	{
		delete[ ] m_pSoundState;
		m_pSoundState = NULL;
	}

	BASS_Free( );
}

bool CSoundManagerBASS::CreateSoundManager( HWND hWnd, DWORD dwMaxChannel )
{
	if( !BASS_Init( -1, 44100, 0, hWnd, NULL ) )
		return false;

	m_dwMaxChannel = dwMaxChannel;

	m_pStream = new HSTREAM[ m_dwMaxChannel ];

	if( NULL == m_pStream )
		return false;

	ZeroMemory( m_pStream, sizeof( HSTREAM ) * m_dwMaxChannel );

	m_pSoundState = new SOUNDSTATE[ m_dwMaxChannel ];

	if( NULL == m_pSoundState )
		return false;

	ZeroMemory( m_pSoundState, sizeof( SOUNDSTATE ) * m_dwMaxChannel );

	m_dwCount = 0;

	return true;
}

HSTREAM CSoundManagerBASS::AddSound( char *szFileName )
{
	m_pStream[ m_dwCount ] = BASS_StreamCreateFile( false, szFileName, 0, 0, 0 );

	if( NULL == m_pStream[ m_dwCount ] )
		return 0;

	strcpy_s( m_pSoundState[ m_dwCount ].szFileName, MAX_PATH, szFileName );

	m_pSoundState[ m_dwCount ].nIndex = m_dwCount;
	m_pSoundState[ m_dwCount ].dwLength = BASS_ChannelGetLength( m_pStream[ m_dwCount ], BASS_POS_BYTE );
	m_pSoundState[ m_dwCount ].dwCurPos = 0;

	m_dwCount++;

	return m_pStream[ m_dwCount - 1 ];
}

bool CSoundManagerBASS::PlaySound( HSTREAM hStream, bool bLoop, float fVol )
{
	BASS_ChannelPlay( hStream, true );

	BASS_ChannelSetAttribute( hStream, BASS_ATTRIB_VOL, fVol );

	return true;
}

bool CSoundManagerBASS::PauseSound( HSTREAM hStream )
{
	return BASS_ChannelPause( hStream );
}

bool CSoundManagerBASS::StopSound( HSTREAM hStream )
{
	return BASS_ChannelStop( hStream );
}

bool CSoundManagerBASS::FreeSound( HSTREAM hStream )
{
	return true;
}

bool CSoundManagerBASS::PlaySoundDirect( char *szFileName ) const
{
	HSTREAM hStream = BASS_StreamCreateFile( false, szFileName, 0, 0, 0 );
	return BASS_ChannelPlay( hStream, true );
}
