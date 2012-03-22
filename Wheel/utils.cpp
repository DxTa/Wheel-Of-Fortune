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

	if (FAILED(hr = SHCreateStreamOnFile(L"file.xml", STGM_READ, &(*pFileStream)))) {
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

bool Utils::xmlat(IXmlReader *pReader, IStream *pFileStream, int index, string *question, string *answer) {
	HRESULT hr;
	const WCHAR* pwszLocalName;
   const WCHAR* pwszValue;
   XmlNodeType nodeType;
	int i=0;//i la` bien dem'. j bien' de? lay ra phan` tu? thu' j. j = 1 thi` lay' phan` tu? dau` tien
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
		 if (wcscmp(pwszLocalName,L"Question") == 0 && index == i) {
			 (*question).clear();
			 *question = strdup(narrow(pwszValue).c_str());
			 std::transform((*question).begin(), (*question).end(), (*question).begin(), ::toupper); //qui đổi sang CAP để tránh lỗi
			 check++;
		 }
		 else if (wcscmp(pwszLocalName,L"Answer") == 0 && index == i) {
			 (*answer).clear();
			 *answer = strdup(narrow(pwszValue).c_str());  
			 std::transform((*answer).begin(), (*answer).end(), (*answer).begin(), ::toupper); //qui đổi sang CAP để tránh lỗi
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
   if (check == 2) return true;
   else return false;
}