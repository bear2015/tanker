#ifndef __CSPRITEGDI__H__
#define __CSPRITEGDI__H__

#include "stdafx.h"

class CSpriteGDI
{
public:
	CSpriteGDI( );
	~CSpriteGDI( );

public:
	bool CreateFromFile( char *szFileName, int nWidth, int nHeight );
	bool SetTargetDC( HDC TargetDC );
	bool RenderToDC( void );

	void SetColorKey( DWORD dwColorKey );
	void MoveTo( int nXPos, int nYPos );
	void SetSize( int nWidth, int nHeight );

public:
	HBITMAP m_hBitmap;
	HDC m_hBitmapDC;
	HDC m_hTargetDC;

	// 透明色
	// -1 为不使用透明色
	// RGB( ) 值指定任意透明色
	DWORD m_dwColorKey;
	int m_nXPos, m_nYPos;
	int m_nWidth, m_nHeight;
};

#endif