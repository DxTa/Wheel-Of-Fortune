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
}

Player::Player(string na) : Sprite() {
	numberPlayer++;
	total_score = 0;
	score = 0;
	name = na;
	setStatus(AWAIT);
	turn_left = 3;
	turn_gift = 0;
}

int Player::spin(Wheel* wheel) {
	if(getStatus() == Player::LOSED)
		return -1;
	if(wheel->spin() == Wheel::STOP) {
		TossUp = wheel->getTossUp();
		if(getStatus() == SPINNING) {
			/*switch(TossUp) {
			case Wheel::G_300_0 :
				setStatus(Player::READY_TO_ANSWER);
				break;
			}*/
			setStatus(Player::READY_TO_ANSWER);
		}
		return TossUp;
	}
	else {
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
				switch(TossUp) {
				default:
					score += TossUp*result;
					break;
				}
			}
			else 
				end_play();
		}
		return ss;
	}
	else
		return "";
}

void Player::end_play() {
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
	}
	else 
		turn_gift--;
}
