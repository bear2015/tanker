#ifndef __CPLAYERGDI__H__
#define __CPLAYERGDI__H__

#include "stdafx.h"

class CPlayerGDI
{
public:
	CPlayerGDI( );
	~CPlayerGDI( );

public:
	bool CreatePlayer( CApp *pApp, int nXIndex, int nYIndex );
	void ResetPlayer( void );
	bool Update( void );
	bool Render( void );

	int GetXPos( void ) const;
	void SetXPos( int nXPos );
	int GetYPos( void ) const;
	void SetYPos( int nYPos );
	TANKSTATE GetState( void ) const;
	void SetState( TANKSTATE emState );
	int GetLifeCount( void ) const;
	void SetLifeCount( int nLife );
	int GetDir( void ) const;
	void SetDir( int nDir );

private:
	RECT m_rcBound;
	TANKSTATE m_emState;
	int m_nCurDir;
	int m_nLifeCount;
	int m_nInitXIndex, m_nInitYIndex;

	CSpriteGDI *m_pSpriteTank;
	CBulletGDI *m_pBullet;
	CAnimaGDI *m_pAnima;
	CAnimaGDI *m_pAnimaBorn;
	CApp *m_pApp;
};

#endif