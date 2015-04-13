#ifndef __CANIMAGID__H__
#define __CANIMAGID__H__

#include "stdafx.h"

class CAnimaGDI
{
public:
	CAnimaGDI( );
	~CAnimaGDI( );

public:
	bool CreateAnima( CApp *pApp, char *szAnimaFile );
	bool PlayAnima( int nXPos, int nYPos, int nCount, bool bLoop );
	bool PauseAnima( void );
	void StopAnima( void );
	void SetState( ANIMASTATE emState );
	bool IsAnimaStop( void ) const;

	int AnimaWidth( void ) const;
	int AnimaHeight( void ) const;

private:
	ANIMASTATE m_emState;
	int m_nFrameCount;
	int m_nCurFrame;
	int m_nFrameWidth, m_nFrameHeight;
	int m_nCurCount;
	int m_nCount;

	CApp *m_pApp;
	CSpriteGDI *m_pSprite;
};

#endif