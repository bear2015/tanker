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

	// ���ڱ�ʶ���ӵ��Ƿ�����ҵ�̹�����
	// ����ǣ�����ӵ�ֻ����˵�̹�˽�����ײ���
	// ������ǣ�����ӵ�ֻ����ҵ�̹�˽�����ײ���
	bool m_bShootByPlayer;
	bool m_bIsKingHit;

	CSpriteGDI *m_pSpriteBullet;
	CApp *m_pApp;
};

#endif