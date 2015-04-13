#ifndef __CINI__H__
#define __CINI__H__

#include <windows.h>
#include <stdio.h>

class CIni
{
	public:
		CIni( );
		CIni( const char* const szIniFileName );
		~CIni( );

	public:
		bool SetIniFile( const char* szIniFileName );
		bool SetSectionName( const char* szSectionName );

		char* GetString( const char* szKeyName );
		char* GetStringEx( const char* szSectionName, const char* szKeyName );
		int GetInt( const char* szKeyName );
		int GetIntEx( const char* szSectionName, const char* szKeyName );
		float GetFloat( const char* szKeyName );
		float GetFloatEx( const char* szSectionName, const char* szKeyName );

	private:
		char m_szIniFileName[ MAX_PATH ];
		char m_szSectionName[ MAX_PATH ];
};

#endif