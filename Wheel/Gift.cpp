#include "Gift.h"


Gift::Gift(string plabel) : Button("Gift_off") {
	ostringstream ss;
	label = plabel;
	ss << "source/gift/" << plabel << ".png";
	image_pressed->Load(ss.str());
}

