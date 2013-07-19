//////////////////////////////////////////////////////////////////////
//	MJim - Alpha
//	Stephens Atomics 
//	Copyright (C) 2013  Matthew James Stephens, B.Eng- Robotics(Hons)
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	<http://www.gnu.org/licenses/>   
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include <string>
	
#define OSG_DES_KEY TEXT("SOFTWARE\\OSG_DES")

class CRegistry
{
public:
	enum cregRestoreEnum
	{
		regVolatile = REG_WHOLE_HIVE_VOLATILE,
		regRefresh = REG_REFRESH_HIVE,
		regNoLazy = REG_NO_LAZY_FLUSH
	};

	enum Keys
	{
		classesRoot,
		currentUser ,
		localMachine ,
		currentConfig,
		users ,
		performanceData ,	//Windows NT/2000
		dynData //Windows 95/98
	};


	CRegistry()
		{
			m_hKey = NULL;
		};

	~CRegistry()
		{
			CloseKey();
			m_hKey = NULL;
		};

	HKEY get_key_from_enum(enum Keys hKey);

	BOOL OpenKey(enum Keys hKey, LPCTSTR szKey);
	BOOL CreateKey(enum Keys hKey, LPCTSTR szKey);
	BOOL DeleteKey(enum Keys hKey, LPCTSTR szKey);
	BOOL DeleteValue(LPCTSTR lpValueName);
	BOOL GetValue(LPCTSTR lpValueName, std::string* p_strValue);
	BOOL GetValue(LPCTSTR lpValueName, DWORD* dwValue);	
	BOOL GetValue(LPCTSTR lpValueName, bool* Value);
	BOOL GetValue(LPCTSTR lpValueName, UINT* Value);
	BOOL SetValue(LPCTSTR lpValueName, LPCTSTR lpData);
	BOOL SetValue(LPCTSTR lpValueName, DWORD dwValue);
	BOOL SetValue(LPCTSTR lpValueName, bool Value);
	BOOL SetValue(LPCTSTR lpValueName, UINT Value);
	BOOL SaveKey(LPCTSTR lpszFileName);
	BOOL RestoreKey(LPCSTR lpszFileName, DWORD dwFlag);
	BOOL LoadKey(enum Keys hKey, LPCTSTR lpszSubKey, LPCSTR lpszFileName);
	void CloseKey();	

protected:
	HKEY m_hKey;
};