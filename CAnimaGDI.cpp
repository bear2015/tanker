#include "CAnimaGDI.h"

CAnimaGDI::CAnimaGDI( )
{
	m_nFrameCount = 0;
	m_nCurFrame = 0;
	m_pApp = NULL;
	m_pSprite = NULL;
}

CAnimaGDI::~CAnimaGDI( )
{
	if( NULL != m_pSprite )
	{
		delete[ ] m_pSprite;
		m_pSprite = NULL;
	}
}

bool CAnimaGDI::CreateAnima( CApp *pApp, char *szAnimaFile )
{
	if( NULL == pApp )
		return false;

	m_pApp = pApp;

	CIni config;

	if( !config.SetIniFile( szAnimaFile ) )
		return false;

	config.SetSectionName( "anima" );

	m_nFrameCount = config.GetInt( "frame_count" );
	m_nFrameWidth = config.GetInt( "frame_width" );
	m_nFrameHeight = config.GetInt( "frame_height" );

	m_pSprite = new CSpriteGDI[ m_nFrameCount ];

	if( NULL == m_pSprite )
		return false;

	char szFileName[ MAX_PATH ];

	for( int i = 0; i < m_nFrameCount; i++ )
	{
		ZeroMemory( szFileName, MAX_PATH );
		sprintf( szFileName, "frame_%d", i );

		if( false == m_pSprite[ i ].CreateFromFile( config.GetString( szFileName ), m_nFrameWidth, m_nFrameHeight ) )
		{
			MessageBox( NULL, "Create Sprite Failed", "", 0 );
			DWORD dwError = GetLastError( );
		}

		if( false == m_pSprite[ i ].SetTargetDC( m_pApp->GetGame( )->GetScreen( )->GetBackBufDC( ) ) )
		{
			MessageBox( NULL, "Create Sprite Failed", "", 0 );
		}

		m_pSprite[ i ].SetColorKey( RGB( 255, 255, 255 ) );
	}

	m_nCurFrame = 0;
	m_nCurCount = 0;
	m_emState = AS_PLAYING;
	m_nCount = 0;

	return true;
}

bool CAnimaGDI::PlayAnima( int nXPos, int nYPos, int nCount, bool bLoop )
{
	m_pSprite[ m_nCurFrame ].MoveTo( nXPos, nYPos );
	m_pSprite[ m_nCurFrame ].RenderToDC( );
	
	m_nCount++;
	if( m_nCount >= 3 )
	{
		m_nCurFrame++;
		m_nCount = 0;
	}
	
	if( bLoop )
	{
		if( m_nCurFrame >= m_nFrameCount )
		{
			m_nCurFrame = 0;
			m_emState = AS_PLAYING;
		}
	}
	else
	{
		if( m_nCurFrame >= m_nFrameCount )
		{
			m_nCurFrame = 0;
			m_nCurCount++;

			if( m_nCurCount >= nCount )
			{
				m_nCurFrame = 0;
				m_nCurCount = 0;
				m_emState = AS_STOP;
			}
		}
	}
	
	return true;
}

bool CAnimaGDI::PauseAnima( void )
{
	return true;
}

void CAnimaGDI::StopAnima( void )
{
	m_emState = AS_STOP;
}

bool CAnimaGDI::IsAnimaStop( void ) const
{
	return ( AS_STOP == m_emState ? true : false );
}

int CAnimaGDI::AnimaWidth( void ) const
{
	return m_nFrameWidth;
}

void CAnimaGDI::SetState( ANIMASTATE emState )
{
	m_emState = emState;
}

int CAnimaGDI::AnimaHeight( void ) const
{
	return m_nFrameHeight;
}