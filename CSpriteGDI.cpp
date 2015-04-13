#include "CSpriteGDI.h"

CSpriteGDI::CSpriteGDI( )
{
	m_hBitmap = NULL;
	m_hBitmapDC = NULL;
	m_hTargetDC = NULL;
	m_dwColorKey = -1;
	m_nXPos = m_nYPos = 0;
}

CSpriteGDI::~CSpriteGDI( )
{
	if( NULL != m_hBitmap )
		DeleteObject( m_hBitmap );

	if( NULL != m_hBitmapDC )
		DeleteObject( m_hBitmapDC );
}

bool CSpriteGDI::CreateFromFile( char *szFileName, int nWidth, int nHeight )
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_hBitmap = ( HBITMAP )::LoadImage( NULL, szFileName, IMAGE_BITMAP,
		m_nWidth, m_nHeight, LR_LOADFROMFILE | LR_CREATEDIBSECTION );

	if( ! m_hBitmap )
	{
		m_hBitmap = ( HBITMAP )::LoadImage( NULL, szFileName, IMAGE_BITMAP,
			m_nWidth, m_nHeight, LR_LOADFROMFILE );
	}

	// DWORD dwError = GetLastError( );

	if( NULL == m_hBitmap )
		return false;

	return true;
}

bool CSpriteGDI::SetTargetDC( HDC TargetDC )
{
	m_hTargetDC = TargetDC;

	if( NULL != m_hBitmapDC )
	{
		DeleteDC( m_hBitmapDC );
		m_hBitmapDC = NULL;
	}

	m_hBitmapDC = ::CreateCompatibleDC( m_hTargetDC );

	if( NULL == m_hBitmapDC )
		return false;

	::SelectObject( m_hBitmapDC, m_hBitmap );

	return true;
}

bool CSpriteGDI::RenderToDC( void )
{
	if( -1 == m_dwColorKey )
	{
		::BitBlt( m_hTargetDC, m_nXPos, m_nYPos, m_nWidth, m_nHeight, m_hBitmapDC, 0, 0, SRCCOPY );
	}
	else
	{
		::TransparentBlt( m_hTargetDC, m_nXPos, m_nYPos, m_nWidth, m_nHeight,
							m_hBitmapDC, 0, 0, m_nWidth, m_nHeight,	( UINT )m_dwColorKey );
	}

	return true;
}

void CSpriteGDI::SetColorKey( DWORD dwColorKey )
{
	m_dwColorKey = dwColorKey;
}

void CSpriteGDI::MoveTo( int nXPos, int nYPos )
{
	m_nXPos = nXPos;
	m_nYPos = nYPos;
}

void CSpriteGDI::SetSize( int nWidth, int nHeight )
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}