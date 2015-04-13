#include "CEnemyGDI.h"

CEnemyGDI::CEnemyGDI( )
{
	m_pSpriteEnemy = NULL;
	m_pBullet = NULL;
	m_pAnima = NULL;
	m_pAnimaBorn = NULL;
	m_pApp = NULL;
}

CEnemyGDI::~CEnemyGDI( )
{
	if( NULL != m_pSpriteEnemy )
	{
		delete[ ] m_pSpriteEnemy;
		m_pSpriteEnemy = NULL;
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

bool CEnemyGDI::CreateEnemy( CApp *pApp, int nXIndex, int nYIndex )
{
	if( NULL == pApp ) return false;

	m_pApp = pApp;

	m_pSpriteEnemy = new CSpriteGDI[ 4 ];

	if( NULL == m_pSpriteEnemy ) return false;

	m_pSpriteEnemy[ 0 ].CreateFromFile( ".\\data\\picture\\enemy\\EnemyUp.bmp", 36, 36 );
	m_pSpriteEnemy[ 1 ].CreateFromFile( ".\\data\\picture\\enemy\\EnemyRight.bmp", 36, 36 );
	m_pSpriteEnemy[ 2 ].CreateFromFile( ".\\data\\picture\\enemy\\EnemyDown.bmp", 36, 36 );
	m_pSpriteEnemy[ 3 ].CreateFromFile( ".\\data\\picture\\enemy\\EnemyLeft.bmp", 36, 36 );

	for( int i = 0; i < 4; i++ )
	{
		m_pSpriteEnemy[ i ].SetColorKey( RGB( 255, 0, 255 ) );
		m_pSpriteEnemy[ i ].SetTargetDC( m_pApp->GetGame( )->GetScreen( )->GetBackBufDC( ) );	
	}

	SetRect( &m_rcBound, nXIndex * 36, nYIndex * 36, nXIndex * 36 + 36, nYIndex * 36 + 36 );

	m_nCurDir = 2;
	m_emState = TS_BORN;
	m_nCurDis = 0;
	m_nTimeBornBegin = timeGetTime( );
	m_nTimeBornWait = 0;
	m_nInitXIndex = nXIndex;
	m_nInitYIndex = nYIndex;
	m_nCurXIndex = m_nInitXIndex;
	m_nCurYIndex = m_nInitYIndex;

	m_pBullet = new CBulletGDI( );

	if( NULL == m_pBullet )
		return false;

	m_pBullet->CreateBullet( m_pApp, false );

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

void CEnemyGDI::ResetEnemy( void )
{
	m_rcBound.left = m_nInitXIndex * 36;
	m_rcBound.top = m_nInitYIndex * 36;
	m_emState = TS_BORN;
	m_nCurDir = 2;
}

bool CEnemyGDI::Update( void )
{
	if( NULL == m_pApp ) return false;

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
			static const int DISTANCE = 36;
			static const int MOVESTEP = 6;

			switch( m_nCurDir )
			{
			case 0:
				{
					m_nPrevYPos -= MOVESTEP;
				}
				break;

			case 1:
				{
					m_nPrevXPos += MOVESTEP;
				}
				break;

			case 2:
				{
					m_nPrevYPos += MOVESTEP;
				}
				break;

			case 3:
				{
					m_nPrevXPos -= MOVESTEP;
				}
				break;
			}

			if( m_nPrevXPos < 0 )
				m_nPrevXPos = 0;
			if( m_nPrevXPos > 680 - 36 )
				m_nPrevXPos = 680 - 36;
			if( m_nPrevYPos < 0 )
				m_nPrevYPos = 0;
			if( m_nPrevYPos > 540 - 36 )
				m_nPrevYPos = 540 - 36;

			m_rcBound.left = m_nPrevXPos;
			m_rcBound.top = m_nPrevYPos;

			m_nCurDis += MOVESTEP;
			if( m_nCurDis >= DISTANCE )
			{
				m_nCurDis = 0;
				m_emState = TS_CONTORL;
			}
		}
		break;

	case TS_CONTORL:
		{
			int nXIndex = m_rcBound.left / 36;
			int nYIndex = m_rcBound.top / 36;

			// 敌人坦克的行为随机确定
			int nNextAction = rand( ) % 4;

			if( 0 == nNextAction )
			{
				if( 0 != m_pApp->GetGame( )->GetMap( )->GetMapItemData( nXIndex, nYIndex - 1 ) )
					break;
				
				m_nCurDir = 0;
				m_emState = TS_WALKING;
			}
			else if( 1 == nNextAction )
			{
				if( 0 != m_pApp->GetGame( )->GetMap( )->GetMapItemData( nXIndex + 1, nYIndex ) )
					break;
				
				m_nCurDir = 1;
				m_emState = TS_WALKING;
			}
			else if( 2 == nNextAction )
			{
				if( 0 != m_pApp->GetGame( )->GetMap( )->GetMapItemData( nXIndex, nYIndex + 1 ) )
					break;
				
				m_nCurDir = 2;
				m_emState = TS_WALKING;
			}
			else if( 3 == nNextAction )
			{
				if( 0 != m_pApp->GetGame( )->GetMap( )->GetMapItemData( nXIndex - 1, nYIndex ) )
					break;
				
				m_nCurDir = 3;
				m_emState = TS_WALKING;
			}

			if( 0 == rand( ) % 4 )
			{
				m_emState = TS_SHOOT;
			}

			// 校正坦克索引
			m_nCurXIndex = m_rcBound.left / 36;
			m_nCurYIndex = m_rcBound.top / 36;

			// 校正坦克坐标
			m_nPrevXPos = m_nCurXIndex * 36;
			m_nPrevYPos = m_nCurYIndex * 36;
		}
		break;

	case TS_SHOOT:
		{
			m_pBullet->Shoot( m_nCurDir, m_rcBound.left, m_rcBound.top );
			m_emState = TS_CONTORL;
		}
		break;
	}
	
	m_pBullet->Update( );

	return true;
}

bool CEnemyGDI::Render( void )
{
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
			// 校正爆炸动画的播放坐标
			int nHalfOfAnima = m_pAnima->AnimaWidth( ) / 2;
			int nAnimaXPos = m_rcBound.left - nHalfOfAnima;
			int nAnimaYPos = m_rcBound.top - nHalfOfAnima;
			m_pAnima->PlayAnima( nAnimaXPos, nAnimaYPos, 2, false );
			m_emState = TS_DEATH;
		}
		else
		{
			m_pAnima->SetState( AS_PLAYING );
			ResetEnemy( );
		}	
	}
	else
	{
		m_pSpriteEnemy[ m_nCurDir ].MoveTo( m_rcBound.left, m_rcBound.top );
		m_pSpriteEnemy[ m_nCurDir ].RenderToDC( );

		m_pBullet->Render( );
	}

	if( m_pApp->GetKeyState( 'O' ) )
	{
		char szInfo[ 100 ];
		int nCurIndex = ( m_rcBound.top / 36 ) * m_pApp->GetGame( )->GetMap( )->GetHorizontalItemCount( ) + ( m_rcBound.left / 36 );
		sprintf( szInfo, "CD %d CI %d\nX %d Y %d", m_nCurDir, nCurIndex, m_rcBound.left, m_rcBound.top );
		m_pApp->GetGame( )->GetScreen( )->RenderTextToBackDC( szInfo, m_rcBound.left + 36, m_rcBound.top, 250, 100, RGB( 125, 125, 125 ) );
	}

	return true;
}

int CEnemyGDI::GetXPos( void ) const
{
	return m_rcBound.left;
}

void CEnemyGDI::SetXPos( int nXPos )
{
	m_rcBound.left = nXPos;
}

int CEnemyGDI::GetYPos( void ) const
{
	return m_rcBound.top;
}

void CEnemyGDI::SetYPos( int nYPos )
{
	m_rcBound.top = nYPos;
}

TANKSTATE CEnemyGDI::GetState( void ) const
{
	return m_emState;
}

void CEnemyGDI::SetState( TANKSTATE emState )
{
	m_emState = emState;
}

int CEnemyGDI::GetDir( void ) const
{
	return m_nCurDir;
}

void CEnemyGDI::SetDir( int nDir )
{
	m_nCurDir = nDir;
}