#ifndef __CMAPGDI__H__
#define __CMAPGDI__H__

#include "stdafx.h"

class CMapGDI
{
public:
	CMapGDI( );
	~CMapGDI( );
	
	CMapGDI( CApp *pApp, char *szMapFileName );
	
public:
	bool CreateMap( CApp *pApp, char *szMapFileName );
	bool ReleaseMap( void );
	bool RenderMapToBackDC( void ) const;
	bool Update( void );
	
	int GetMapWidth( void ) const;
	int GetMapHeight( void ) const;
	int GetMapItemCount( void ) const;
	int GetHorizontalItemCount( void ) const;
	int GetVerticalItemCount( void ) const;
	int GetMapItemData( int nXIndex, int nYIndex ) const;
	void SetMapItemData( int nValue, int nXIndex, int nYIndex );

	CAnimaGDI *GetAnimaKingBomb( void );
	
private:
	char m_szMapFileName[ MAX_PATH ];
	char m_szBkFileName[ MAX_PATH ];
	int m_nWidth, m_nHeight;
	int m_nItemCount;
	int m_nItemWidth, m_nItemHeight;
	int m_nHorizontalItemCount;
	int m_nVerticalItemCount;
	int *m_pMapData;
	CSpriteGDI *m_pSpriteBk;
	CSpriteGDI *m_pSpriteItem;
	CAnimaGDI *m_pAnimaKingBomb;
	
	CApp *m_pApp;
};

#endif