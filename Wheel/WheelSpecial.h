#ifndef H_WHEEL_SPECIAL_H
#define H_WHEEL_SPECIAL_H

#include "wheel.h"
using namespace Advanced2D;
using namespace std;

class WheelSpecial : public Wheel {
public :
	enum {G_CAR,G_TRIP,G_TABLET,G_1mil,G_PHONE,G_TV};
	WheelSpecial();
	int getTossUp();
};

#endif