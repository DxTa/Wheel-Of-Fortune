#ifndef H_LETTER_H
#define H_LETTER_H

#include "Engine/Advanced2D.h"
using namespace Advanced2D;
using namespace std;

enum L_STATUS {OFF,ON};

class Letters : public Sprite {
private:
	//int value;
	int status;
	string label;
	Texture *image_off;
	Texture *image_on;
	//void (*callback)();
public :
	Letters(string );
	~Letters();
	inline int getStatus() const {return status;}
	string getLabel() const {return label;}

	//void setCallback(void(*function)()) {callback = function;}
	//void setStatus(int sta) {status = sta;}
	//void setLabel(string pLabel) {label = pLabel;}
	void off(); //chưa lật
	void on(); //lật ô chữ
	void release();
};

#endif