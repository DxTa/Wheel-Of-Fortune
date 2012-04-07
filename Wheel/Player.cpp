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
}

int Player::spin(Wheel* wheel) {
	if(getStatus() == Player::LOSED)
		return -1;
	if(wheel->spin() == Wheel::STOP) {
		if(getStatus() == SPINNING) {
			int out = wheel->getTossUp();
			setStatus(Player::READY_TO_ANSWER);
			switch(out) {
			case Wheel::G_100 :
			case Wheel::G_100_1 :
			case Wheel::G_100_2 :
				TossUp = 100;
				break;
			case Wheel::G_200:
			case Wheel::G_200_1:
			case Wheel::G_200_2:
				TossUp = 200;
				break;
			case Wheel::G_300 :
			case Wheel::G_300_1 :
			case Wheel::G_300_2:
			case Wheel::G_300_3 :
				TossUp = 300;
				break;
			case Wheel::G_400 :
			case Wheel::G_400_1:
				TossUp = 400;
				break;	
			case Wheel::G_500 :
				TossUp = 500;
				break;
			case Wheel::G_600 :
			case Wheel::G_600_1 :
				TossUp = 600;
				break;
			case Wheel::G_700 :
				TossUp = 700;
				break;
			case Wheel::G_800:
				TossUp = 800;
				break;
			case Wheel::G_900 :
				TossUp = 900;
				break;
			case Wheel::G_BANKRUPT :
			case Wheel::G_BANKRUPT_1 :
				break;
			case Wheel::G_LOSEATURN :
			case Wheel::G_LOSEATURN_1 :
				break;
			case Wheel::G_FREEATURN :
				break;
			}
			return TossUp;
		}
		
	}
	else {
		if (wheel->isFixxing() == true) return -1;
		setStatus(Player::SPINNING);
		return Player::SPINNING;
	}
}

string Player::answer(Keyboard* keyboard,Quiz* quiz) {
	if(getStatus() == LOSED)
		return "";
	if(getStatus() == READY_TO_ANSWER) {
		string ss;
		keyboard->setStatus(Keyboard::AVAILABLE);
		ss = keyboard->chose();
		if(ss!= "") {
			setStatus(PLAYING);
			int result = 0;
			if(quiz->check(ss,&result)) {
				turn_left = 3;
				if(TossUp > 0)
					score += TossUp*result;
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
		static int count=0;
		bool check;
		quiz->setClearTemp(false);
		static int result = 0;
		quiz->indicator(count);
		if(ss!= "") {
			quiz->setLetter(count,ss);
			quiz->indicator(count+1);
			setStatus(READY_TO_FULL_ANSWER);
			check = quiz->check(ss,count);
			if(check == true) 
				result++;
			count++;
			if(count == (quiz->getSize())) {
				if(result == count) {
					this->winStage();
					quiz->openAll();
				}
				else {
					end_play(LOSED);
				}
				count = 0;
				result = 0;
				quiz->setClearTemp(true);
				keyboard->loadState();
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
		return;
	}
	if(turn_gift == 0) {
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
	turn_gift = 0;
	turn_left = 3;
	score = 0;
}


void Player::winStage() {
	setStatus(WIN_STAGE);
	if(score == 0)
		total_score += 300;
	else
		total_score += score;
	reset();
}
