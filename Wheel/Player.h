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
		static int numberPlaying;
		string gift;
		string specialGift;
		int total_score;
		int score;
		int TossUp;
		int status;
		int turn_left;
		int turn_gift;
	public :
		enum P_STATUS {AWAIT=100,PLAYING,READY_TO_SPIN,SPINNING,READY_TO_ANSWER,READY_TO_FULL_ANSWER,LOSED,WIN_STAGE,BEGIN_SPECIAL,FULL_SPECIAl,WIN_GAME,WIN_SPECIAL,LOSED_SPECIAL};
		enum EMO {NEXT_PLAYER=5000,FULLGUESSTRUE,FULLGUESSWRONG,WTF};
		Player();
		Player(string );
		static int getCurrentPlayer() {return currentPlayer;}
		static int getNumPlayer() {return numberPlayer;}
		string getName() const {return name;}
		int getScore() const {return score;}
		int getTotalScore() const {return total_score;}
		int getStatus() const {return status;}
		int getTurnLeft() const {return turn_left;}
		int getTurnGift() const {return turn_gift;}
		string getSpecialGift() const {return specialGift;}
		string getGift() const {return gift;}

		static void setCurrentPlayer(int current) {currentPlayer = current > numberPlayer ? 1 : current;}
		static void setNumPlayer(int num) {numberPlayer = num;}
		void winScore() {total_score += score;}
		void setScore(int pscore) {score = pscore;}
		void setStatus(P_STATUS sta) {status = sta;}
		void setGift(string g) {gift = gift + g + " ";}
		void setSpecialGift(string g) {specialGift = g;}

		void winStage();
		void loseGame();
		void reset();
		int spin(Wheel*);
		string answer(Keyboard*,Quiz*);
		string fullanswer(Keyboard*,Quiz*);
		void end_play(int = AWAIT);

		void showGift(Font*);
		void showScore(Font*);
		void showPlay(Font*);

		void draw(Font*);
};

#endif
