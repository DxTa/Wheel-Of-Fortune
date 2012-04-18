#include "Player.h"

int Player::numberPlayer = 0;
int Player::currentPlayer =0;

Player::Player() : Sprite() {
	numberPlayer++;
	stringstream ss;
	ss << numberPlayer;
	name = "Player" + ss.str();
	total_score = 0;
	score = 0;
	turn_left = 3;
	setStatus(AWAIT);
	turn_gift = 0;
	TossUp = 0;
	gift = "";
	specialGift = "";
	ostringstream ss2;
	ss2 << "source/player/" << name << "_on.png";
	loadImage(ss2.str());
}

Player::Player(string na) : Sprite() {
	numberPlayer++;
	total_score = 0;
	score = 0;
	name = na;
	setStatus(AWAIT);
	turn_left = 3;
	turn_gift = 0;
	TossUp = 0;
	gift = "";
	specialGift = "";
	ostringstream ss;
	ss << "source/player/" << name << "_on.png";
	loadImage(ss.str());
}

int Player::spin(Wheel* wheel) {
	if(getStatus() == Player::LOSED)
		return -1;
	if(wheel->spin() == Wheel::STOP) {
		if(getStatus() == SPINNING) {
			int out = wheel->getTossUp();
			if(wheel->getObjectType() == Wheel::WHEEL_POS_SPECIAL)
				return out;
			switch(out) {
			case Wheel::G_100 :
			case Wheel::G_100_1 :
			case Wheel::G_100_2 :
				TossUp = 100;
				setStatus(Player::READY_TO_ANSWER);
				break;
			case Wheel::G_200:
			case Wheel::G_200_1:
			case Wheel::G_200_2:
				TossUp = 200;
				setStatus(Player::READY_TO_ANSWER);
				break;
			case Wheel::G_300 :
			case Wheel::G_300_1 :
			case Wheel::G_300_2:
			case Wheel::G_300_3 :
				TossUp = 300;
				setStatus(Player::READY_TO_ANSWER);
				break;
			case Wheel::G_400 :
			case Wheel::G_400_1:
				setStatus(Player::READY_TO_ANSWER);
				TossUp = 400;
				break;	
			case Wheel::G_500 :
				TossUp = 500;
				setStatus(Player::READY_TO_ANSWER);
				break;
			case Wheel::G_600 :
			case Wheel::G_600_1 :
				TossUp = 600;
				setStatus(Player::READY_TO_ANSWER);
				break;
			case Wheel::G_700 :
				TossUp = 700;
				setStatus(Player::READY_TO_ANSWER);
				break;
			case Wheel::G_800:
				TossUp = 800;
				setStatus(Player::READY_TO_ANSWER);
				break;
			case Wheel::G_900 :
				TossUp = 900;
				setStatus(Player::READY_TO_ANSWER);
				break;
			case Wheel::G_BANKRUPT :
			case Wheel::G_BANKRUPT_1 :
				TossUp = Wheel::G_BANKRUPT;
				break;
			case Wheel::G_LOSEATURN :
			case Wheel::G_LOSEATURN_1 :
				TossUp = Wheel::G_LOSEATURN;
				break;
			case Wheel::G_FREEATURN :
				TossUp = Wheel::G_FREEATURN;
				setStatus(Player::READY_TO_ANSWER);
				break;
			case Wheel::G_GIFT :
				TossUp = Wheel::G_GIFT;
				break;
			}
		}
		return TossUp;
	}
	else {
		TossUp = 0;
		setStatus(Player::SPINNING);
		return Player::SPINNING;
	}
}

string Player::answer(Keyboard* keyboard,Quiz* quiz) {
	static int count_full = 0;
	static int result_full = 0;
	if(getStatus() == LOSED) {
		count_full = 0;
		result_full = 0;
		quiz->setClearTemp(true);
		quiz->indicator(-1);
		keyboard->loadState();
		return "";
	}
	if(getStatus() == READY_TO_ANSWER) {
		string ss;
		keyboard->setStatus(Keyboard::AVAILABLE);
		ss = keyboard->chose();
		if(ss!= "") {
			setStatus(PLAYING);
			int result = 0;
			if(quiz->check(ss,&result)) {
				turn_left = 3;
				if((TossUp > 0) && (TossUp != Wheel::G_FREEATURN))
					score += TossUp*result;
				if(TossUp == Wheel::G_FREEATURN)
					turn_gift++;
			}
			else 
				end_play();
		}
		return ss;
	}
	if(getStatus() == READY_TO_FULL_ANSWER) {
		string ss;
		keyboard->setStatus(Keyboard::AVAILABLE);
		ss = keyboard->chose();
		keyboard->setStatus(Keyboard::WAIT);
		bool check;
		quiz->setClearTemp(false);
		quiz->indicator(count_full);
		if(ss!= "") {
			quiz->setLetter(count_full,ss);
			quiz->indicator(count_full+1);
			setStatus(READY_TO_FULL_ANSWER);
			check = quiz->check(ss,count_full);
			if(check == true) 
				result_full++;
			count_full++;
			if(count_full == (quiz->getSize())) {
				if(result_full == count_full) {
					this->winStage();
					quiz->openAll();
				}
				else {
					end_play(LOSED);
				}
				count_full = 0;
				result_full = 0;
				quiz->setClearTemp(true);
				keyboard->loadState();
				keyboard->setStatus(Keyboard::UNAVAILABLE);
			}
		}
		return ss;
		
	}
	if(getStatus() == BEGIN_SPECIAL) {
		static int turnguess = 0;
		string ss;
		keyboard->setStatus(Keyboard::AVAILABLE);
		ss = keyboard->chose();
		keyboard->setStatus(Keyboard::WAIT);
		if(ss!= "") {
			setStatus(BEGIN_SPECIAL);
			int result = 0;
			quiz->check(ss,&result);
			turnguess++;
			if(turnguess >= quiz->getAnswer().length()/5)
				status = Player::FULL_SPECIAl;
			quiz->indicator(0);
		}
		return ss;
	}
	if(getStatus() == FULL_SPECIAl) {
		string ss;
		keyboard->setStatus(Keyboard::AVAILABLE);
		ss = keyboard->chose();
		keyboard->setStatus(Keyboard::WAIT);
		static int count=0;
		bool check;
		quiz->setClearTemp(false);
		static int result = 0;
		quiz->indicator(count);
		if(ss!= "") {
			quiz->setLetter(count,ss);
			quiz->indicator(count+1);
			setStatus(FULL_SPECIAl);
			check = quiz->check(ss,count);
			if(check == true) 
				result++;
			count++;
			if(count == (quiz->getSize())) {
				if(result == count) {
					this->setStatus(WIN_SPECIAL);
					quiz->openAll();
				}
				else {
					this->setStatus(LOSED_SPECIAL);
					quiz->indicator(0);
				}
				count = 0;
				result = 0;
				quiz->setClearTemp(true);
				keyboard->setStatus(Keyboard::WAIT);
			}
		}
		return ss;
	}
	return "";
}

void Player::end_play(int pstatus) {
	if(pstatus == LOSED) {
		setStatus(LOSED);
		currentPlayer += 1;
		if(currentPlayer > numberPlayer)
			currentPlayer = 1;
		image->Release();
		image = new Texture();
		ostringstream ss;
		ss << "source/player/" << name << "_off.png";
		loadImage(ss.str());
		return;
	}
	if(turn_gift == 0) {
		if((pstatus == Wheel::G_BANKRUPT) || (pstatus == Wheel::G_LOSEATURN))
			;
		else 
			--turn_left;
		if(turn_left <= 0)
			setStatus(LOSED);
		else {
			setStatus(AWAIT);
		}
		currentPlayer += 1;
		if(currentPlayer > numberPlayer)
			currentPlayer = 1;
		return;
	}
	else 
		turn_gift--;
}

void Player::reset() {
	setStatus(AWAIT);
	image->Release();
	image = new Texture();
	ostringstream ss;
	ss << "source/player/" << name << "_on.png";
	loadImage(ss.str());
	turn_gift = 0;
	turn_left = 3;
	setScore(0);
}

void Player::winStage() {
	if(getStatus()!= WIN_STAGE) {
		setStatus(WIN_STAGE);
		if(score == 0)
			total_score += 300;
		else
			total_score += score;
		setScore(0);
	}
}

void Player::draw(Font* font) {
	Sprite::draw();
	showScore(font);
	showGift(font);
}

void Player::showScore(Font* font) {
	ostringstream ss1,ss2,ss3;
	ss1 << this->getTotalScore();
	font->setRotation(g_engine->math->toRadians(20));
	string temp = ss1.str();
	for(int i = 0 ;i<temp.size();i++) {
		if((temp.at(i) == '1'))
			temp.insert(i+1," ");
	}
	if(ss1.str().size() >= 4)
		font->Print(this->getPosition().getX()+this->getWidth()/3 * this->getScale(),this->getPosition().getY()+50,temp,D3DCOLOR_XRGB(255,33,22));
	else
		font->Print(this->getPosition().getX()+this->getWidth()/2 * this->getScale(),this->getPosition().getY()+50,temp,D3DCOLOR_XRGB(255,33,22));
	ss2 << "_____";
	font->Print(this->getPosition().getX()+this->getWidth()/3 * this->getScale(),this->getPosition().getY()+60,ss2.str(),D3DCOLOR_XRGB(255,33,22));
	ss3	<< this->getScore();
	temp = ss3.str();
	for(int i = 0 ;i<temp.size();i++) {
		if((temp.at(i) == '1'))
			temp.insert(i+1," ");
	}
	if(temp.size() >=4)
		font->Print(this->getPosition().getX()+this->getWidth()/3 * this->getScale(),this->getPosition().getY()+85,temp,D3DCOLOR_XRGB(255,33,22));
	else 
		font->Print(this->getPosition().getX()+this->getWidth()/2 * this->getScale(),this->getPosition().getY()+85,temp,D3DCOLOR_XRGB(255,33,22));
}

void Player::showGift(Font* font) {
	ostringstream ss,ss1;
	if(turn_gift > 0)
		ss << "FREETURN";
	if(turn_gift > 1)
		ss << " X " << turn_gift;
	font->setRotation(g_engine->math->toRadians(20));
	font->Print(this->getPosition().getX()+this->getWidth() * this->getScale(),this->getPosition().getY()+120,ss.str(),D3DCOLOR_XRGB(255,33,22));
	if(gift!= "")
		font->Print(this->getPosition().getX()+this->getWidth()/3 * this->getScale(),this->getPosition().getY()+140,"G I F T :",D3DCOLOR_XRGB(255,33,22));
	int count = 0;
	string temp;
	for(int i = 0;i<gift.size();i++) {
		if(gift.at(i) != ' ') {
			temp += gift.at(i);
		}
		if(gift.at(i) == ' ') {
			count++;
			font->Print(this->getPosition().getX()+this->getWidth()/2.5 * this->getScale()+5*count,this->getPosition().getY()+150+22*count,temp,D3DCOLOR_XRGB(255,33,22));
			temp.clear();
		}
		//font->Print(this->getPosition().getX()+this->getWidth()/3 * this->getScale(),this->getPosition().getY()+160+20*count,gift,D3DCOLOR_XRGB(255,33,22));
	}
}

void Player::showPlay(Font* font) {
	font->setScale(1.5);
	font->setRotation(g_engine->math->toRadians(0));
	font->Print(this->getPosition().getX()+this->getWidth()/3 * this->getScale(),this->getPosition().getY()-15,"X",D3DCOLOR_XRGB(255,33,255));
}

void Player::loseGame() {
	setStatus(LOSED);
	image->Release();
	image = new Texture();
	ostringstream ss;
	ss << "source/player/" << name << "_off.png";
	loadImage(ss.str());
}