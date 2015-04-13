#include "CMapGDI.h"

CMapGDI::CMapGDI( )
{
	m_pSpriteBk = NULL;
	m_pSpriteItem = NULL;
	m_pMapData = NULL;
	m_pAnimaKingBomb = NULL;
}

CMapGDI::~CMapGDI( )
{
	CMapGDI::ReleaseMap( );
}

CMapGDI::CMapGDI( CApp *pApp, char *szMapFileName )
{
	CMapGDI::CMapGDI( );

	CreateMap( pApp, szMapFileName );
}

bool CMapGDI::CreateMap( CApp *pApp, char *szMapFileName )
{
	if( NULL == pApp )
		return false;

	m_pApp = pApp;

	m_nWidth = 680;
	m_nHeight = 540;

	// 5 种地图元素 分别是
	//
	// 1 河流
	// 2 墙
	// 3 破裂的墙
	// 4 钢铁
	// 5 基地

	m_nItemCount = 5;
	m_nItemWidth = 36;
	m_nItemHeight = 48;

	m_nHorizontalItemCount = m_nWidth / m_nItemWidth + 1;
	m_nVerticalItemCount = m_nHeight / m_nItemWidth + 1;

	strcpy( m_szMapFileName, szMapFileName );
	strcpy( m_szBkFileName, ".\\data\\map\\bk1.bmp" );

	m_pSpriteBk = new CSpriteGDI( );

	if( NULL == m_pSpriteBk )
		return false;

	m_pSpriteBk->CreateFromFile( m_szBkFileName, m_nWidth, m_nHeight );
	m_pSpriteBk->SetTargetDC( m_pApp->GetGame( )->GetScreen( )->GetBackBufDC( ) );

	m_pSpriteItem = new CSpriteGDI[ m_nItemCount ];

	if( NULL == m_pSpriteItem )
		return false;

	FILE *pFile = fopen( m_szMapFileName, "rb" );

	fseek( pFile, 0, SEEK_END );

	int nFileSize = ftell( pFile );

	if( 0 == nFileSize )
		return false;

	fseek( pFile, 0, SEEK_SET );

	m_pMapData = new int[ nFileSize ];

	ZeroMemory( m_pMapData, sizeof( int ) * nFileSize );

	int nCount = 0;

	while( !feof( pFile ) )
	{
		char cChar = ( char )fgetc( pFile );

		if( ' ' != cChar && '\t' != cChar && '\r' != cChar && '\n' != cChar )
		{
			char szToken[ 2 ];
			sprintf( szToken, "%c\0", cChar );

			int nIntValue = ( int )atoi( szToken );

			if( nIntValue >= 0 && nIntValue <= m_nItemCount )
			{
				m_pMapData[ nCount ] = nIntValue;

				nCount++;
			}
		}
	}

	if( NULL != pFile )
		fclose( pFile );

	m_pSpriteItem[ 0 ].CreateFromFile( ".\\data\\map\\ItemRiver.bmp", m_nItemWidth, m_nItemHeight );
	m_pSpriteItem[ 0 ].SetColorKey( RGB( 255, 0, 255 ) );
	m_pSpriteItem[ 1 ].CreateFromFile( ".\\data\\map\\ItemWall.bmp", m_nItemWidth, m_nItemHeight );
	m_pSpriteItem[ 1 ].SetColorKey( RGB( 255, 0, 255 ) );
	m_pSpriteItem[ 2 ].CreateFromFile( ".\\data\\map\\ItemWallTragic.bmp", m_nItemWidth, m_nItemHeight );
	m_pSpriteItem[ 2 ].SetColorKey( RGB( 255, 255, 255 ) );
	m_pSpriteItem[ 3 ].CreateFromFile( ".\\data\\map\\ItemIron.bmp", m_nItemWidth, m_nItemHeight );
	m_pSpriteItem[ 3 ].SetColorKey( RGB( 255, 0, 255 ) );
	m_pSpriteItem[ 4 ].CreateFromFile( ".\\data\\map\\ItemKing.bmp", m_nItemWidth, m_nItemHeight );
	m_pSpriteItem[ 4 ].SetColorKey( RGB( 0, 0, 0 ) );

	for( int i = 0; i < m_nItemCount; i++ )
	{
		m_pSpriteItem[ i ].SetTargetDC( m_pApp->GetGame( )->GetScreen( )->GetBackBufDC( ) );
	}

	m_pAnimaKingBomb = new CAnimaGDI( );

	if( NULL == m_pAnimaKingBomb )
		return false;

	m_pAnimaKingBomb->CreateAnima( m_pApp, ".\\config\\tankbomb.ini" );

	return true;
}

bool CMapGDI::ReleaseMap( void )
{
	if( NULL != m_pSpriteBk )
	{
		delete m_pSpriteBk;
		m_pSpriteBk = NULL;
	}

	if( NULL != m_pSpriteItem )
	{
		delete[ ] m_pSpriteItem;
		m_pSpriteItem = NULL;
	}

	if( NULL != m_pMapData )
	{
		delete[ ] m_pMapData;
		m_pMapData = NULL;
	}

	if( NULL != m_pAnimaKingBomb )
	{
		delete m_pAnimaKingBomb;
		m_pAnimaKingBomb = NULL;
	}

	return true;
}

bool CMapGDI::RenderMapToBackDC( void ) const
{
	char szToken[ 2 ];

	m_pSpriteBk->RenderToDC( );

	for( int nY = 0; nY < m_nVerticalItemCount; nY++ )
	{
		for( int nX = 0; nX < m_nHorizontalItemCount; nX++ )
		{
			int nXPos = nX * m_nItemWidth;
			int nYPos = nY * m_nItemWidth;
			int nIndex = nY * m_nHorizontalItemCount + nX;
			int nItemIndex = m_pMapData[ nIndex ];

			if( 0 != nItemIndex && nItemIndex <= 5 )
			{
				m_pSpriteItem[ nItemIndex - 1 ].MoveTo( nXPos, nYPos );
				m_pSpriteItem[ nItemIndex - 1 ].RenderToDC( );
			}

			if( m_pApp->GetKeyState( VK_F3 ) )
			{
				sprintf( szToken, "%d\0", nItemIndex );
				m_pApp->GetGame( )->GetScreen( )->RenderTextToBackDC( szToken, nXPos, nYPos,
					m_nItemWidth, m_nItemHeight, RGB( 255, 255, 255 ) );
			}
		}
	}

	return true;
}

bool CMapGDI::Update( void )
{
	return true;
}

int CMapGDI::GetMapWidth( void ) const
{
	return m_nWidth;
}

int CMapGDI::GetMapHeight( void ) const
{
	return m_nHeight;	
}

int CMapGDI::GetMapItemCount( void ) const
{
	return m_nItemCount;
}

int CMapGDI::GetHorizontalItemCount( void ) const
{
	return m_nHorizontalItemCount;
}

int CMapGDI::GetVerticalItemCount( void ) const
{
	return m_nVerticalItemCount;
}

int CMapGDI::GetMapItemData( int nXIndex, int nYIndex ) const
{
	return m_pMapData[ nYIndex * m_nHorizontalItemCount + nXIndex ];
}

void CMapGDI::SetMapItemData( int nValue, int nXIndex, int nYIndex )
{
	m_pMapData[ nYIndex * m_nHorizontalItemCount + nXIndex ] = nValue;
}

CAnimaGDI *CMapGDI::GetAnimaKingBomb( void )
{
	return m_pAnimaKingBomb;
}
