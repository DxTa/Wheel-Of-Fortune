#include "Quiz.h"

Quiz::Quiz() : Sprite() {
	question = "";
	answer = "";
}

Quiz::Quiz(string pquestion,string panswer) : Sprite() {
	question = pquestion;
	answer = panswer;
}

void Quiz::addLetter(int i) {
	ostringstream ss;
	ss << answer.at(i);
	Button* button = new Button(ss.str());
	button->setCollidable(false);
	button->setID(i);
	letters.push_back(button);
}

void Quiz::arrangeLetter() {
	std::list<Button*>::iterator iter;
	letters.clear();
	int strlen = answer.length();
	for(int i = 0;i<strlen;i++) {
		addLetter(i);
	}
	iter = letters.begin();
	
}

int Quiz::inputQuiz(int x){
	HRESULT hr;
   CComPtr<IStream> pFileStream;
   CComPtr<IXmlReader> pReader;
   XmlNodeType nodeType;
   //const WCHAR* pwszPrefix;
   const WCHAR* pwszLocalName;
   const WCHAR* pwszValue;
   //UINT cwchPrefix;

   if (FAILED(hr = SHCreateStreamOnFile(L"file.xml", STGM_READ, &pFileStream))) {
      wprintf(L"Error creating file reader, error is %08.8lx", hr);
	   getchar();
      return -1;
   }

   if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL))) {
      wprintf(L"Error creating xml reader, error is %08.8lx", hr);
      return -1;
   }

   if (FAILED(hr = pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit))) {
      wprintf(L"Error setting XmlReaderProperty_DtdProcessing, error is %08.8lx", hr);
      return -1;
   }

   if (FAILED(hr = pReader->SetInput(pFileStream))) {
      wprintf(L"Error setting input for reader, error is %08.8lx", hr);
      return -1;
   }

   int i = 0;
   while (S_OK == (hr = pReader->Read(&nodeType))) {
	   if (nodeType == XmlNodeType_Element) {
		   if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL))) {
            wprintf(L"Error getting local name, error is %08.8lx", hr);
            return -1;
		   }
		   if (wcscmp(pwszLocalName,L"entry") == 0) i++; // i la` so element trong xml
	   }
   }

   LARGE_INTEGER liBeggining = { 0 };
   pFileStream->Seek(liBeggining, STREAM_SEEK_SET, NULL);
   pReader->SetInput(pFileStream);

   int k=0,j=1;//k la` bien dem'. j bien' de? lay ra phan` tu? thu' j. j = 1 thi` lay' phan` tu? dau` tien
   
   while (S_OK == (hr = pReader->Read(&nodeType))) {
      switch (nodeType) {

      case XmlNodeType_Element: 
         if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL))) {
            wprintf(L"Error getting local name, error is %08.8lx", hr);
            return -1;
         }
		 if (wcscmp(pwszLocalName,L"entry") == 0) k++;

         if (FAILED(hr = pReader->MoveToElement())) {
            wprintf(L"Error moving to the element that owns the current attribute node, error is %08.8lx", hr);
            return -1;
         }
         if (pReader->IsEmptyElement() )
            wprintf(L" (empty element)\n");
         break;
      case XmlNodeType_EndElement:
          if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL))) {
            wprintf(L"Error getting local name, error is %08.8lx", hr);
            return -1;
         }
         break;

      case XmlNodeType_Text:

      case XmlNodeType_CDATA:
         if (FAILED(hr = pReader->GetValue(&pwszValue, NULL))) {
            wprintf(L"Error getting value, error is %08.8lx", hr);
            return -1;
         }
		 if (wcscmp(pwszLocalName,L"Question") == 0) {
			 setQuestion(strdup(narrow(pwszValue).c_str()));
		 }
		 else if (wcscmp(pwszLocalName,L"Answer") == 0) {
			 setAnswer(strdup(narrow(pwszValue).c_str()));
		 }
         break;

      case XmlNodeType_ProcessingInstruction:
         if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL))) {
            wprintf(L"Error getting name, error is %08.8lx", hr);
            return -1;
         }
         if (FAILED(hr = pReader->GetValue(&pwszValue, NULL))) {
            wprintf(L"Error getting value, error is %08.8lx", hr);
            return -1;
         }
         wprintf(L"Processing Instruction name:%S value:%S\n", pwszLocalName, pwszValue);
         break;

      case XmlNodeType_Comment:
         if (FAILED(hr = pReader->GetValue(&pwszValue, NULL))) {
            wprintf(L"Error getting value, error is %08.8lx", hr);
            return -1;
         }
         wprintf(L"Comment: %s\n", pwszValue);
         break;

      case XmlNodeType_DocumentType:
         wprintf(L"DOCTYPE is not printed\n");
         break;
      }
	  if (k > j) break;
   }
   return 0;
}