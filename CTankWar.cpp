#include "CTankWar.h"

CTankWar::CTankWar( )
{
	m_pApp = NULL;
	m_pScreen = NULL;
	m_pSndMgr = NULL;
	m_pMap = NULL;
	m_pPlayer = NULL;
	m_pEnemy = NULL;
	m_pMainMenu = NULL;

	srand( ( unsigned int )::timeGetTime( ) );
}

CTankWar::~CTankWar( )
{
}

bool CTankWar::CreateGame( CApp *pApp )
{
	if( NULL == pApp )
		return false;

	m_pApp = pApp;

	m_pScreen = new CScreenGDI( );

	if( NULL == m_pScreen )
		return false;

	RECT rcClient;
	GetClientRect( m_pApp->GetWnd( ), &rcClient );

	m_pScreen->SetRenderPos( 0, 0 );
	m_pScreen->SetRenderSize( rcClient.right, rcClient.bottom );
	m_pScreen->SetRenderTargetWindow( m_pApp->GetWnd( ) );
	m_pScreen->CreateFont( "Arial", 15, 30, true, true );

	m_pSndMgr = new CSoundManagerBASS( );

	if( NULL == m_pSndMgr )
		return false;

	if( !m_pSndMgr->CreateSoundManager( m_pApp->GetWnd( ), 10 ) )
		return false;

	m_pMap = new CMapGDI( m_pApp, ".\\data\\map\\map1.txt" );

	m_pPlayer = new CPlayerGDI( );

	if( NULL == m_pPlayer )
		return false;

	m_pPlayer->CreatePlayer( m_pApp, 7, 14 );
	m_pPlayer->SetLifeCount( 3 );

	m_pEnemy = new CEnemyGDI[ 4 ];

	if( NULL == m_pEnemy )
		return false;

	m_pEnemy[ 0 ].CreateEnemy( m_pApp, 0, 0 );
	m_pEnemy[ 1 ].CreateEnemy( m_pApp, 6, 0 );
	m_pEnemy[ 2 ].CreateEnemy( m_pApp, 12, 0 );
	m_pEnemy[ 3 ].CreateEnemy( m_pApp, 18, 0 );

	m_pMainMenu = new CSpriteGDI( );

	if( NULL == m_pMainMenu )
		return false;

	m_pMainMenu->CreateFromFile( ".\\data\\picture\\主菜单.bmp", 680, 540 );
	m_pMainMenu->SetTargetDC( m_pScreen->GetBackBufDC( ) );

	m_nEnemyCount = 15;
	m_nStage = 1;
	m_emGameState = GS_LOADING;

	return true;
}

bool CTankWar::Update( )
{
	static bool bDebug = false;
	static int nFrame = 0;

	nFrame++;

	switch( m_emGameState )
	{
	case GS_LOADING:
		{
			static bool play = false;
			if( !play )
			{
				m_pSndMgr->PlaySoundDirect( ".\\data\\sound\\star.wav" );
				play = true;
			}
		}
		break;

	case GS_PLAYING:
		{
			static bool play = false;

			if( !play )
			{
				HSTREAM hStage1 = m_pSndMgr->AddSound( ".\\data\\sound\\stage1.wav" );
				m_pSndMgr->PlaySound( hStage1, true, 0.8f );
				play = true;
			}

			if( m_pApp->IsKeyDown( VK_F4 ) )
			{
				bDebug = !bDebug;
				m_pApp->KeyClear( );
			}

			if( bDebug ) return true;

			m_pPlayer->Update( );

			for( int i = 0; i < 4; i++ )
			{
				m_pEnemy[ i ].Update( );
			}
		}
		break;

	case GS_GAMEOVER:
		{
			static bool play = false;
			if( !play )
			{
				// m_pSndMgr->PlaySoundDirect( ".\\data\\sound\\star.wav" );
				play = true;
			}
		}
		break;

	case GS_GAMEWIN:
		{
			static bool play = false;
			if( !play )
			{
				// m_pSndMgr->PlaySoundDirect( ".\\data\\sound\\star.wav" );
				play = true;
			}
		}
		break;
	}

	char szInfo[ 100 ];
	sprintf( szInfo, "TankWar V3 Frame %d FPS %d FrameInterval %d 剩余敌人数量 %d 我方剩余生命 %d",
		nFrame, m_pApp->GetFPS( ), m_pApp->GetFrameInterval( ), m_nEnemyCount, m_pPlayer->GetLifeCount( ) );
	
	SetWindowText( m_pApp->GetWnd( ), szInfo );

	m_pApp->KeyClear( );

	return true;
}

bool CTankWar::Render( )
{
	static int nTime = 0;

	if( NULL != m_pScreen )
	{
		m_pScreen->Clear( m_pApp->GetWnd( ), RGB( 0, 0, 0 ) );

		switch( m_emGameState )
		{
		case GS_LOADING:
			{
				static DWORD dwTimeBegin = timeGetTime( );

				m_pMainMenu->RenderToDC( );

				nTime += timeGetTime( ) - dwTimeBegin;

				if( nTime >= 4500 )
				{
					nTime = 0;
					m_emGameState = GS_PLAYING;
				}

				// char *szInfo = "Loading Please Wait ...";
				// m_pScreen->RenderTextToBackDC( szInfo, 190, 240, 680, 200, RGB( 255, 255, 0 ) );

				dwTimeBegin = timeGetTime( );
			}
			break;

		case GS_PLAYING:
			{
				m_pMap->RenderMapToBackDC( );
				m_pPlayer->Render( );

				for( int i = 0; i < 4; i++ ) m_pEnemy[ i ].Render( );
			}
			break;

		case GS_GAMEOVER:
			{
				char *szInfo = "Game Over";
				m_pScreen->RenderTextToBackDC( szInfo, 190, 240, 680, 200, RGB( 255, 255, 0 ) );
			}
			break;

		case GS_GAMEWIN:
			{
				char *szInfo = "You Win";
				m_pScreen->RenderTextToBackDC( szInfo, 190, 240, 680, 200, RGB( 255, 255, 0 ) );
			}
			break;
		}

		m_pScreen->Present( );
	}

	return true;
}

bool CTankWar::DestroyGame( )
{
	if( NULL != m_pScreen )
	{
		delete m_pScreen;
		m_pScreen = NULL;
	}

	if( NULL != m_pMap )
	{
		m_pMap->ReleaseMap( );

		delete m_pMap;
		m_pMap = NULL;
	}

	if( NULL != m_pPlayer )
	{
		delete m_pPlayer;
		m_pPlayer = NULL;
	}

	if( NULL != m_pEnemy )
	{
		delete[ ] m_pEnemy;
		m_pEnemy = NULL;
	}

	if( NULL != m_pSndMgr )
	{
		delete m_pSndMgr;
		m_pSndMgr = NULL;
	}

	if( NULL != m_pMainMenu )
	{
		delete m_pMainMenu;
		m_pMainMenu = NULL;
	}

	return true;
}

CScreenGDI *CTankWar::GetScreen( void ) const
{
	return m_pScreen;
}

CSoundManagerBASS *CTankWar::GetSndMgr( void ) const
{
	return m_pSndMgr;
}

CMapGDI *CTankWar::GetMap( void ) const
{
	return m_pMap;
}

CPlayerGDI *CTankWar::GetPlayer( void ) const
{
	return m_pPlayer;
}

CEnemyGDI *CTankWar::GetEnemy( void )
{
	return m_pEnemy;
}

int CTankWar::GetEnemyCount( void ) const
{
	return m_nEnemyCount;
}

void CTankWar::SetEnemyCount( int nCount )
{
	m_nEnemyCount = nCount;
}

void CTankWar::SetGameState( GAMESTATE emGameState )
{
	m_emGameState = emGameState;
}