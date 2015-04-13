#include "CPlayerGDI.h"

CPlayerGDI::CPlayerGDI( )
{
	ZeroMemory( &m_rcBound, sizeof( RECT ) );
	m_pSpriteTank = NULL;
	m_pBullet = NULL;
	m_pAnima = NULL;
	m_pAnimaBorn = NULL;
	m_pApp = NULL;
}

CPlayerGDI::~CPlayerGDI( )
{
	if( NULL != m_pSpriteTank )
	{
		delete[ ] m_pSpriteTank;
		m_pSpriteTank = NULL;
	}

	if( NULL != m_pBullet )
	{
		delete m_pBullet;
		m_pBullet = NULL;
	}

	if( NULL != m_pAnima )
	{
		delete m_pAnima;
		m_pAnima = NULL;
	}

	if( NULL != m_pAnimaBorn )
	{
		delete m_pAnimaBorn;
		m_pAnimaBorn = NULL;
	}
}

bool CPlayerGDI::CreatePlayer( CApp *pApp, int nXIndex, int nYIndex )
{
	if( NULL == pApp )
		return false;

	m_pApp = pApp;

	m_pSpriteTank = new CSpriteGDI[ 4 ];

	if( NULL == m_pSpriteTank )
		return false;

	m_pSpriteTank[ 0 ].CreateFromFile( ".\\data\\picture\\player\\PlayerUp.bmp", 36, 36 );
	m_pSpriteTank[ 1 ].CreateFromFile( ".\\data\\picture\\player\\PlayerRight.bmp", 36, 36 );
	m_pSpriteTank[ 2 ].CreateFromFile( ".\\data\\picture\\player\\PlayerDown.bmp", 36, 36 );
	m_pSpriteTank[ 3 ].CreateFromFile( ".\\data\\picture\\player\\PlayerLeft.bmp", 36, 36 );

	for( int i = 0; i < 4; i++ )
	{
		m_pSpriteTank[ i ].SetColorKey( RGB( 255, 0, 255 ) );
		m_pSpriteTank[ i ].SetTargetDC( m_pApp->GetGame( )->GetScreen( )->GetBackBufDC( ) );	
	}

	SetRect( &m_rcBound, nXIndex * 36, nYIndex * 36, nXIndex * 36 + 36, nYIndex * 36 + 36 );

	// 四个方向分别为
	// 0 - 上
	// 1 - 右
	// 2 - 下
	// 3 - 左

	m_nCurDir = 0;
	m_emState = TS_BORN;
	m_nInitXIndex = nXIndex;
	m_nInitYIndex = nYIndex;

	m_pBullet = new CBulletGDI( );

	if( NULL == m_pBullet )
		return false;

	m_pBullet->CreateBullet( m_pApp, true );

	m_pAnima = new CAnimaGDI( );

	if( NULL == m_pAnima )
		return false;

	if( !m_pAnima->CreateAnima( m_pApp, ".\\config\\tankbomb.ini" ) )
		return false;

	m_pAnimaBorn = new CAnimaGDI( );

	if( NULL == m_pAnimaBorn )
		return false;

	if( !m_pAnimaBorn->CreateAnima( m_pApp, ".\\config\\tankborn.ini" ) )
		return false;

	return true;
}

void CPlayerGDI::ResetPlayer( void )
{
	m_rcBound.left = m_nInitXIndex * 36;
	m_rcBound.top = m_nInitYIndex * 36;
	m_emState = TS_BORN;
	m_nCurDir = 0;
	m_nLifeCount--;
}

bool CPlayerGDI::Update( void )
{
	if( NULL == m_pApp )
		return false;

	switch( m_emState )
	{
		/*
	case TS_BORN:
		{
		}
		break;

	case TS_DEATH:
		{
		}
		break;
		*/

	case TS_WALKING:
		{
			// 必须行驶一段距离后才能返回受控状态
			static const int DISTANCE = 36;
			static const int MOVESTEP = 6;
			static int nCurDis = 0;

			// 根据坦克的当前方向进行移动
			switch( m_nCurDir )
			{
			// 向上移动
			case 0:
				{
					m_rcBound.top -= MOVESTEP;
				}
				break;

			// 向右移动
			case 1:
				{
					m_rcBound.left += MOVESTEP;
				}
				break;

			// 向下移动
			case 2:
				{
					m_rcBound.top += MOVESTEP;
				}
				break;

			// 向左移动
			case 3:
				{
					m_rcBound.left -= MOVESTEP;
				}
				break;
			}

			if( m_rcBound.left < 0 )
				m_rcBound.left = 0;
			if( m_rcBound.left > 680 - 36 )
				m_rcBound.left = 680 - 36;
			if( m_rcBound.top < 0 )
				m_rcBound.top = 0;
			if( m_rcBound.top > 540 - 36 )
				m_rcBound.top = 540 - 36;

			nCurDis += MOVESTEP;
			if( nCurDis >= DISTANCE )
			{
				nCurDis = 0;
				m_emState = TS_CONTORL;
			}
		}
		break;

	// 玩家属于受控状态，此时可以控制坦克
	case TS_CONTORL:
		{
			int nXIndex = m_rcBound.left / 36;
			int nYIndex = m_rcBound.top / 36;

			if( m_pApp->GetKeyState( VK_UP ) )
			{
				if( m_nCurDir != 0 )
				{
					m_nCurDir = 0;
				}
				else
				{
					if( 0 != m_pApp->GetGame( )->GetMap( )->GetMapItemData( nXIndex, nYIndex - 1 ) )
						return true;

					m_nCurDir = 0;
					m_emState = TS_WALKING;
				}
			}
			else if( m_pApp->GetKeyState( VK_RIGHT ) )
			{
				if( m_nCurDir != 1 )
				{
					m_nCurDir = 1;
				}
				else
				{
					if( 0 != m_pApp->GetGame( )->GetMap( )->GetMapItemData( nXIndex + 1, nYIndex ) )
						return true;

					m_nCurDir = 1;
					m_emState = TS_WALKING;
				}
			}
			else if( m_pApp->GetKeyState( VK_DOWN ) )
			{
				if( m_nCurDir != 2 )
				{
					m_nCurDir = 2;
				}
				else
				{
					if( 0 != m_pApp->GetGame( )->GetMap( )->GetMapItemData( nXIndex, nYIndex + 1 ) )
						return true;

					m_nCurDir = 2;
					m_emState = TS_WALKING;
				}
			}
			else if( m_pApp->GetKeyState( VK_LEFT ) )
			{
				if( m_nCurDir != 3 )
				{
					m_nCurDir = 3;
				}
				else
				{
					if( 0 != m_pApp->GetGame( )->GetMap( )->GetMapItemData( nXIndex - 1, nYIndex ) )
						return true;

					m_nCurDir = 3;
					m_emState = TS_WALKING;
				}
			}
			else if( m_pApp->IsKeyDown( VK_SPACE ) )
			{
				m_emState = TS_SHOOT;
			}
		}
		break;

	case TS_SHOOT:
		{
			if( !m_pBullet->IsShow( ) )
				m_pApp->GetGame( )->GetSndMgr( )->PlaySoundDirect( ".\\data\\sound\\shoot.wav" );

			m_pBullet->Shoot( m_nCurDir, m_rcBound.left, m_rcBound.top );

			m_emState = TS_CONTORL;
		}
		break;
	}
	
	m_pBullet->Update( );

	return true;
}

bool CPlayerGDI::Render( void )
{
	int nXIndex = m_rcBound.left / 36;
	int nYIndex = m_rcBound.top / 36;
	
	if( m_pApp->GetKeyState( VK_F3 ) )
	{
		char szInfo[ 20 ];
		ZeroMemory( szInfo, sizeof( szInfo ) );
		sprintf( szInfo, "%d %d", nXIndex, nYIndex );
		m_pApp->GetGame( )->GetScreen( )->RenderTextToBackDC( szInfo, 0, 0, 62, 32, RGB( 255, 255, 0 ) );
	}

	if( TS_BORN == m_emState )
	{
		if( !m_pAnimaBorn->IsAnimaStop( ) )
		{
			int nHalfOfAnima = m_pAnima->AnimaWidth( ) / 2;
			int nAnimaXPos = m_rcBound.left + 13 - nHalfOfAnima + 36;
			int nAnimaYPos = m_rcBound.top + 13 - nHalfOfAnima + 36;

			m_pAnimaBorn->PlayAnima( nAnimaXPos, nAnimaYPos, 3, false );
			m_emState = TS_BORN;
		}
		else
		{
			m_pAnimaBorn->SetState( AS_PLAYING );
			m_emState = TS_CONTORL;
		}
	}
	else if( TS_DEATH == m_emState )
	{
		if( !m_pAnima->IsAnimaStop( ) )
		{
			int nHalfOfAnima = m_pAnima->AnimaWidth( ) / 2;
			int nAnimaXPos = m_rcBound.left + 13 - nHalfOfAnima;
			int nAnimaYPos = m_rcBound.top + 13 - nHalfOfAnima;

			m_pAnima->PlayAnima( nAnimaXPos, nAnimaYPos, 2, false );
			m_emState = TS_DEATH;
		}
		else
		{
			m_pAnima->SetState( AS_PLAYING );
			ResetPlayer( );
		}		
	}
	else
	{
		m_pSpriteTank[ m_nCurDir ].MoveTo( m_rcBound.left, m_rcBound.top );
		m_pSpriteTank[ m_nCurDir ].RenderToDC( );

		m_pBullet->Render( );
	}

	return true;
}

int CPlayerGDI::GetXPos( void ) const
{
	return m_rcBound.left;
}

void CPlayerGDI::SetXPos( int nXPos )
{
	m_rcBound.left = nXPos;
}

int CPlayerGDI::GetYPos( void ) const
{
	return m_rcBound.top;
}

void CPlayerGDI::SetYPos( int nYPos )
{
	m_rcBound.top = nYPos;
}

TANKSTATE CPlayerGDI::GetState( void ) const
{
	return m_emState;
}

void CPlayerGDI::SetState( TANKSTATE emState )
{
	m_emState = emState;
}

int CPlayerGDI::GetLifeCount( void ) const
{
	return m_nLifeCount;
}

void CPlayerGDI::SetLifeCount( int nLife )
{
	m_nLifeCount = nLife;
}

int CPlayerGDI::GetDir( void ) const
{
	return m_nCurDir;
}

void CPlayerGDI::SetDir( int nDir )
{
	m_nCurDir = nDir;
}