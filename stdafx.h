#ifndef __STDAFX__H__
#define __STDAFX__H__

#include <windows.h>
#include <mmsystem.h>
#include <assert.h>
#include <stdio.h>
#include "bass.h"

#pragma comment( lib, "bass.lib" )
#pragma comment( lib, "msimg32.lib" )
#pragma comment( lib, "winmm.lib" )

#pragma warning( disable : 4018 )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4996 )
#pragma warning( disable : 4200 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4200 )
#pragma warning( disable : 4244 )

#define WINDOW_CLASS_NAME "__class__name__"

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int );
HRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

enum GAMESTATE
{
	GS_LOADING = 0,
	GS_PLAYING,
	GS_GAMEOVER,
	GS_GAMEWIN,
};

enum TANKSTATE
{
	TS_BORN = 0,
	TS_WALKING,
	TS_CONTORL,
	TS_SHOOT,
	TS_DEATH,
};

enum ANIMASTATE
{
	AS_STOP = 0,
	AS_PLAYING,
	AS_PAUSE,
};

struct SOUNDSTATE
{
	char szFileName[ MAX_PATH ];
	int nIndex;
	DWORD dwLength;
	DWORD dwCurPos;
};

class CApp;
class CScreenGDI;
class CSpriteGDI;
class CTankWar;
class CMapGDI;
class CPlayerGDI;
class CEnemyGDI;
class CBulletGDI;
class CAnimaGDI;
class CSoundManagerBASS;

#include "CApp.h"
#include "CScreenGDI.h"
#include "CSpriteGDI.h"
#include "CTankWar.h"
#include "CIni.h"
#include "CMapGDI.h"
#include "CPlayerGDI.h"
#include "CEnemyGDI.h"
#include "CBulletGDI.h"
#include "CAnimaGDI.h"
#include "CSoundManagerBASS.h"

#endif