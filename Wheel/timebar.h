#ifndef H_TIMEBAR_H
#define H_TIMEBAR_H

#include "Engine/Advanced2D.h"
using namespace Advanced2D;
class Timebar : public Sprite {
private:
	Timer timecheck;
	double timeguess;
	double timeanswer;
	double timespecial;
public :
	Timebar();
	void setTimeGuess(double ms) {timeguess = ms;}
	void setTimeAnswer(double ms) {timeanswer = ms;}
	void setTimeSpecial(double ms) {timespecial = ms;}
	bool guessTimeUp();
	bool answerTimeUp();
	bool specialTimeUp();
};

#endif