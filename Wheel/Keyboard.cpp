#include "Keyboard.h"


Keyboard::Keyboard() {
	string ss;
	char i;
	char temp;
	for(i=0;i<26;i++) {
		temp = i+65;
		ss = temp;
		letter[i] = new Button(ss);
		letter[i]->reset();
	}
	setStatus(UNAVAILABLE);
}

Keyboard::~Keyboard() {
	int i;
	for(i=0;i<26;i++) {
		if(letter[i])
			delete letter[i];
	}
}

void Keyboard::setPosition(double x,double y) {
	int i;
	double fx,fy;
	for(i=0;i<26;i++) {
		fx = i%13;
		fy = (int)i/13;
		letter[i]->setPosition(x+ letter[i]->getWidth()*fx,y + letter[i]->getHeight()*fy);
	}
}

void Keyboard::draw() {
	if(this==NULL)
		return;
	int i;
	for(i=0;i<26;i++)
		letter[i]->draw();
}

void Keyboard::reset() {
	if(this==NULL)
		return;
	int i;
	for(i=0;i<26;i++) {
		letter[i]->reset();
	}
}

void Keyboard::updateMouseMove(Cursor* cursor) {
	if(this==NULL)
		return;
	int i;
	for(i=0;i<26;i++) {
		letter[i]->updateMouseButton(cursor,0.7);
		if((status == AVAILABLE) || (status == WAIT))
			letter[i]->setCollidable(true);
		else
			letter[i]->setCollidable(false);
	}
}

string Keyboard::chose() {
	if(this==NULL)
		return "";
	int i;
	string temp;
	if(status == UNAVAILABLE)
		return "";
	if(status == AVAILABLE) {
		for(i=0;i<26;i++) {
			if((letter[i]->isPosition()==true) && (letter[i]->getStatus() != Button::BUTTON_PRESSED)) {
				letter[i]->pressed();
				setStatus(UNAVAILABLE);
				return letter[i]->getLabel();
			}
		}
	}
	return "";
}

void Keyboard::addEntity() {
	if(this==NULL)
		return;
	int i;
	for(i=0;i<26;i++) {
		if(letter[i])
			g_engine->addEntity(letter[i]);
	}
}

void Keyboard::saveState() {
	int i;
	for(i=0;i<26;i++) {
		if(letter[i]->getStatus() == Button::BUTTON_PRESSED)
			state[i] = 1;
		else 
			state[i] = 0;
	}
}

void Keyboard::loadState() {
	int i;
	for(i=0;i<26;i++) {
		if(state[i]==1)
			letter[i]->pressed();
		else 
			letter[i]->reset();
	}
}

void Keyboard::setScale(double scale) {
	for(int i = 0;i<26;i++)
		letter[i]->setScale(scale);
}