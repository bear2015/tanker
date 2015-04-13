#include "CApp.h"

CApp::CApp( )
{
	m_nFPS = 25;
	m_nFrame = 0;
	m_nRealFPS = 0;
	m_pGame = NULL;
	ZeroMemory( m_szWndTitle, sizeof( m_szWndTitle ) );
}

CApp::~CApp( )
{
}

bool CApp::CreateApp( char *szWndTitle, int nWidth, int nHeight )
{
	m_hApp = ::GetModuleHandle( NULL );

	if( NULL == m_hApp )
		return false;

	strcpy( m_szWndTitle, szWndTitle );

	WNDCLASS wc = { CS_CLASSDC, MsgProc, 0, 0, m_hApp,
					( HICON )::LoadIcon( NULL, IDI_WINLOGO ),
					( HCURSOR )::LoadCursor( NULL, MAKEINTRESOURCE(32649) ),
					( HBRUSH )::GetStockObject( BLACK_BRUSH ),
					NULL,
					WINDOW_CLASS_NAME };

	if( !::RegisterClass( &wc ) )
		return false;

	nWidth = ::GetSystemMetrics( SM_CXFIXEDFRAME ) * 2 + nWidth;
	nHeight = ::GetSystemMetrics( SM_CYFIXEDFRAME ) * 2 + ::GetSystemMetrics( SM_CYCAPTION ) + nHeight;
	int nXPos = ( ::GetSystemMetrics( SM_CXSCREEN ) - nWidth ) / 2;
	int nYPos = ( ::GetSystemMetrics( SM_CYSCREEN ) - nHeight ) / 2;

	m_hWnd = ::CreateWindow( WINDOW_CLASS_NAME,	m_szWndTitle,
							WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
							nXPos, nYPos, nWidth, nHeight,
							NULL, NULL, m_hApp, NULL );

	if( !m_hWnd )
		return false;

	::ShowWindow( m_hWnd, SW_SHOW );
	::UpdateWindow( m_hWnd );

	timeBeginPeriod( 1 );

	m_pGame = new CTankWar( );

	if( NULL == m_pGame )
		return false;

	m_pGame->CreateGame( this );

	return true;
}

bool CApp::CreateAppFromIniFile( char *szIniFileName )
{
	CIni config;

	if( !config.SetIniFile( szIniFileName ) )
		return false;

	config.SetSectionName( "app" );

	strcpy( m_szWndTitle, config.GetString( "title" ) );

	m_nWndWidth = config.GetInt( "width" );
	m_nWndHeight = config.GetInt( "height" );

	return CApp::CreateApp( m_szWndTitle, m_nWndWidth, m_nWndHeight );
}

bool CApp::RunApp( void )
{
	MSG msg;

	if( -1 == ::GetMessage( &msg, NULL, 0, 0 ) )
		return false;

	while( WM_QUIT != msg.message )
	{
		if( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}
		else
		{
			static DWORD dwTimePrevFrame = ::timeGetTime( );
			static DWORD dwSecondBegin = ::timeGetTime( );

			DWORD dwTimeThisFrameBegin = ::timeGetTime( );

			if( dwTimeThisFrameBegin - dwTimePrevFrame >= ( 1000 / m_nFPS ) )
			{
				dwTimePrevFrame = dwTimeThisFrameBegin;

				if( ::timeGetTime( ) - dwSecondBegin >= 1000 )
				{
					dwSecondBegin = ::timeGetTime( );

					m_nRealFPS = m_nFrame;
					m_nFrame = 0;
				}

				m_nFrame++;

				// Time to render and update ...

				if( NULL != m_pGame )
				{
					m_pGame->Update( );
					m_pGame->Render( );
				}

				m_nFrameInterval = ::timeGetTime( ) - dwTimeThisFrameBegin;
			}
			else
			{
				if( dwTimeThisFrameBegin - dwTimePrevFrame <= ( 1000 / m_nFPS - 5 ) )
					Sleep( 1 );
			}
		}
	}

	return true;
}

void CApp::DestroyApp( void )
{
	if( NULL != m_pGame )
	{
		m_pGame->DestroyGame( );

		delete m_pGame;
		m_pGame = NULL;
	}

	timeEndPeriod( 1 );

	UnregisterClass( WINDOW_CLASS_NAME, m_hApp );
	m_hApp = NULL;
}

void CApp::KeyClear( void )
{
	ZeroMemory( m_pKeyState, sizeof( m_pKeyState ) );
}

void CApp::KeyDown( WPARAM wParam )
{
	m_pKeyState[ wParam ] = true;
}

void CApp::KeyUp( WPARAM wParam )
{
	m_pKeyState[ wParam ] = false;
}

bool CApp::IsKeyDown( BYTE bKeyCode ) const
{
	return m_pKeyState[ bKeyCode ];
}

bool CApp::GetKeyState( BYTE bKeyCode ) const
{
	return ( ::GetKeyState( bKeyCode ) & 0x8000 ? true : false );
}

void CApp::SetFPS( int nFPS )
{
	if( nFPS < 10 || nFPS > 60 )
		return;

	m_nFPS = nFPS;
}

int CApp::GetFPS( void ) const
{
	return m_nRealFPS;
}

int CApp::GetFrameInterval( void ) const
{
	return m_nFrameInterval;
}

HWND CApp::GetWnd( void ) const
{
	return m_hWnd;
}

CTankWar *CApp::GetGame( void ) const
{
	return m_pGame;
}