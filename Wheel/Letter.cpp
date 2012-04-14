#include "Letter.h"

Texture* Letters::image_off = new Texture();

Letters::Letters(string plabel) : Sprite() {
	setObjectType(LETTER_PER);
	label = plabel;
	status = OFF;
	setScale(0.7f);
	setCollidable(false);

	ostringstream ss;

	static int i = 0;
	if ((i == 0) || (image_off == NULL)) {
		image_off->Load("button/off.png");
		i=1;
	}
	
	image_on = new	Texture();
	ss << "button/" << label << "_normal" <<".png";
	image_on->Load(ss.str());
	off();
}

Letters::~Letters() {
	Sprite::~Sprite();
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
	}
}

void Letters::release() {
	if(image_off!=NULL)
		image_off->Release();
}
