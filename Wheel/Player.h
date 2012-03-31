#ifndef _H_PLAYER_H
#define _H_PLAYER_H

#include "Engine/Advanced2D.h"
#include "Wheel.h"
#include "Keyboard.h"
#include "Quiz.h"

using namespace Advanced2D;
using namespace	std;

class Player : public Sprite {
	private:
		string name;
		static int numberPlayer;
		static int currentPlayer;
		int total_score;
		int score;
		int TossUp;
		int status;
		int turn_left;
		int turn_gift;
	public :
		enum P_STATUS {AWAIT=100,PLAYING,READY_TO_SPIN,SPINNING,READY_TO_ANSWER,READY_TO_FULL_ANSWER,LOSED,WIN_STAGE};
		Player();
		Player(string );
		~Player() {Sprite::~Sprite();}
		static int getCurrentPlayer() {return currentPlayer;}
		static int getNumPlayer() {return numberPlayer;}
		string getName() const {return name;}
		int getScore() const {return score;}
		int getStatus() const {return status;}
		int getTurnLeft() const {return turn_left;}
		int getTurnGift() const {return turn_gift;}

		static void setCurrentPlayer(int current) {currentPlayer = current > numberPlayer ? 1 : current;}
		static void setNumPlayer(int num) {numberPlayer = num;}
		void setName(string na) {name = na;}
		void setScore(int s) {score = s;}
		void setStatus(P_STATUS sta) {status = sta;}
		void winScore(int sco) {score = sco;}

		void winStage(Quiz*);
		void reset();

		int start_play();
		int spin(Wheel*);
		string answer(Keyboard*,Quiz*);
		string fullanswer(Keyboard*,Quiz*);
		void end_play(int = AWAIT);
};

#endif
