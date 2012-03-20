#ifndef H_QUIZ_H
#define H_QUIZ_H

#include "Engine/Advanced2D.h"
#include "button.h"
#include "utils.h"
#include <atlbase.h>
#include "xmllite.h"
#include <iostream>
#include <sstream>
using namespace std;
using namespace Advanced2D;
using namespace Utils;

class Quiz : public Sprite {
private:
	string question;
	string answer;
	std::list<Button*> letters;
public:
	Quiz();
	Quiz(string,string);

	string getQuestion() const {return question;}
	string getAnswer() const {return answer;}

	void setQuestion(string pquestion) {question = pquestion;}
	void setAnswer(string panswer) {answer = panswer;}

	int inputQuiz(int i);
	void addLetter(int );
	void arrangeLetter();
	void update();
	void reset();
};

#endif