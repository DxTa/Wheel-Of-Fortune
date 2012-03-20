#include "Letter.h"

Letters::Letters(string plabel) : Sprite() {
	label = plabel;
	status = OFF;
	//checkposition = false;
	setScale(1.0f);
	setCollisionMethod(COLLISION_RECT);
	//setObjectType(BUTTON);

	ostringstream ss1,ss2;
	image_off = new Texture();
	//ss1 << "letters_off" <<".png";
	ss1 << "button/" << label << "_normal" <<".png";
	image_off->Load(ss1.str());
	
	image_on = new	Texture();
	//ss2 << "button/"<< label << "_on" <<".png";
	ss2 << "button/" << label << "_pressed" <<".png";
	image_on->Load(ss2.str());
	//callback = NULL;
	off();
}

Letters::~Letters() {
	Sprite::~Sprite();
	if(image_off!=NULL)
		image_off->Release();
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
	if(image_on!=NULL)
		image_on->Release();
	setAlive(false);
}