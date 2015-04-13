#ifndef __CSCREENGDI__H__
#define __CSCREENGDI__H__

#include "stdafx.h"

class CScreenGDI
{
public:
	CScreenGDI( );
	~CScreenGDI( );

public:
	bool RenderTextToBackDC( char *szText, int nXPos, int nYPos, int nWidth, int nHeight, DWORD dwColor );
	bool Clear( HWND hWnd, DWORD dwClearColor );
	bool Present( void ) const;
	bool CreateFont( char *szFaceName, int nWidth = 15, int nHeight = 30, bool bItalic = false, bool bBold = false );
	bool SetRenderTargetWindow( HWND hWnd );
	void SetRenderPos( int nXPos, int nYPos );
	void SetRenderSize( int nWidth, int nHeight );
	HDC GetBackBufDC( void ) const;

private:
	HDC m_hFrontDC;
	HDC m_hBackDC;
	HFONT m_hNewFont, m_hOldFont;

	int m_nXPos, m_nYPos;
	int m_nWidth, m_nHeight;
};

#endif