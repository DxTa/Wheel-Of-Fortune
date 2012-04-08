#include "Gift.h"


Gift::Gift(string plabel) : Button("Gift_off") {
	ostringstream ss;
	ss << "button/" << plabel << ".png";
	image_pressed->Load(ss.str());
}

Gift::~Gift() {
	Button::~Button();
}
