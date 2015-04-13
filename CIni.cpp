#include "CIni.h"

CIni::CIni( )
{
	::ZeroMemory( m_szIniFileName, sizeof( m_szIniFileName ) );
	::ZeroMemory( m_szSectionName, sizeof( m_szSectionName ) );
}

CIni::CIni( const char* szIniFileName )
{
	CIni::CIni( );

	CIni::SetIniFile( szIniFileName );
}

CIni::~CIni( )
{
}

bool CIni::SetIniFile( const char* szIniFileName )
{
	if( 0 != strlen( szIniFileName ) )
	{
		FILE *pFile = NULL;
		
		fopen_s( &pFile, szIniFileName, "rb" );

		if( NULL == pFile ) return false;

		fclose( pFile );

		strcpy_s( m_szIniFileName, MAX_PATH, szIniFileName );
	}

	return true;
}

bool CIni::SetSectionName( const char* szSectionName )
{
	if( 0 == strlen( szSectionName ) || NULL == m_szIniFileName ) return false;

	strcpy_s( m_szSectionName, MAX_PATH, szSectionName );

	return true;
}

char* CIni::GetString( const char* szKeyName )
{
	if( NULL == m_szSectionName ) return NULL;

	return CIni::GetStringEx( m_szSectionName, szKeyName );
}

char* CIni::GetStringEx( const char* szSectionName, const char* szKeyName )
{
	static char szRet[ MAX_PATH ];
	ZeroMemory( szRet, MAX_PATH );

	GetPrivateProfileStringA( szSectionName, szKeyName, "", szRet, sizeof( szRet ), m_szIniFileName );

	return szRet;
}

int CIni::GetInt( const char* szKeyName )
{
	if( NULL == m_szSectionName ) return NULL;

	return CIni::GetIntEx( m_szSectionName, szKeyName );
}

int CIni::GetIntEx( const char* szSectionName, const char* szKeyName )
{
	static char szRet[ MAX_PATH ];
	ZeroMemory( szRet, MAX_PATH );

	GetPrivateProfileStringA( szSectionName, szKeyName, "", szRet, sizeof( szRet ), m_szIniFileName );

	int nRet = atoi( szRet );

	return nRet;
}

float CIni::GetFloat( const char* szKeyName )
{
	if( NULL == m_szSectionName ) return NULL;

	return CIni::GetFloatEx( m_szSectionName, szKeyName );
}

float CIni::GetFloatEx( const char* szSectionName, const char* szKeyName )
{
	static char szRet[ MAX_PATH ];
	ZeroMemory( szRet, MAX_PATH );

	GetPrivateProfileStringA( szSectionName, szKeyName, "", szRet, sizeof( szRet ), m_szIniFileName );

	float fRet = ( float )atof( szRet );

	return fRet;
}