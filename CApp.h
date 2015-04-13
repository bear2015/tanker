#ifndef __CAPP__H__
#define __CAPP__H__

#include "stdafx.h"

class CApp
{
public:
	CApp( );
	~CApp( );

public:
	bool CreateApp( char *szWndTitle, int nWidth = 640, int nHeight = 480 );
	bool CreateAppFromIniFile( char *szIniFileName );
	bool RunApp( void );
	void DestroyApp( void );

	void KeyClear( void );
	void KeyDown( WPARAM wParam );
	void KeyUp( WPARAM lParam );
	bool IsKeyDown( BYTE bKeyCode ) const;
	bool GetKeyState( BYTE bKeyCode ) const;
	void SetFPS( int nFPS );
	int GetFPS( void ) const;
	int GetFrameInterval( void ) const;
	HWND GetWnd( void ) const;
	CTankWar *GetGame( void ) const;

private:
	HINSTANCE m_hApp;
	HWND m_hWnd;
	char m_szWndTitle[ MAX_PATH ];
	int m_nFPS, m_nFrame, m_nRealFPS, m_nFrameInterval;
	int m_nWndWidth, m_nWndHeight;
	bool m_pKeyState[ 256 ];

	CTankWar *m_pGame;
};

#endif