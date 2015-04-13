#include "CBulletGDI.h"

CBulletGDI::CBulletGDI( )
{
	m_pApp = NULL;
	m_pSpriteBullet = NULL;
}

CBulletGDI::~CBulletGDI( )
{
	if( NULL != m_pSpriteBullet )
	{
		delete m_pSpriteBullet;
		m_pSpriteBullet = NULL;
	}
}

bool CBulletGDI::CreateBullet( CApp *pApp, bool bShootByPlayer )
{
	if( NULL == pApp ) return false;

	m_pApp = pApp;

	m_pSpriteBullet = new CSpriteGDI( );

	if( NULL == m_pSpriteBullet ) return false;

	m_pSpriteBullet->CreateFromFile( ".\\data\\picture\\bullet.bmp", 11, 11 );
	m_pSpriteBullet->SetColorKey( RGB( 0, 0, 0 ) );
	m_pSpriteBullet->SetTargetDC( m_pApp->GetGame( )->GetScreen( )->GetBackBufDC( ) );

	m_bShow = false;
	m_bShootByPlayer = bShootByPlayer;
	m_bIsKingHit = false;

	return true;
}

bool CBulletGDI::Shoot( int nDir, int nXPos, int nYPos )
{
	if( true == m_bShow )
		return true;
	else
		m_bShow = true;

	m_nCurDir = nDir;

	// 根据方向校正子弹的射击位置
	switch( nDir )
	{
	// 向上
	case 0:
		{
			m_rcBound.left = nXPos + ( 36 - 11 ) / 2;
			m_rcBound.top = nYPos;
		}
		break;

	// 向右
	case 1:
		{
			m_rcBound.left = nXPos + 36 - 1;
			m_rcBound.top = nYPos + ( 36 - 11 ) / 2;
		}
		break;

	// 向下
	case 2:
		{
			m_rcBound.left = nXPos + ( 36 - 11 ) / 2;
			m_rcBound.top = nYPos + 36 - 1;
		}
		break;

	// 向左
	case 3:
		{
			m_rcBound.left = nXPos + 1;
			m_rcBound.top = nYPos + ( 36 - 11 ) / 2;
		}
		break;
	}

	return true;
}

bool CBulletGDI::Update( void )
{
	if( m_bShow )
	{
		static const int MOVESTEP = 18;
		int nXIndex = m_rcBound.left / 36;
		int nYIndex = m_rcBound.top / 36;

		switch( m_nCurDir )
		{
		case 0:
			{
				int nNextItem = m_pApp->GetGame( )->GetMap( )->GetMapItemData( nXIndex, nYIndex - 1 );

				if( 0 != nNextItem && 1 != nNextItem )
				{
					if( 2 == nNextItem )
					{
						m_pApp->GetGame( )->GetMap( )->SetMapItemData( 3, nXIndex, nYIndex - 1 );
					}
					else if( 3 == nNextItem )
					{
						m_pApp->GetGame( )->GetMap( )->SetMapItemData( 0, nXIndex, nYIndex - 1 );					
					}
					else if( 5 == nNextItem )
					{
						m_pApp->GetGame( )->GetSndMgr( )->PlaySoundDirect( ".\\data\\sound\\kingbomb.wav" );
						m_bIsKingHit = true;
					}

					m_bShow = false;
					return true;
				}

				m_rcBound.top -= MOVESTEP;
			}
			break;

		case 1:
			{
				int nNextItem = m_pApp->GetGame( )->GetMap( )->GetMapItemData( nXIndex + 1, nYIndex );

				if( 0 != nNextItem && 1 != nNextItem )
				{
					if( 2 == nNextItem )
					{
						m_pApp->GetGame( )->GetMap( )->SetMapItemData( 3, nXIndex + 1, nYIndex );
					}
					else if( 3 == nNextItem )
					{
						m_pApp->GetGame( )->GetMap( )->SetMapItemData( 0, nXIndex + 1, nYIndex );
					}
					else if( 5 == nNextItem )
					{
						m_pApp->GetGame( )->GetSndMgr( )->PlaySoundDirect( ".\\data\\sound\\kingbomb.wav" );
						m_bIsKingHit = true;
					}

					m_bShow = false;
					return true;
				}

				m_rcBound.left += MOVESTEP;
			}
			break;

		case 2:
			{
				int nNextItem = m_pApp->GetGame( )->GetMap( )->GetMapItemData( nXIndex, nYIndex + 1 );

				if( 0 != nNextItem && 1 != nNextItem )
				{
					if( 2 == nNextItem )
					{
						m_pApp->GetGame( )->GetMap( )->SetMapItemData( 3, nXIndex, nYIndex + 1 );
					}
					else if( 3 == nNextItem )
					{
						m_pApp->GetGame( )->GetMap( )->SetMapItemData( 0, nXIndex, nYIndex + 1 );
					}
					else if( 5 == nNextItem )
					{
						m_pApp->GetGame( )->GetSndMgr( )->PlaySoundDirect( ".\\data\\sound\\kingbomb.wav" );
						m_bIsKingHit = true;
					}

					m_bShow = false;
					return true;
				}

				m_rcBound.top += MOVESTEP;
			}
			break;

		case 3:
			{
				int nNextItem = m_pApp->GetGame( )->GetMap( )->GetMapItemData( nXIndex - 1, nYIndex );

				if( 0 != nNextItem && 1 != nNextItem )
				{
					if( 2 == nNextItem )
					{
						m_pApp->GetGame( )->GetMap( )->SetMapItemData( 3, nXIndex - 1, nYIndex );
					}
					else if( 3 == nNextItem )
					{
						m_pApp->GetGame( )->GetMap( )->SetMapItemData( 0, nXIndex - 1, nYIndex );
					}
					else if( 5 == nNextItem )
					{
						m_pApp->GetGame( )->GetSndMgr( )->PlaySoundDirect( ".\\data\\sound\\kingbomb.wav" );
						m_bIsKingHit = true;
					}

					m_bShow = false;
					return true;
				}

				m_rcBound.left -= MOVESTEP;
			}
			break;
		}

		if( m_rcBound.left <= 0 || m_rcBound.left >= 680 ||
			m_rcBound.top <= 0 || m_rcBound.top >= 540 )
		{
			m_bShow = false;
		}

		// 检测玩家的子弹是否击中敌人的坦克或者敌人的子弹击中玩家

		POINT ptBullet = { m_rcBound.left, m_rcBound.top };

		if( m_bShootByPlayer )
		{
			CEnemyGDI *pEnemy = m_pApp->GetGame( )->GetEnemy( );

			for( int i = 0; i < 4; i++ )
			{
				RECT rcEnemy = { pEnemy[ i ].GetXPos( ), pEnemy[ i ].GetYPos( ),
							pEnemy[ i ].GetXPos( ) + 36, pEnemy[ i ].GetYPos( ) + 36 };

				if( 0 != PtInRect( &rcEnemy, ptBullet ) )
				{
					pEnemy[ i ].SetState( TS_DEATH );

					m_pApp->GetGame( )->SetEnemyCount( m_pApp->GetGame( )->GetEnemyCount( ) - 1 );

					m_pApp->GetGame( )->GetSndMgr( )->PlaySoundDirect( ".\\data\\sound\\tankbomb.wav" );

					if( m_pApp->GetGame( )->GetEnemyCount( ) <= 0 )
					{
						m_pApp->GetGame( )->SetGameState( GS_GAMEWIN );
					}

					m_bShow = false;
				}
			}
		}
		else
		{
			CPlayerGDI *pPlayer = m_pApp->GetGame( )->GetPlayer( );

			RECT rcPlayer = { pPlayer->GetXPos( ), pPlayer->GetYPos( ),
							pPlayer->GetXPos( ) + 36, pPlayer->GetYPos( ) + 36 };

			if( 0 != PtInRect( &rcPlayer, ptBullet ) )
			{
				pPlayer->SetState( TS_DEATH );

				m_pApp->GetGame( )->GetSndMgr( )->PlaySoundDirect( ".\\data\\sound\\tankbomb.wav" );

				if( pPlayer->GetLifeCount( ) <= 0 )
					m_pApp->GetGame( )->SetGameState( GS_GAMEOVER );

				m_bShow = false;
			}
		}
	}

	return true;
}

bool CBulletGDI::Render( void )
{
	if( m_bShow )
	{
		m_pSpriteBullet->MoveTo( m_rcBound.left, m_rcBound.top );
		m_pSpriteBullet->RenderToDC( );
	}

	return true;
}

bool CBulletGDI::IsShow( void ) const
{
	return m_bShow;
}