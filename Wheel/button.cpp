#include "button.h"


Button::Button(string plabel) : Sprite() {
	label = plabel;
	status = BUTTON_NORMAL;
	checkposition = false;
//	setScale(0.8f);
	setCollisionMethod(COLLISION_RECT);
	setObjectType(BUTTON);

	ostringstream ss1,ss2;
	image_normal = new Texture();
	ss1 << "source/button/" << label << "_normal" <<".tga";
	image_normal->Load(ss1.str());
	
	image_pressed = new	Texture();
	ss2 << "source/button/"<< label << "_pressed" <<".tga";
	image_pressed->Load(ss2.str());
	callback = NULL;
	reset();
}

Button::~Button() {
	Sprite::~Sprite();
	if(image_normal!=NULL)
		image_normal->Release();
	if(image_pressed!=NULL)
		image_pressed->Release();
}

void Button::reset() {
	setImage(image_normal);
	status = BUTTON_NORMAL;
}

void Button::pressed() {
	if(status==BUTTON_NORMAL) {
		setImage(image_pressed);
		status = BUTTON_PRESSED;
		if(callback!=NULL)
			callback();
		checkpress.sleep(10);
	}
}

void Button::toggle() {
	if(status == BUTTON_NORMAL) {
		setImage(image_pressed);
		status = BUTTON_PRESSED;
		checkpress.sleep(10);
		if(callback!=NULL)
			callback();
		return;
	}
	if(status == BUTTON_PRESSED) {
		setImage(image_normal);
		status = BUTTON_NORMAL;
		checkpress.sleep(10);
		if(callback!=NULL)
			callback();
		return;
	}

}

void Button::toggle(int sta) {
	if(sta == BUTTON_PRESSED) {
		setImage(image_pressed);
		status = BUTTON_PRESSED;
		checkpress.sleep(10);
		return;
	}
	if(sta == BUTTON_NORMAL) {
		setImage(image_normal);
		status = BUTTON_NORMAL;
		checkpress.sleep(10);
		return;
	}

}

void Button::release() {
	Sprite::~Sprite();
	if(image_normal!=NULL)
		image_normal->Release();
	if(image_pressed!=NULL)
		image_pressed->Release();
	setAlive(false);
}

bool Button::updateMouseMove(Cursor* cursor,double scale) {
	bool temp;
	temp = g_engine->collision(cursor,this);
	if(!temp) {
		this->setCheckPosition(false);
		this->setScale(scale);
	}
	return temp;
}