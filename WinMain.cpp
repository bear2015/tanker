#include "stdafx.h"

CApp g_App;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	g_App.CreateAppFromIniFile( ".\\config\\app.ini" );
	g_App.RunApp( );
	g_App.DestroyApp( );

	return 0;
}

HRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			if( VK_ESCAPE == wParam )
			{
				if( IDYES == ::MessageBox( hWnd, "Exit Game ?", "Exit", MB_YESNO ) )
					::PostQuitMessage( 0 );
				else
					return 0;
			}

			g_App.KeyDown( wParam );
		}
		break;

	case WM_KEYUP:
		{
			g_App.KeyUp( wParam );
		}
		break;

	case WM_CLOSE:
		{
			if( IDYES == ::MessageBox( hWnd, "Exit Game ?", "Exit", MB_YESNO ) )
				::PostQuitMessage( 0 );
			else
				return 0;
		}
		break;

	default:
		return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return 0;
}