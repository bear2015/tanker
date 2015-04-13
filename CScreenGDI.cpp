#include "CScreenGDI.h"

CScreenGDI::CScreenGDI( )
{
	m_hFrontDC = NULL;
	m_hBackDC = NULL;
	m_hOldFont = NULL;
	m_hNewFont = NULL;
}

CScreenGDI::~CScreenGDI( )
{
}

bool CScreenGDI::RenderTextToBackDC( char *szText, int nXPos, int nYPos, int nWidth, int nHeight, DWORD dwColor )
{
	SetTextColor( m_hBackDC, dwColor );
	SetBkMode( m_hBackDC, TRANSPARENT );

	RECT rcDraw = { nXPos, nYPos, nXPos + nWidth, nYPos + nHeight };

	DrawText( m_hBackDC, szText, strlen( szText ), &rcDraw, DT_LEFT );

	return true;
}

bool CScreenGDI::Clear( HWND hWnd, DWORD dwClearColor )
{
	LOGBRUSH logBrush;
	logBrush.lbColor = dwClearColor;
	logBrush.lbHatch = 0;
	logBrush.lbStyle = BS_SOLID;

	HBRUSH hBrush = ::CreateBrushIndirect( &logBrush );

	if( NULL == hBrush )
		return false;

	RECT rcClient;
	::GetClientRect( hWnd, &rcClient );

	FillRect( m_hBackDC, &rcClient, hBrush );

	DeleteObject( hBrush );

	return true;
}

bool CScreenGDI::Present( void ) const
{
	if( 0 == ::BitBlt( m_hFrontDC, m_nXPos, m_nYPos, m_nWidth, m_nHeight, m_hBackDC, 0, 0, SRCCOPY ) )
		return false;

	return true;
}

bool CScreenGDI::CreateFont( char *szFaceName, int nWidth, int nHeight, bool bItalic, bool bBold )
{
	if( NULL != m_hNewFont )
	{
		DeleteObject( m_hNewFont );
		m_hNewFont = NULL;
	}

	LOGFONT logFont;
	ZeroMemory( &logFont, sizeof( LOGFONT ) );

	strcpy( logFont.lfFaceName, szFaceName );
	logFont.lfWidth = nWidth;
	logFont.lfHeight = nHeight;
	logFont.lfItalic = bItalic;
	logFont.lfWeight = bBold ? 700 : 400 ;

	m_hNewFont = ::CreateFontIndirect( &logFont );

	if( NULL == m_hNewFont )
		return false;

	m_hOldFont = ( HFONT )::SelectObject( m_hBackDC, m_hNewFont );

	return true;
}

bool CScreenGDI::SetRenderTargetWindow( HWND hWnd )
{
	if( NULL == hWnd )
		return false;

	m_hFrontDC = ::GetDC( hWnd );

	if( NULL == m_hFrontDC )
		return false;

	if( NULL != m_hBackDC )
	{
		DeleteDC( m_hBackDC );
		m_hBackDC = NULL;
	}

	m_hBackDC = CreateCompatibleDC( m_hFrontDC );

	if( NULL == m_hBackDC )
		return false;

	RECT rcClient;
	GetClientRect( hWnd, &rcClient );

	HBITMAP hBitmap = CreateCompatibleBitmap( m_hFrontDC, rcClient.right, rcClient.bottom );

	if( NULL == hBitmap )
		return false;

	SelectObject( m_hBackDC, hBitmap );

	return true;
}

void CScreenGDI::SetRenderPos( int nXPos, int nYPos )
{
	m_nXPos = nXPos;
	m_nYPos = nYPos;
}

void CScreenGDI::SetRenderSize( int nWidth, int nHeight )
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

HDC CScreenGDI::GetBackBufDC( void ) const
{
	return m_hBackDC;
}