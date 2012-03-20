#ifndef H_CURSOR_H
#define H_CURSOR_H

#include "Engine/Advanced2D.h"
using namespace Advanced2D;

class Cursor : public Sprite {
private:
	float delta_x;
	float delta_y;
public :

	void setDeltaX(float px) {delta_x = px;}
	void setDeltaY(float py) {delta_y = py;}

	float getDeltaX() const {return delta_x;}
	float getDeltaY() const {return delta_y;}
};

#endif