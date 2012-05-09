#include "utils.h"

double Utils::timecount() {
	static double count_temp;
	double count;
	double result;
	count = timeGetTime();
	result = count-count_temp;
	count_temp = count;
	return result;
}


string Utils::narrow( const wstring& str ){
	ostringstream stm ;
	const ctype<char>& ctfacet =
		use_facet< ctype<char> >( stm.getloc() ) ;
	for( size_t i=0 ; i<str.size() ; ++i )
		stm << ctfacet.narrow( str[i], 0 ) ;
	return stm.str() ;
}

bool Utils::xmlopen(IStream **pFileStream, IXmlReader **pReader,WCHAR* filename) {
	HRESULT hr;

	if (FAILED(hr = SHCreateStreamOnFile(filename, STGM_READ, &(*pFileStream)))) {
      wprintf(L"Error creating file reader, error is %08.8lx", hr);
      return false;
   }

   if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) pReader, NULL))) {
      wprintf(L"Error creating xml reader, error is %08.8lx", hr);
      return false;
   }

   if (FAILED(hr = (*pReader)->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit))) {
      wprintf(L"Error setting XmlReaderProperty_DtdProcessing, error is %08.8lx", hr);
      return false;
   }

   if (FAILED(hr = (*pReader)->SetInput(*pFileStream))) {
      wprintf(L"Error setting input for reader, error is %08.8lx", hr);
      return false;
   }
   return true;
}

int Utils::xmlcount(IXmlReader *pReader, IStream *pFileStream, WCHAR* entry) {

	xmlrewind(pReader,pFileStream);

	HRESULT hr;
	XmlNodeType nodeType;
	int i = 0;
	const WCHAR* pwszLocalName;
   while (S_OK == (hr = pReader->Read(&nodeType))) {
	   if (nodeType == XmlNodeType_Element) {
		   if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL))) {
            wprintf(L"Error getting local name, error is %08.8lx", hr);
            return -1;
		   }
		   if (wcscmp(pwszLocalName,entry) == 0) i++; // i la` so element trong xml
	   }
   }
   return i;
}

void Utils::xmlrewind(IXmlReader *pReader, IStream *pFileStream) {
	LARGE_INTEGER liBeggining = { 0 };
   pFileStream->Seek(liBeggining, STREAM_SEEK_SET, NULL);
   pReader->SetInput(pFileStream);
}

bool Utils::xmlat(IXmlReader *pReader, IStream *pFileStream, int index, WCHAR* string1, string *string2) {
	HRESULT hr;
	const WCHAR* pwszLocalName;
   const WCHAR* pwszValue;
   XmlNodeType nodeType;
	int i=-1;//i la` bien dem'. j bien' de? lay ra phan` tu? thu' j. j = 1 thi` lay' phan` tu? dau` tien
   xmlrewind(pReader,pFileStream);
   int check = 0;
   while (S_OK == (hr = pReader->Read(&nodeType))) {
      switch (nodeType) {

      case XmlNodeType_Element: 
         if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL))) {
            wprintf(L"Error getting local name, error is %08.8lx", hr);
            return false;
         }
		 if (wcscmp(pwszLocalName,L"entry") == 0) i++;
		 if (i < index) break;
         if (FAILED(hr = pReader->MoveToElement())) {
            wprintf(L"Error moving to the element that owns the current attribute node, error is %08.8lx", hr);
            return false;
         }
         if (pReader->IsEmptyElement() )
            wprintf(L" (empty element)\n");
         break;
      case XmlNodeType_EndElement:
          if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL))) {
            wprintf(L"Error getting local name, error is %08.8lx", hr);
            return false;
         }
         break;

      case XmlNodeType_Text:

      case XmlNodeType_CDATA:
         if (FAILED(hr = pReader->GetValue(&pwszValue, NULL))) {
            wprintf(L"Error getting value, error is %08.8lx", hr);
            return false;
         }
		 if (wcscmp(pwszLocalName,string1) == 0 && index == i) {
			 (*string2).clear();
			 (*string2) = strdup(narrow(pwszValue).c_str());  
			 std::transform((*string2).begin(), (*string2).end(), (*string2).begin(), ::toupper); //qui đổi sang CAP để tránh lỗi
			 check++;
		 }
         break;

      case XmlNodeType_ProcessingInstruction:
         if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL))) {
            wprintf(L"Error getting name, error is %08.8lx", hr);
            return false;
         }
         if (FAILED(hr = pReader->GetValue(&pwszValue, NULL))) {
            wprintf(L"Error getting value, error is %08.8lx", hr);
            return false;
         }
         wprintf(L"Processing Instruction name:%S value:%S\n", pwszLocalName, pwszValue);
         break;

      case XmlNodeType_Comment:
         if (FAILED(hr = pReader->GetValue(&pwszValue, NULL))) {
            wprintf(L"Error getting value, error is %08.8lx", hr);
            return false;
         }
         wprintf(L"Comment: %s\n", pwszValue);
         break;

      case XmlNodeType_DocumentType:
         wprintf(L"DOCTYPE is not printed\n");
         break;
      }
	  if (i > index) break;
   }
   if (check == 1) return true;
   else return false;
}

DWORD Utils::FindProcess(TCHAR *procname)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	TCHAR fname[100];

	lstrcpy(fname,procname);

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == (HANDLE)-1)
		return 0;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{
			if (lstrcmp(fname,pe32.szExeFile) == 0 )
			{
				CloseHandle (hProcessSnap);
				return pe32.th32ProcessID;
			}
		}
		while (Process32Next(hProcessSnap, &pe32));
	}

	CloseHandle (hProcessSnap);
	return 0;
}

bool Utils::KillProcess(char *szProcessToKill){
	DWORD pid = FindProcess(wchar(szProcessToKill));
	if(pid) {
		HANDLE hProcess = OpenProcess(
			PROCESS_ALL_ACCESS,FALSE,pid); 
		TerminateProcess (hProcess, 0);    
		CloseHandle(hProcess);
		return true;
	}
	else
		return false;
}