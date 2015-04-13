#ifndef __CBULLETGDI__H__
#define __CBULLETGDI__H__

#include "stdafx.h"

enum BULLTSTATE
{
	BS_SHOOTING = 0,
	BS_DEATH,
};

class CBulletGDI
{
public:
	CBulletGDI( );
	~CBulletGDI( );

public:
	bool CreateBullet( CApp *pApp, bool bShootByPlayer );
	bool Shoot( int nDir, int nXPos, int nYPos );
	bool Update( void );
	bool Render( void );
	bool IsShow( void ) const;

private:
	RECT m_rcBound;
	BULLTSTATE m_emState;
	int m_nCurDir;
	bool m_bShow;

	// 用于标识该子弹是否由玩家的坦克射出
	// 如果是，则该子弹只与敌人的坦克进行碰撞检测
	// 如果不是，则该子弹只与玩家的坦克进行碰撞检测
	bool m_bShootByPlayer;
	bool m_bIsKingHit;

	CSpriteGDI *m_pSpriteBullet;
	CApp *m_pApp;
};

#endif