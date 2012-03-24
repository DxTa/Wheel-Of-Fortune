#include "Letter.h"

Texture* Letters::image_off = new Texture();

Letters::Letters(string plabel) : Sprite() {
	label = plabel;
	status = OFF;
	//checkposition = false;
	setScale(0.7f);
	setCollisionMethod(COLLISION_RECT);
	//setObjectType(BUTTON);

	ostringstream ss;

	static int i = 0;
	if (i == 0) {
		image_off->Load("button/off.png");
		i=1;
	}
	
	image_on = new	Texture();
	//ss2 << "button/"<< label << "_on" <<".png";
	ss << "button/" << label << "_pressed" <<".png";
	image_on->Load(ss.str());
	//callback = NULL;
	off();
}

Letters::~Letters() {
	Sprite::~Sprite();
	//if(image_off!=NULL)
	//	image_off->Release(); //chac' la ko nen neu' dung` 1 anh? duy nhat'
	if(image_on!=NULL)
		image_on->Release();
}

void Letters::off() {
	setImage(image_off);
	status = OFF;
}

void Letters::on() {
	if(status==OFF) {
		setImage(image_on);
		status = ON;
		//if(callback!=NULL)
		//	callback();
	}
}

void Letters::release() {
	Sprite::~Sprite();
	if(image_off!=NULL)
		image_off->Release();
	//if(image_on!=NULL)
	//	image_on->Release();
	setAlive(false);
}