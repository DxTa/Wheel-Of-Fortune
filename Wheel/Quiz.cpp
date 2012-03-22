#include "Quiz.h"


Quiz::Quiz() : Sprite() {
	question = "";
	answer = "";
	number_of_words = 0;
	count_words.push_back(-1);
}

Quiz::Quiz(string pquestion,string panswer) : Sprite() {
	question = pquestion;
	answer = panswer;
	number_of_words = 0;
	count_words.push_back(-1);
}

void Quiz::setQuestion(string pquestion) {
		std::transform(pquestion.begin(), pquestion.end(), pquestion.begin(), ::toupper);
		question = pquestion;
}

void Quiz::setAnswer(string panswer) {
		std::transform(panswer.begin(), panswer.end(), panswer.begin(), ::toupper);
		answer = panswer;
}

void Quiz::addLetter(int i) {
	ostringstream ss;
	if (answer.at(i) == '\n' || answer.at(i) == '\0' || answer.at(i) == ' ' || answer.at(i) == '\r') {
		count_words.push_back(i);
		number_of_words++;
	}// lưu lại vị trí các khoảng cách, lưu cả vị trí đầu và cuối, chẳng hạn "aa" thì lưu {-1,2}
	ss << answer.at(i);

	Letters* letter = new Letters(ss.str());
	if (answer.at(i) == '\n' || answer.at(i) == '\0' || answer.at(i) == ' ' || answer.at(i) == '\r')
		letter->setStatus(ON);
	letter->setCollidable(false);
	//letter->setColor(D3DCOLOR_XRGB(0,255,255));
	letter->setID(i);
	letter->off();
	letters.push_back(letter);
}

void Quiz::arrangeLetter() {
	std::list<Letters*>::iterator iter;
	letters.clear();
	int strlen = answer.length();
	if (strlen <= 0 ) return;
	for(int i = 0;i<strlen;i++) {
		addLetter(i);
	}
	number_of_words++; //do mặc định khởi tạo là 0 nên phải +1
	count_words.push_back(strlen); //lưu ký tự kết thúc
	iter = letters.begin();
}

void Quiz::inputLog() {
	ifstream log("log.txt");
	if (!log.is_open()) {
		return;
	}
	else {
		int buffer;
		vector<int>::iterator iter;
	   	while(!log.eof()) {
			log >> buffer;
			iter = find(recent.begin(),recent.end(),buffer);
			if (iter != recent.end()) continue;
			recent.push_back(buffer);
			int a = recent.size();
			if (recent.size() > 100) {
				recent.erase(recent.begin());
			}
		}
	}
	log.close();
}

void Quiz::outputLog() {
	ofstream log("log.txt");
	vector<int>::iterator iter;
	for (iter = recent.begin(); iter < recent.end(); iter++)
		log << *iter << "\n";
	log.close();
}

int Quiz::inputQuiz(int x,int numberPlayer){
   CComPtr<IStream> pFileStream;
   CComPtr<IXmlReader> pReader;

   if (Utils::xmlopen(&pFileStream,&pReader,L"file.xml") == false) return -1;

   int i = Utils::xmlcount(pReader,pFileStream,L"entry");

   Utils::xmlrewind(pReader,pFileStream);

   int num_player = numberPlayer;
   if (x> (num_player + 1)) x = num_player + 1; // biến x để chọn độ khó, số độ khó bằng số bàn chơi = num_player + 1. lớn hơn thì auto set khó nhất.
   int interval = i/(num_player+1);
   int k; //biến chạy vòng lặp
   std::vector<int> randomNumber;
   int tempxleft = x, tempxright = x, tempx = x; //lưu cấp độ khó vào biến temp để tránh thay đổi giá trị ban đầu
   int j;
   rmkrandom:
   for (k = tempx*interval; k < (tempx+1)*interval; k++) { //chạy vong lặp này để tạo mảng các quiz có thể chọn dc, từ đó lấy random
	   vector<int>::iterator iter;
	   iter = find(recent.begin(),recent.end(),k);
	   if (iter == recent.end()) {
		   randomNumber.push_back(k);
	   }
   }
   if (randomNumber.empty() == true) {
	   if ((abs(x-tempxleft) >= abs(x-tempxright)) && (tempxleft >= 0)) { //chọn lân cận trái
			tempxleft--;
			if (tempxleft < 0) goto right;
	   		tempx = tempxleft;
	   }
	   else { //chọn lân cận phải
		   right:
		   tempxright++; 
		   tempx = tempxright;
	   }

	   if ((tempxleft < 0) && (tempxright > num_player)) { // các câu hỏi đã chọn hết --> xóa log.
		   recent.erase(recent.begin(),recent.end());
		   recent.clear();
		   tempx = x;	tempxleft = x;	tempxright = x;
	   }

	   goto rmkrandom; //chọn lại
   }
   else {
		srand ( time(NULL) );
		j = rand() % randomNumber.size();
		j = randomNumber.at(j);
		recent.push_back(j);
		if (recent.size() > 100) {
			recent.erase(recent.begin());
		}
		outputLog();
		randomNumber.erase(randomNumber.begin(),randomNumber.end()); //sau khi đã chọn dc quiz thì giải phóng mảng random
		randomNumber.clear();
   }

   Utils::xmlat(pReader,pFileStream,j,L"Question",&question);
   Utils::xmlat(pReader,pFileStream,j,L"Answer",&answer);
   return 0;
}

/*
chiều dài nút, chiều cao nút. x1,y1 x2,y2 là vùng mà ta muốn sắp xếp ô chữ trong đó
*/
void Quiz::setQuizPos(double x1, double y1, double x2, double y2) {
	double xo = x1,yo = y1,fx,fy;
	double width_button = letters.front()->getWidth() * letters.front()->getScale();
	double height_button = letters.front()->getHeight() * letters.front()->getScale();
	int max_words_in_row = (x2-x1)/(width_button+5);
	int max_rows = (y2-y1)/(height_button+5);
	if (letters.size() >= (max_rows*max_words_in_row)) return; //qua' dai`
	/*
	xếp từng hàng đầy nhất có thể cho đến khi không xếp dc hơn thì chuyển hàng mới tiếp tục xêp
	xếp từng hàng thì tiện căn giữa từng hàng.
	dừng khi letters.end()
	*/
	int row = 0;
	std::list<Letters*>::iterator iter;
	iter = letters.begin();
	Letters* letter;
	int i = 0;
	letter = *iter;
	fy = yo;
	i=0;
	while (iter != letters.end()) {
		int k,j;
		if (row > 0) {iter++;}
		k = count_words[i] + 1;
		for (j=i; j<=number_of_words; j++) 
			if ((count_words[j] - count_words[i]) >= max_words_in_row+2) { //gioi han max_words_in_row ky tu 1 hang`, + 2 vi` co 2 khoang trong'
				break;
			}
		i = j-1;
		fx = xo+(width_button+5)*(((max_words_in_row-(count_words[i] - k))%max_words_in_row)/2);
		
		int p = count_words[i];
		while (iter != letters.end() && k < count_words[i]) {
			for (j=0;j<i;j++)
				if (k==count_words[j]) {

					goto cont;
				}
			letter = *iter;
			letter->setPosition(fx,fy);
			cont:
			fx = fx + width_button + 5;
			k++;
			++iter;
		}
		row++;
		fy = fy + height_button + 5;
	}
	//xong sắp xếp ô chữ, tiếp theo sẽ là tính toàn vị trí đặt ô chữ đã săp xếp trong khu vực x1,y1 x2,y2 phù hợp
	//i = row;
	double delta_y = (double)(y2-fy)/2;
	iter = letters.begin();
	while (iter != letters.end()) {
		letter = *iter;
		letter->setY(letter->getY() + delta_y);
		++iter;
	}
}

void Quiz::drawQuiz() {
	std::list<Letters*>::iterator iter;
	iter = letters.begin();
	Letters* letter;
	while (iter != letters.end()) {
		letter = *iter;
		string ss = letter->getLabel();
		if ((ss.compare("\n") == 0) || (ss.compare("\0") == 0) || (ss.compare("\r") == 0) || (ss.compare(" ") == 0)) {iter++;continue;}
		letter->draw();
		//g_engine->addEntity(button);

		iter++;
	}
}

void Quiz::reset() {
	std::list<Letters*>::iterator iter;
	iter = letters.begin();

	while (iter != letters.end()) {
		delete(*iter);
		iter = letters.erase(iter);
		++iter;
	}
	letters.clear();
	number_of_words = 0;
	if (count_words.empty() != false) {
		count_words.erase(count_words.begin(),count_words.end());
		count_words.clear();
		count_words.push_back(-1);
	}
}

void Quiz::change() {
	reset();
	arrangeLetter();
	setQuizPos(this->getX(),this->getY(),this->getX() + this->getWidth(),this->getY() + this->getHeight());
}

bool Quiz::check(string panswer, int *result) {
	if (panswer.compare(answer) == 0) {
		*result = 0;
		return true;
	}
	*result = 0;
	std::transform(panswer.begin(), panswer.end(), panswer.begin(), ::toupper); //qui đổi sang CAP để tránh lỗi
	std::list<Letters*>::iterator iter;
	iter = letters.begin();
	Letters* letter;
	while (iter != letters.end()) {
		letter = *iter;
		if (panswer.compare(letter->getLabel()) == 0) {
			(*result)++;
			letter->on();
		}
		iter++;
	}
	if (*result > 0) return true;
	else return false;
}

bool Quiz::isFinish() {
	bool result = true;
	std::list<Letters*>::iterator iter;
	iter = letters.begin();
	Letters* letter;
	while (iter != letters.end()) {
		letter = *iter;
		string ss = letter->getLabel();
		if ((ss.compare("\n") == 0) || (ss.compare("\0") == 0) || (ss.compare("\r") == 0) || (ss.compare(" ") == 0)) {iter++;continue;}
		if(letter->getStatus() == OFF) {
			result = false;
			break;
		}
		iter++;
	}
	return result;
}