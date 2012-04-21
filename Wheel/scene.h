#ifndef H_SCENE_H
#define H_SCENE_H

#include "Menu.h"
#include "wheel.h"
#include "WheelSpecial.h"
#include "Keyboard.h"
#include "Quiz.h"
#include "Gift.h"
#include "Cursor.h"
#include "Player.h"
#include "Engine/Advanced2D.h"

namespace Scene {
	std::list<Player*> playerlist;
	std::list<Gift*> giftlist;

	std::list<Button*> giftconfig;
	std::list<Button*> playerconfig;

	Timer timecheck;
	Timer buttoncheck;
	Timer arrorcheck;
	Timer emotioncheck;
	Timer menucheck;

	Font* system12;
	
	Texture* background_image;
	Sprite* background;
	Sprite* score_background;
	Sprite* timebar;
	Sprite* arrow;

	Player* g_player;
	Wheel* wheel;
	WheelSpecial* wheel_special;
	Keyboard* keyboard;
	Quiz* quiz;
	Menu* menu;
	Cursor* cursor;
	

	Button* PlayerMenu_Spin;
	Button* PlayeMenu_Guess;
	Button* Next_Stage;
	Button* button_ready;
	Button* button_ok;
	Button* startGame;
	Button* back_button;

	int openspecial = 0;
	int numGift;
	int numPlayer;
	int timeGuess;
	string chose,ss;
	string specialGift;

	bool sceneplay_start = false;
	bool sceneplay_on = false;
	bool scenePlayerMenu_start = false;
	bool scenePlayerMenu_on = false;
	bool scenePlayerGift_start = false;
	bool scenePlayerGift_on = false;
	bool sceneSpecial_start = false;
	bool sceneSpecial_on = false;
	bool sceneMain_start = false;
	bool sceneMain_on = false;
	bool sceneNewGame_start = false;
	bool sceneNewGame_on = false;
	bool sceneHelp_start = false;
	bool sceneHelp_on = false;
	bool checkNextStage = false;
	bool cleartemp= true;

	void sceneplay();
	void sceneSpecial();
	void sceneMain();
	void sceneNewGame();
	void sceneHelp();

	void release();

	bool isEndStage();
	bool isNextStage();
	void nextStage();

	enum {CURSOR=150,EMOTION_GIFT = 11000,NEXT_STAGE,GUESSAWORD,LOSEALL,OVERTIME,NOTIFY_GIFT,NEWGAME_TITTLE};
}

void g_exit() {
	g_engine->Close();
}

void play2special() {
	Scene::sceneplay_start = false;
	Scene::sceneplay_on = false;
	Scene::scenePlayerMenu_start  = false;
	Scene::scenePlayerMenu_on = false;
	Scene::sceneSpecial_start = true;
}

void config2play() {
	Scene::sceneplay_start = true;
	Scene::sceneNewGame_start = false;
	Scene::sceneNewGame_on = false;
	Player::setCurrentPlayer(1);
}

void spin() {
	Scene::wheel->setStatus(Wheel::WAIT);
	Scene::keyboard->setStatus(Keyboard::WAIT);
	Scene::scenePlayerMenu_start  = false;
	Scene::scenePlayerMenu_on = false;
	Scene::PlayerMenu_Spin->setVisible(false);
	Scene::PlayerMenu_Spin->setCollidable(false);
	Scene::PlayeMenu_Guess->setVisible(false);
	Scene::PlayeMenu_Guess->setCollidable(false);
}

void guess() {
	Scene::timebar->setVisible(true);
	Scene::keyboard->saveState();
	Scene::g_player->setStatus(Player::READY_TO_FULL_ANSWER);
	Scene::scenePlayerMenu_start  = false;
	Scene::scenePlayerMenu_on = false;
	Scene::PlayerMenu_Spin->setVisible(false);
	Scene::PlayerMenu_Spin->setCollidable(false);
	Scene::PlayeMenu_Guess->setVisible(false);
	Scene::PlayeMenu_Guess->setCollidable(false);	
}

void readyspecial() {
	Scene::wheel_special->setStatus(Wheel::WAIT);
	Scene::keyboard->reset();
	Scene::keyboard->setStatus(Keyboard::WAIT);
}

void showButtonOk() {
	Scene::button_ok->setVisible(true);
	Scene::button_ok->setCollidable(true);
	std::list<Gift*>::iterator iter;
	iter = Scene::giftlist.begin();
	Gift* gift;
	while(iter!=Scene::giftlist.end()) {
		gift = *iter;
		gift->setCollidable(false);
		iter++;
	}
}

void timechose10() {
	Scene::timeGuess = 10;
	Button* time20 = Scene::menu->getButton("20_button");
	Button* time30 = Scene::menu->getButton("30_button");
	time20->toggle(Button::BUTTON_NORMAL);
	time30->toggle(Button::BUTTON_NORMAL);
}

void timechose20() {
	Scene::timeGuess = 20;
	Button* time10 = Scene::menu->getButton("10_button");
	Button* time30 = Scene::menu->getButton("30_button");
	time10->toggle(Button::BUTTON_NORMAL);
	time30->toggle(Button::BUTTON_NORMAL);
}

void timechose30() {
	Scene::timeGuess = 30;
	Button* time10 = Scene::menu->getButton("10_button");
	Button* time20 = Scene::menu->getButton("20_button");
	time10->toggle(Button::BUTTON_NORMAL);
	time20->toggle(Button::BUTTON_NORMAL);
}

bool Scene::isEndStage() {
	Player* player;
	bool result = true;
	std::list<Player*>::iterator iter;
	iter = playerlist.begin();
	while (iter != playerlist.end()) {
		player = *iter;
		if(player) { 
			if(player->getStatus()!= Player::LOSED) {
				result = false;
				break;
			}
		}
		++iter;
	}
	return result;
}

bool Scene::isNextStage() {
	return checkNextStage;
}

void Scene::nextStage() {
	static int phase=1;
	if(phase > (Player::getNumPlayer()+1)) {
		scenePlayerMenu_start = false;
		scenePlayerMenu_on = false;
		sceneSpecial_start = false;
		sceneSpecial_on = false;
		g_engine->Close();
	}
	if(!quiz->isFinish())
		return;
	phase++;
	g_player->winScore();
	quiz->change(0,Player::getNumPlayer());
	keyboard->reset();
	Player* player;
	std::list<Player*>::iterator iter;
	iter = playerlist.begin();
	while (iter != playerlist.end()) {
		player = *iter;
		player->reset();
		++iter;
	}
	if(phase == (Player::getNumPlayer()+1)) {
		string max_id;
		int max = -1;
		int fortune;
		iter = playerlist.begin();
		
		while (iter != playerlist.end()) {
			player = *iter; 
			if(max == player->getTotalScore()) {
				fortune = rand() % 2;
				if(!fortune) {
					max = player->getTotalScore();
					max_id = player->getName();
				}
			}
			else if (max < player->getTotalScore()) {
				max = player->getTotalScore();
				max_id = player->getName();
			}
			++iter;
		}
		int count = 1;
		iter = playerlist.begin();
		while (iter != playerlist.end()) {
			player = *iter;
			if(max_id == player->getName()){
				player->setStatus(Player::WIN_GAME);
			}
			else {
				player->loseGame();
			}
			++count;
			++iter;
		}
		play2special();
		Player::setCurrentPlayer(count);
	}
	else {
		Player::setCurrentPlayer(phase);
		scenePlayerMenu_start = true;
	}
	checkNextStage = false;
	Next_Stage->setVisible(false);
	Next_Stage->setCollidable(false);
	cleartemp = true;
}

void Scene::sceneplay() {
	if(sceneplay_on == true)
		return;
	if((sceneplay_start == true) && (sceneplay_on == false)) {
		quiz->change(0,Player::getNumPlayer());
		score_background->setVisible(true);
		keyboard->setVisible(true);
		wheel->setVisible(true);
		wheel->setCollidable(true);
		arrow->setVisible(true);
		sceneplay_on = true;
	}
}

void Scene::sceneSpecial() {
	if(sceneSpecial_on == true)
		return;
	if((sceneSpecial_start == true) && (sceneSpecial_on == false)) {
		wheel->setVisible(false);
		wheel->setCollidable(false);
		wheel_special->setVisible(true);
		wheel_special->setCollidable(true);
		button_ready->setVisible(true);
		button_ready->setCollidable(true);
		timebar->setVisible(true);
		sceneSpecial_on = true;
	}
}

void Scene::release() {
	Letters::release();
	delete menu;
	delete background;
	delete score_background;
	delete background_image;
	delete arrow;
	delete wheel;
	delete wheel_special;
	delete keyboard;
	delete quiz;
	delete PlayerMenu_Spin;
	delete PlayeMenu_Guess;
	delete Next_Stage;
	delete button_ok;
	delete button_ready;
	delete startGame;
	delete timebar;
	delete cursor;
}

#endif