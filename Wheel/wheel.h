#ifndef H_WHEEL_H
#define H_WHEEL_H

#include "Engine/Advanced2D.h"
#include "utils.h"
using namespace std;
using namespace Advanced2D;

class Wheel : public Advanced2D::Sprite {
protected:
	Sprite *power_bar;
	int TossUp;
	int direction;
	int status;
	int slice;

	double speed;
	double max_speed;
	double speed_interval;
	double friction;
	double angle,angle_redundance;
	double s;
	double interval;
	double ox,oy,or;

	bool start_spin;
	bool wheel_holding;
	bool checkposition;
	string teng;

	bool isSlices();

public:
	enum slices {
		G_100,G_300,G_700,G_200,G_100_1,G_800,G_LOSEATURN,G_300_1,G_400,G_600,G_BANKRUPT,
		G_900,G_300_2,G_200_1,G_GIFT,G_100_2,G_400_1,G_LOSEATURN_1,G_FREEATURN,
		G_500,G_300_3,G_BANKRUPT_1,G_600_1,G_200_2
	};
	enum DICRECTION  {LEFT,RIGHT,NONE};
	enum WHEEL_STATUS {STOP=0,SPINNING=1,WAIT};
	enum OBJECTYPE {WHEEL_POS=151,WHEEL_POS_SPECIAL};
	Wheel();
	virtual int getTossUp();
	int getDirection() const {return direction;}
	int getStatus() const {return status;}
	bool getStartSpin() const {return start_spin;}
	bool isCheckPosition() const {return checkposition;}
	bool isHolding() const {return wheel_holding;}
	double getAngle() const {return angle;}
	double getAngleRedundance() const {return angle_redundance;}
	double getS() const {return s;}
	double getOX() const {return ox;}
	double getOY() const {return oy;}
	double getR() const {return or;}
	double getInterval() const {return interval;}
	double getSpeed() const {return speed;}
	string getTeng() const {return teng;}

	void setStatus(int pstatus) {status = pstatus;}
	void setStartSpin(bool stat) {start_spin = stat;}
	void setSpeed(double sp) {speed = sp;}
	void setAngle(double angl) {angle = angl;}
	void setS(double epsilon) {s = epsilon;}
	void setTeng(string pteng) {teng = pteng;}
	void setCheckPosition(bool pos) {checkposition = pos;}
	void setHolding(bool hold) {wheel_holding = hold;}
	void setDirection(int dir) {direction = dir;}
	void setInterval(double i) {interval = i;}
	void setOR(double x, double y, double r);
	void setFriction(double fric) {friction = fric;}
	void drawPowerBar();
	void update();
	void updateDirection(double ,double ,Sprite*);
	void updateMouseButton();
	void updateMouseMove(double,double,double,double);
	void updateMousePosition(Sprite*);

	int spin();
};

#endif