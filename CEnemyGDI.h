#ifndef __CENEMGDI__H__
#define __CENEMGDI__H__

#include "stdafx.h"

class CEnemyGDI
{
public:
	CEnemyGDI( );
	~CEnemyGDI( );

public:
	bool CreateEnemy( CApp *pApp, int nXIndex, int nYIndex );
	void ResetEnemy( void );
	bool Update( void );
	bool Render( void );

	int GetXPos( void ) const;
	void SetXPos( int nXPos );
	int GetYPos( void ) const;
	void SetYPos( int nYPos );
	TANKSTATE GetState( void ) const;
	void SetState( TANKSTATE emState );
	int GetDir( void ) const;
	void SetDir( int nDir );

private:
	RECT m_rcBound;
	TANKSTATE m_emState;
	int m_nCurDir;
	int m_nCurDis;
	int m_nPrevXPos, m_nPrevYPos;
	int m_nTimeBornBegin;
	int m_nTimeBornWait;
	int m_nCurXIndex, m_nCurYIndex;
	int m_nInitXIndex, m_nInitYIndex;

	CBulletGDI *m_pBullet;
	CSpriteGDI *m_pSpriteEnemy;
	CAnimaGDI *m_pAnima;
	CAnimaGDI *m_pAnimaBorn;
	CApp *m_pApp;
};

#endif