#include "wheel.h"
#include "math.h"

Wheel::Wheel() : Sprite() {
	speed = 0;
	power_bar = new Sprite();
	power_bar->loadImage("powerbar.png");
	power_bar->setSize(904,62);
	power_bar->setScale(0.5);
	power_bar->setCurrentFrame(0);
	power_bar->setCollidable(false);
	power_bar->setVisible(true);
	power_bar->setPosition(513,400);
	power_bar->setTotalFrames(21);
	power_bar->setColumns(1);
			
	friction = 0.0002;
	angle = 0;
	s=0;
	direction = NONE;
	teng = "pop";
	g_engine->audio->Load("sound.mp3",teng);
	start_spin  = false;
	setStatus(STOP);
}

Wheel::Wheel(double sp,double fric) : Sprite() {
	speed = sp;
	friction = fric;
	angle = 0;
	s = 0;
	direction = NONE;
	teng = "pop";
	g_engine->audio->Load("sound.mp3",teng);
	start_spin = false;
	setStatus(STOP);
}

int Wheel::spin() {
	if (start_spin  == false) goto fin;

	double speed_temp;
	speed_temp =speed;
	speed = speed - friction;
	if(speed > 0) 
		s = (speed_temp*speed_temp - speed*speed)/(2*friction);
	if(speed <= 0)
		start_spin = false;

	if(start_spin == true) {
		if(direction == LEFT) 
			angle -= s;
		else if(direction == RIGHT) {
			angle += s;
		}
	}
	fin:
	this->setRotation(this->getAngle());
	if (isSlices15() == true) {
		if(teng!= "")
			g_engine->audio->Play(teng);
	}

	if(speed > 0) {
		setStatus(SPINNING);
		return SPINNING;
	}
	else {
		if(getStatus() == Wheel::WAIT) {
			return Wheel::WAIT;
		}
		else {
			setStatus(Wheel::STOP);
			return Wheel::STOP;
		}
	}
}

bool Wheel::isSlices15() {
	double angle_in_degree = g_engine->math->toDegrees(this->getAngle());
	int times = (int)(angle_in_degree/15);
	return (fabs(angle_in_degree - times*15) < fabs(g_engine->math->toDegrees(s)));
}

int Wheel::getTossUp() {
	double angle_in_degree = g_engine->math->toDegrees(this->getAngle());
	angle_redundance = (this->getAngle() - (((int)((this->getAngle())/g_engine->math->toRadians(360)))*g_engine->math->toRadians(360)));
	double delta_15 = fabs(g_engine->math->toDegrees(angle_redundance) - std::floor(g_engine->math->toDegrees(angle_redundance)/15)*15);
	if (delta_15 < 1.5 || (15 - delta_15) < 1.5)  {
		if (direction == LEFT) {
			angle_redundance -= g_engine->math->toRadians(2.5);
			angle -= g_engine->math->toRadians(2.5);
		}
		else {
			angle_redundance += g_engine->math->toRadians(2.5);
			angle+= g_engine->math->toRadians(2.5);
		}
		this->setRotation(this->getAngle());
	}
	double slices = angle_redundance/g_engine->math->toRadians(15);
	int position = (int)(std::floor(slices));
	if (position < 0) position = 24 + position;
	return (Wheel::TossUp = position);
}

void Wheel::setOR(double x, double y, double r) {
	ox = x;
	oy = y;
	or = r;
	max_speed = (or * 3.14)/8000;
	speed_interval = max_speed/21;
}

void Wheel::update() {
	if(this->isHolding() ==false) {
		if (this->getSpeed() > 0) {
			if (this->getStartSpin() == false) 
				this->setStartSpin(true);
		}
	}
}

void Wheel::updateDirection(double delta_x,double delta_y,Sprite* cursor) {
	if(this->isHolding()==true) {
		double a = delta_x*(cursor->getY() - delta_y - this->getOY()) - delta_y*(cursor->getX() - delta_x - this->getOX());
		if (a > 0) this->setDirection(Wheel::LEFT);
		else if ( a < 0) this->setDirection(Wheel::RIGHT);
		else this->setDirection(Wheel::NONE);
	}
}

void Wheel::updateMouseButton() {
	if((this->isCheckPosition() == true) && (this->getStartSpin() == false)&& ((getStatus() == Wheel::WAIT) || (getStatus() == Wheel::SPINNING))) {
		this->setHolding(true);
	}
	else this->setHolding(false);
}

void Wheel::updateMouseMove(double delta_x,double delta_y,double fx,double fy) {
	
	float d = (float)sqrt(pow((fx - this->getOX()),2) + pow((fy - this->getOY()),2));
	if (d > this->getR()) {
		this->setHolding(false);
	}
	if (this->isHolding() == true) {
		double x = (delta_x * (delta_y + this->getOY() - fy) + delta_y * (delta_x + this->getOX() - fx)*(-1))/
			(sqrt(delta_x * delta_x + delta_y * delta_y)*sqrt(pow((delta_x + this->getOX() - fx),2) + pow((delta_y + this->getOY() - fy),2)));
		x = x*sqrt(delta_x * delta_x + delta_y * delta_y);
		x = x/sqrt(pow((fx - delta_x - this->getOX()),2) + pow((fy - delta_y - this->getOY()),2));
		this->setS(fabs(atan(x)));
		if (delta_x < 0)
			this->setAngle(this->getAngle() - fabs(atan(x)));
		else
			this->setAngle(this->getAngle() + fabs(atan(x)));
		double time = Utils::timecount();
		if(this->getS()*10/time < ((2*3.14 + friction)/100)) 
			this->setSpeed(((2*3.14 + friction)/100));
		else 
			this->setSpeed(this->getS()*10/time);
	}
}

void Wheel::updateMousePosition(Sprite* cursor) {
	float d = (float)sqrt(pow((cursor->getX() - this->getOX()),2) + pow((cursor->getY() - this->getOY()),2));
	if (d <= this->getR()) {
		this->setCheckPosition(true);
	}
	else this->setCheckPosition(false);
}

void Wheel::drawPowerBar() {
	int ratio = speed/speed_interval;
	if (ratio > 20) ratio = 20;
	power_bar->setCurrentFrame(ratio);
	power_bar->draw();
}