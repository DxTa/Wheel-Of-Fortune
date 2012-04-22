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
#include "timebar.h"
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
	Timebar* timebar;
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
	Button* pause_button;
	Button* back_button;
	Button* play_button;
	Button* help_button;
	Button* exit_button;

	int openspecial = 0;
	int numGift;
	int numPlayer;
	int timeGuess;
	int phase=1;
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
	bool scenePause_start = false;
	bool scenePause_on = false;
	bool cleartemp= true;

	void sceneplay();
	void sceneSpecial();
	void sceneMain();
	void sceneNewGame();
	void sceneHelp();
	void scenePause();

	void release();

	bool isEndStage();
	bool isNextStage();
	void nextStage();

	enum {CURSOR=150,EMOTION_GIFT = 11000,NEXT_STAGE,GUESSAWORD,LOSEALL,OVERTIME,NOTIFY_GIFT,NEWGAME_TITTLE,EMO_SPECIAL,PAUSE_LAYER};
}

void g_exit() {
	g_engine->Close();
}

void main2newgame() {
	Scene::sceneNewGame_start = true;
	Scene::sceneMain_start = false;
	Scene::sceneMain_on = false;
}

void main2help() {
	Scene::sceneHelp_start = true;
	Scene::sceneMain_start = false;
	Scene::sceneMain_on = false;
}

void newgame2play() {
	Scene::sceneplay_start = true;
	Scene::sceneMain_start = false;
	Scene::sceneMain_on = false;
	Scene::sceneNewGame_start = false;
	Scene::sceneNewGame_on = false;
	Player::setCurrentPlayer(1);
}

void play2special() {
	Scene::sceneplay_start = false;
	Scene::sceneplay_on = false;
	Scene::scenePlayerMenu_start  = false;
	Scene::scenePlayerMenu_on = false;
	Scene::sceneSpecial_start = true;
}

void special2main() {
	Scene::keyboard->setVisible(false);
	Scene::wheel->setVisible(false);
	Scene::wheel->setCollidable(false);
	Scene::wheel_special->setVisible(false);
	Scene::wheel_special->setCollidable(false);
	Scene::arrow->setVisible(false);
	Scene::quiz->reset();
	Scene::score_background->setVisible(false);
	Scene::Next_Stage->setVisible(false);
	Scene::Next_Stage->setCollidable(false);
	Scene::PlayeMenu_Guess->setCollidable(false);
	Scene::PlayeMenu_Guess->setVisible(false);
	Scene::PlayerMenu_Spin->setCollidable(false);
	Scene::PlayerMenu_Spin->setVisible(false);
	Scene::quiz->setClearOff(true);
	Scene::phase = 1;
	
	Scene::sceneMain_start = true;

	Scene::sceneplay_start = false;
	Scene::sceneplay_on = false;
	Scene::sceneSpecial_start = false;
	Scene::sceneSpecial_on = false;
	Scene::scenePlayerMenu_start = false;
	Scene::scenePlayerMenu_on = false;
}

void stop2pause() {
	if(((Scene::sceneplay_start == false) && (Scene::sceneSpecial_start == false)) || ((Scene::sceneSpecial_start == true) && (Scene::Next_Stage->getVisible()==true))) {
		Scene::pause_button->reset();
		return;
	}
	if((Scene::g_player->getStatus() == Player::SPINNING) || (Scene::g_player->getStatus() == Player::BEGIN_SPECIAL)) {
		Scene::pause_button->reset();
		return;
	}
	Scene::scenePause_start = true;
	Scene::PlayeMenu_Guess->setCollidable(false);
	Scene::PlayerMenu_Spin->setCollidable(false);
	Scene::button_ready->setCollidable(false);
}

void pause2resume() {
	if(Scene::scenePause_start == true) {
		Scene::scenePause_start = false;
		Scene::scenePause_on = false;
		Scene::PlayeMenu_Guess->setCollidable(true);
		Scene::PlayerMenu_Spin->setCollidable(true);
		Scene::menu->close();
		Scene::pause_button->reset();
		if(Scene::sceneSpecial_start == true) {
			Scene::wheel_special->setCollidable(true);
			if(Scene::button_ready->getStatus() == Button::BUTTON_NORMAL)
				Scene::button_ready->setCollidable(true);
		}
		if(Scene::Next_Stage->getVisible()==true)
			Scene::Next_Stage->setCollidable(true);
	}
}

void pause2main() {
	Scene::scenePause_start = false;
	Scene::scenePause_on =false;
	Scene::pause_button->setVisible(false);
	Scene::pause_button->setCollidable(false);
	special2main();
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

void Scene::nextStage()  {
	if((!isEndStage() && !quiz->isFinish() && g_player->getStatus() != Player::WIN_SPECIAL && g_player->getStatus() != Player::LOSED_SPECIAL) || (sceneMain_start == true))
		return;
	phase++;
	if(phase > (Player::getNumPlayer()+1)) {
		special2main();
		phase = 1;
	}
	else {
		if(!quiz->isFinish())
			return;
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
}

void Scene::sceneMain() {
	if(sceneMain_on ==true)
		return;
	if((sceneMain_start==true) &&  (sceneMain_on == false)) {
		menu->close();
		background_image->Release();
		background_image->Load("source/mainmenu/mainmenu.png");
		background->setImage(background_image);
		menu = new Menu("mainmenu","play_button","help_button","exit_button",NULL);
		menu->addCallback("play_button",main2newgame);
		menu->addCallback("help_button",main2help);
		menu->addCallback("exit_button",g_exit);
		menu->setPosition(550,350);
		sceneMain_on = true;
	}
}

void Scene::sceneplay() {
	if(sceneplay_on == true)
		return;
	if((sceneplay_start == true) && (sceneplay_on == false)) {
		menu->close();
		pause_button->setVisible(true);
		pause_button->setCollidable(true);
		quiz->change(0,Player::getNumPlayer());
		specialGift = "";
		score_background->setVisible(true);
		keyboard->setVisible(true);
		keyboard->reset();
		wheel->setVisible(true);
		wheel->setCollidable(true);
		arrow->setVisible(true);
		sceneplay_on = true;
		scenePlayerMenu_start = true;
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
		button_ready->reset();
		button_ready->setVisible(true);
		button_ready->setCollidable(true);
		timebar->setVisible(true);
		sceneSpecial_on = true;
	}
}

void Scene::sceneHelp() {
	if(sceneHelp_on == true)
		return;
	if((sceneHelp_start == true) && (sceneHelp_on == false)) {
		menu->close();
		back_button->reset();
		back_button->setVisible(true);
		background_image->Release();
		background_image->Load("source/helpscene.png");
		background->setImage(background_image);
		sceneHelp_on = true;
	}
}

void Scene::scenePause() {
	if(scenePause_on ==true)
		return;
	if((scenePause_start == true) && (scenePause_on == false)) {
		Sprite* layer = new Sprite();
		layer->loadImage("source/pauselayer.png");
		layer->setObjectType(Scene::PAUSE_LAYER);
		g_engine->addEntity(layer);
		wheel->setCollidable(false);
		wheel_special->setCollidable(false);
		keyboard->setCollidable(false);
		pause_button->reset();
		pause_button->setVisible(true);
		pause_button->setCollidable(true);

		menu->close();
		menu = new Menu("pausescene","resume","quit",NULL);
		menu->addCallback("resume",pause2resume);
		menu->addCallback("quit",pause2main);
		menu->setPosition(g_engine->getScreenWidth()/2-75,75);
		scenePause_on = true;
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
	delete back_button;
	delete pause_button;
	delete play_button;
	delete help_button;
	delete exit_button;
	delete timebar;
	delete cursor;
}

#endif