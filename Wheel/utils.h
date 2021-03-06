#ifndef H_ULTILITY_H
#define H_ULTILITY_H
#include "Engine/Advanced2D.h"
#include <atlbase.h>
#include "xmllite.h"
#include <iostream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <tlhelp32.h>

using namespace Advanced2D;
using namespace std;

namespace Utils {
	double timecount();
	string narrow( const wstring& str );
	bool xmlopen(IStream **pFileStream, IXmlReader **pReader,WCHAR* filename);
	int xmlcount(IXmlReader *pReader, IStream *pFileStream, WCHAR* entry);
	void xmlrewind(IXmlReader *pReader, IStream *pFileStream);
	bool xmlat(IXmlReader *pReader, IStream *pFileStream, int index, WCHAR* string1, string *string2);
	DWORD FindProcess(TCHAR*);
	bool KillProcess(char*);
}

#endif