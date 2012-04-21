#include "timebar.h"

Timebar::Timebar() : Sprite() {
	this->loadImage("source/gameplay/timebar.png");
	this->setTotalFrames(60);
	this->setSize(489,10);
	this->setColumns(1);
	this->setCurrentFrame(59);
	this->setPosition(g_engine->getScreenWidth()/2-this->getWidth()/2,g_engine->getScreenHeight()/2+100);
	this->setVisible(false);
	timeanswer = 500;
	timespecial = 1000;
}

bool Timebar::guessTimeUp() {
	if(timecheck.stopwatch(timeguess)) {
		this->setCurrentFrame(this->getCurrentFrame()-1);
	}
	if(this->getCurrentFrame()==0)
		return true;
	else
		return false;
}

bool Timebar::answerTimeUp() {
	if(timecheck.stopwatch(timeanswer)) {
		this->setCurrentFrame(this->getCurrentFrame()-1);
	}
	if(this->getCurrentFrame()==0)
		return true;
	else
		return false;
}

bool Timebar::specialTimeUp() {
	if(timecheck.stopwatch(timespecial)) {
		this->setCurrentFrame(this->getCurrentFrame()-1);
	}
	if(this->getCurrentFrame()==0)
		return true;
	else
		return false;
}


