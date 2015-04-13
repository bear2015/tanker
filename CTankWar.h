#ifndef __CTANKWAR__H__
#define __CTANKWAR__H__

#include "stdafx.h"

class CTankWar
{
public:

	CTankWar( );
	~CTankWar( );

public:

	bool CreateGame( CApp *pApp );
	bool DestroyGame( void );

	bool Update( void );
	bool Render( void );

	CScreenGDI *GetScreen( void ) const;
	CSoundManagerBASS *GetSndMgr( void ) const;
	CMapGDI *GetMap( void ) const;
	CPlayerGDI *GetPlayer( void ) const;
	CEnemyGDI *GetEnemy( void );

	int GetEnemyCount( void ) const;
	void SetEnemyCount( int nCount );
	void SetGameState( GAMESTATE emGameState );

private:

	CApp *m_pApp;
	CScreenGDI *m_pScreen;
	CSoundManagerBASS *m_pSndMgr;
	CMapGDI *m_pMap;
	CPlayerGDI *m_pPlayer;
	CEnemyGDI *m_pEnemy;
	int m_nEnemyCount;
	int m_nStage;
	CSpriteGDI *m_pMainMenu;

	GAMESTATE m_emGameState;
};

#endif