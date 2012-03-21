#ifndef H_QUIZ_H
#define H_QUIZ_H

#include "Engine/Advanced2D.h"
#include "Letter.h"
#include <atlbase.h>
#include "xmllite.h"
#include <iostream>
#include <sstream>
#include "utils.h"
#include <vector>
#include <string.h>
#include "Player.h"
#include <stdlib.h>

using namespace std;
using namespace Advanced2D;
using namespace Utils;

class Quiz : public Sprite {
private:
	string question;
	string answer;
	std::vector<int> count_words; //lưu index vị trí bắt đầu kết thúc của mỗi từ (index các khoảng trống)
	int number_of_words; //số từ trong answer
	std::list<Letters*> letters; //lưu từng ký tự answers trong này
public:
	Quiz();
	Quiz(string,string);

	string getQuestion() const {return question;}
	string getAnswer() const {return answer;}
	void setQuestion(string pquestion) {question = pquestion;}
	void setAnswer(string panswer) {answer = panswer;}

	void setQuizPos(double width_button, double height_button, double x1, double y1, double x2, double y2);
	void drawQuiz();
	int inputQuiz(int i); // biến i để chọn độ khó, số độ khó bằng số bàn chơi = num_player + 1. lớn hơn thì auto set khó nhất.
	void addLetter(int );
	void arrangeLetter();
	void change();
	void reset();
	bool check(string panswer, int *result); //nếu đoán đúng cả dãy thì result=0,true / nếu đoán dc 1 chữ thì result = số chữ trong dãy,true
};

#endif