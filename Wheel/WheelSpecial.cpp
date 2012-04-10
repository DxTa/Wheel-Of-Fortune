#include "WheelSpecial.h"

WheelSpecial::WheelSpecial() {
	Wheel::Wheel();
	slice = 60;
}

int WheelSpecial::getTossUp() {
	double angle_in_degree = g_engine->math->toDegrees(this->getAngle());
	angle_redundance = (this->getAngle() - (((int)((this->getAngle())/g_engine->math->toRadians(360)))*g_engine->math->toRadians(360)));
	double delta_slice = fabs(g_engine->math->toDegrees(angle_redundance) - std::floor(g_engine->math->toDegrees(angle_redundance)/slice)*slice);
	double slices = angle_redundance/g_engine->math->toRadians(slice);
	int position = (int)(std::floor(slices));
	if (position < 0) position = 6 + position;
	return (Wheel::TossUp = position);
}

