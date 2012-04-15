#pragma once

#include "Menu.h"
#include "wheel.h"
#include "WheelSpecial.h"
#include "Keyboard.h"
#include "Quiz.h"
#include "Gift.h"
#include "Cursor.h"
#include "Engine/Advanced2D.h"

namespace Scene {
	std::list<Player*> playerlist;
	std::list<Gift*> giftlist;
	string chose;
	string ss;
	string specialGift;
	Timer timecheck;
	Timer buttoncheck;
	Font* system12;
	Player* g_player;
	Menu* menu;
	Cursor* cursor;
	Sprite* background;
	Sprite* score_background;
	Texture* background_image;
	Sprite* arrow;
	Wheel* wheel;
	WheelSpecial* wheel_special;
	Keyboard* keyboard;
	Quiz* quiz;
	Button* PlayerMenu_Spin;
	Button* PlayeMenu_Guess;
	Button* Next_Stage;
	Button* button_ok;
	Button* button_ready;
	bool sceneplay_start = false;
	bool sceneplay_on = false;
	bool scenePlayerMenu_start = false;
	bool scenePlayerMenu_on = false;
	bool scenePlayerGift_start = false;
	bool scenePlayerGift_on = false;
	bool sceneSpecial_start = false;
	bool sceneSpecial_on = false;
	bool checkNextStage = false;
	bool cleartemp= true;
	void sceneplay();
	void scenePlayerMenu();
	void scenePlayerGift();
	void sceneSpecial();

	void init();
	void update();
	void updateMouseButton();
	void updateMouseMove(double delta_x,double delta_y,double fx,double fy);
	void release();

	void newPlayer();
	void newPlayer(string name,double x,double y);
	void updatePlayer();
	void deletePlayer();
	void resetPlayer();
	void nextStage();
	void spinPlayer();
	void drawPlayer();

	void newGift(string );
	void setGiftPosition(int x,int y);
	void showGift();
	void hideGift();
	void updateGiftMouseMove();
	void updateGiftMouseButton();
	void deleteGift();

	bool isEndStage();
	bool isNextStage();
	Sprite* timebar;


	enum {CURSOR=150};
}

void g_exit() {
	g_engine->Close();
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
	Scene::keyboard->setStatus(Keyboard::WAIT);
}

void showButtonOk() {
	Scene::button_ok->setVisible(true);
	Scene::button_ok->setCollidable(true);
}

void Scene::newPlayer() {
	Player* player = new Player();
	player->setID(Player::getNumPlayer());
	playerlist.push_back(player);
}

void Scene::newPlayer(string name,double x,double y) {
	Player* player = new Player(name);
	player->setID(Player::getNumPlayer());
	player->setScale(0.25);
	player->setRotation(g_engine->math->toRadians(-20));
	player->setPosition(x,y);
	playerlist.push_back(player);
}

void Scene::drawPlayer() {
	std::list<Player*>::iterator iter;
	iter = playerlist.begin();
	Player*temp;
	while(iter!= playerlist.end()) {
		temp = *iter;
		temp->draw(system12);
		iter++;
	}
	if(!isEndStage())
		g_player->showPlay(system12);
}

void Scene::newGift(string name) {
	Gift* gift = new Gift(name);
	gift->setVisible(false);
	gift->setCollidable(false);
	gift->setCallback(showButtonOk);
	g_engine->addEntity(gift);
	giftlist.push_back(gift);
}

void Scene::setGiftPosition(int x,int y) {
	list<Gift*>::iterator iter;
	Gift* gift;
	iter = giftlist.begin();
	int i = 0;
	while(iter!=giftlist.end()) {
		gift = *iter;
		gift->setPosition(x + (i%3)*gift->getWidth(),y + (int)(i/3)*gift->getHeight());
		++i;
		++iter;
	}
}

void Scene::showGift() {
	list<Gift*>::iterator iter;
	iter = giftlist.begin();
	int i = 0;
	Gift* gift;
	while(iter!=giftlist.end()) {
		gift = *iter;
		gift->setVisible(true);
		gift->setCollidable(true);
		++iter;
	}
	if(button_ok->getStatus()==Button::BUTTON_PRESSED) {
		button_ok->reset();
		button_ok->setVisible(false);
		button_ok->setCollidable(false);
	}
}

void Scene::hideGift() {
	list<Gift*>::iterator iter;
	iter = giftlist.begin();
	int i = 0;
	Gift* gift;
	while(iter!=giftlist.end()) {
		gift = *iter;
		gift->setVisible(false);
		gift->setCollidable(false);
		++iter;
	}
	scenePlayerGift_start = false;
	scenePlayerGift_on = false;
	scenePlayerMenu_start = true;
}

void Scene::deleteGift() {
	list<Gift*>::iterator iter;
	iter = giftlist.begin();
	int i = 0;
	Gift* gift;
	while(iter!=giftlist.end()) {
		gift = *iter;
		gift->setAlive(false);
		iter = giftlist.erase(iter);
	}
}

void play2special() {
	Scene::sceneplay_start = false;
	Scene::sceneplay_on = false;
	Scene::scenePlayerMenu_start  = false;
	Scene::scenePlayerMenu_on = false;
	Scene::sceneSpecial_start = true;
}

void Scene::updatePlayer() {
	if(isEndStage() == true) {
		quiz->openAll();
		keyboard->setStatus(Keyboard::UNAVAILABLE);
		Next_Stage->setVisible(true);
		Next_Stage->setCollidable(true);
		if(isNextStage() == true) {
			nextStage();
		}
		return;
	}
	std::list<Player*>::iterator iter;
	iter = playerlist.begin();
	while (iter != playerlist.end()) {
		g_player = *iter;
		if(g_player) {
			if(g_player->getID() == Player::getCurrentPlayer()) {
				int count = 0;
				while(g_player->getStatus() == Player::LOSED) {
					Player::setCurrentPlayer(Player::getCurrentPlayer()+1);
					++iter;
					g_player = *iter;
					count++;
					if(count > Player::getNumPlayer())
						return;
				}
				if((g_player->getStatus()!= Player::LOSED) && (g_player->getStatus()!= Player::READY_TO_ANSWER) && (g_player->getStatus()!= Player::SPINNING)&& (g_player->getStatus()!= Player::READY_TO_FULL_ANSWER) && (g_player->getStatus() == Player::WIN_STAGE) && (g_player->getStatus() == Player::BEGIN_SPECIAL) && (g_player->getStatus() == Player::FULL_SPECIAl))
					g_player->setStatus(Player::PLAYING);
				break;
			}
		}
		++iter;
	}
}

void Scene::deletePlayer() {
	std::list<Player*>::iterator iter;
	iter = playerlist.begin();
	while (iter != playerlist.end()) {
		g_player = *iter;
		delete g_player;
		iter = playerlist.erase(iter);
	}
}

void Scene::spinPlayer() {
	Scene::wheel->setHolding(false);
	Scene::wheel_special->setHolding(false);
	if(sceneplay_start == true) {
		if(wheel->getStatus() == Wheel::STOP)
			return;
		timebar->setVisible(true);
		switch(g_player->spin(wheel)) {
		case Wheel::G_BANKRUPT :
			g_player->setScore(0);
			g_player->end_play(Wheel::G_BANKRUPT);
			Scene::scenePlayerMenu_start = true;
			break;
		case Wheel::G_LOSEATURN :
			g_player->end_play(Wheel::G_LOSEATURN);
			Scene::scenePlayerMenu_start = true;
			break;
		case Wheel::G_GIFT :
			Scene::scenePlayerGift_start = true;
			break;
		}
		return;
	}
	if(sceneSpecial_start == true) {
		if(wheel_special->getStatus() == Wheel::STOP)
			return;
		switch(g_player->spin(wheel_special)) {
		case WheelSpecial::G_1mil :
			specialGift = "$1.000.000 ";
			Scene::g_player->setStatus(Player::BEGIN_SPECIAL);
			break;
		case WheelSpecial::G_PHONE :
			specialGift = "a iPhone";
			Scene::g_player->setStatus(Player::BEGIN_SPECIAL);
			break;
		case WheelSpecial::G_TABLET :
			specialGift = "an iPad";
			Scene::g_player->setStatus(Player::BEGIN_SPECIAL);
			break;
		case WheelSpecial::G_TRIP :
			specialGift = "a Europe Trip";
			Scene::g_player->setStatus(Player::BEGIN_SPECIAL);
			break;
		case WheelSpecial::G_TV :
			specialGift = "a Tivi";
			Scene::g_player->setStatus(Player::BEGIN_SPECIAL);
			break;
		}
		return;
	}
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
		int max = 0;
		iter = playerlist.begin();
		while (iter != playerlist.end()) {
			player = *iter;
			player->setStatus(Player::LOSED);
			if(max < player->getTotalScore()) {
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
			else 
				player->setStatus(Player::LOSED);
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

void Scene::init() {
	menu = new Menu();
	background = new Sprite();
	background_image = new Texture();
	Scene::background->setCollidable(false);
	g_engine->addEntity(Scene::background);

	cursor = new Cursor();
	cursor->loadImage("Cursor_564.png");
	cursor->setScale(0.5);
	cursor->setCollisionMethod(COLLISION_RECT);
	cursor->setObjectType(CURSOR);
	g_engine->addEntity(cursor);
	
	system12 = new Font();
	if (!system12->loadImage("font.tga")) {
		g_engine->message("Error loading system12.tga");
		return;
	}
	system12->setColumns(16);
	system12->setCharSize(21,32);

	if (!system12->loadWidthData("font.dat")) {
		g_engine->message("Error loading system12.dat");
		return;
	}
	wheel = new Wheel();
	wheel->loadImage("wheel.png");
	wheel->setPosition(0,500);
	wheel->setOR(wheel->getX() + wheel->getWidth()/2,wheel->getY() + wheel->getHeight()/2,(wheel->getHeight()/2));
	wheel->setObjectType(Wheel::WHEEL_POS);
	wheel->setVisible(false);
	wheel->setCollidable(false);
	g_engine->addEntity(wheel);

	wheel_special = new WheelSpecial();
	wheel_special->loadImage("wheel_special.png");
	wheel_special->setPosition(0,500);
	wheel_special->setOR(wheel_special->getX() + wheel_special->getWidth()/2,wheel_special->getY() + wheel_special->getHeight()/2,(wheel_special->getHeight()/2));
	wheel_special->setObjectType(Wheel::WHEEL_POS_SPECIAL);
	wheel_special->setVisible(false);
	wheel_special->setCollidable(false);
	g_engine->addEntity(wheel_special);

	arrow = new Sprite();
	arrow->loadImage("arrow.png");
	arrow->setRotation(g_engine->math->toRadians(90));
	arrow->setPosition(222.0f,430);
	arrow->setVisible(false);
	arrow->setCollidable(false);
	g_engine->addEntity(arrow);

	keyboard = new Keyboard();
	keyboard->setPosition(550,550);
	keyboard->addEntity();

	PlayerMenu_Spin = new Button("spin_button");
	PlayerMenu_Spin->setCallback(spin);
	PlayerMenu_Spin->setCollidable(false);
	PlayerMenu_Spin->setVisible(false);
	PlayerMenu_Spin->setPosition(400,300);
	g_engine->addEntity(PlayerMenu_Spin);

	PlayeMenu_Guess = new Button("guess_button");
	PlayeMenu_Guess->setCallback(guess);
	PlayeMenu_Guess->setCollidable(false);
	PlayeMenu_Guess->setVisible(false);
	PlayeMenu_Guess->setPosition(600,300);
	g_engine->addEntity(PlayeMenu_Guess);

	Next_Stage = new Button("NextStage_button");
	Next_Stage->setCallback(nextStage);
	Next_Stage->setCollidable(false);
	Next_Stage->setVisible(false);
	Next_Stage->setPosition(g_engine->getScreenWidth()-Next_Stage->getWidth(),g_engine->getScreenHeight()/2-Next_Stage->getHeight()/2+100);
	g_engine->addEntity(Next_Stage);

	button_ok = new Button("ok_button");
	button_ok->setCallback(hideGift);
	button_ok->setCollidable(false);
	button_ok->setVisible(false);
	button_ok->setPosition(600-75,400);
	g_engine->addEntity(button_ok);

	button_ready = new Button("ready_button");
	button_ready->setCallback(readyspecial);
	button_ready->setCollidable(false);
	button_ready->setVisible(false);
	button_ready->setPosition(g_engine->getScreenWidth()/2-button_ready->getWidth()/2,g_engine->getScreenHeight()/2-button_ready->getHeight()/2);
	g_engine->addEntity(button_ready);

	Scene::newPlayer("player1",750,80);
	Scene::newPlayer("player2",860,80);
	Scene::newPlayer("player3",980,80);
	/*
	Scene::newPlayer("player1",750,100);
	Scene::newPlayer("player2",830,95);
	Scene::newPlayer("player3",920,90);
	Scene::newPlayer("player4",1000,90);
	*/
	Player::setCurrentPlayer(1);
	Scene::newGift("car");
	Scene::newGift("car");
	Scene::newGift("car");
	Scene::newGift("car");
	Scene::newGift("car");
	Scene::newGift("car");
	Scene::setGiftPosition(button_ok->getPosition().getX()-100,button_ok->getPosition().getY()-200);

	quiz = new Quiz();
	quiz->setPosition(50,125);
	quiz->setWidth(400);
	quiz->setHeight(200);
	quiz->inputLog();
	quiz->change(0,Player::getNumPlayer());
	Scene::scenePlayerMenu_start = true;
	Scene::checkNextStage = false;
	background_image->Load("background.png");
	Scene::background->setImage(Scene::background_image);

	Scene::score_background = new Sprite();
	score_background->loadImage("score_background.png");
	score_background->setPosition(590,-40);

	Scene::timebar = new Sprite();
	timebar->loadImage("source/button/time bar.png");
	timebar->setTotalFrames(60);
	timebar->setSize(489,10);
	timebar->setColumns(1);
	timebar->setCurrentFrame(59);
	timebar->setPosition(g_engine->getScreenWidth()/2-timebar->getWidth()/2,g_engine->getScreenHeight()/2+100);
	timebar->setVisible(false);
}

void Scene::sceneplay() {
	if(sceneplay_on == true)
		return;
	if((sceneplay_start == true) && (sceneplay_on == false)) {
		menu->close();
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

void Scene::scenePlayerMenu() {
	if((scenePlayerMenu_on == true) || (Scene::isEndStage() == true) ||(Scene::quiz->isFinish() == true))
		return;
	if((scenePlayerMenu_start == true) && (scenePlayerMenu_on == false)) {
		scenePlayerMenu_on = true;
		PlayerMenu_Spin->setVisible(true);
		PlayerMenu_Spin->setCollidable(true);
		PlayeMenu_Guess->setVisible(true);
		PlayeMenu_Guess->setCollidable(true);
	}
}

void Scene::scenePlayerGift() {
	if((scenePlayerGift_on == true) || (Scene::isEndStage() == true) ||(Scene::quiz->isFinish() == true))
		return;
	if((scenePlayerGift_start == true) && (scenePlayerGift_on == false)) {
		scenePlayerGift_on = true;
		showGift();
	}
}

void Scene::update() {
	Scene::updatePlayer();
	sceneplay();
	sceneSpecial();
	scenePlayerMenu();
	scenePlayerGift();
	//Scene::menu->update();
	PlayerMenu_Spin->reset();
	PlayeMenu_Guess->reset();
	Next_Stage->reset();
	if((quiz->isFinish() == true) && (isEndStage() == false)) {
		Next_Stage->setVisible(true);
		Next_Stage->setCollidable(true);
		keyboard->setStatus(Keyboard::UNAVAILABLE);
		if(isNextStage() == true)
			nextStage();
	}
	if((timebar->getVisible() == true) &&(g_player->getStatus() == Player::FULL_SPECIAl)) { 
		if(timecheck.stopwatch(1000)) {
			timebar->setCurrentFrame(timebar->getCurrentFrame()-1);
		}
		if(timebar->getCurrentFrame()==0)
			g_player->setStatus(Player::LOSED);
	}
	if((timebar->getVisible() == true) &&(g_player->getStatus() == Player::READY_TO_ANSWER)) { 
		if(timecheck.stopwatch(100)) {
			timebar->setCurrentFrame(timebar->getCurrentFrame()-1);
		}
		if(timebar->getCurrentFrame()==0) {
			timebar->setCurrentFrame(59);
			g_player->end_play();
			scenePlayerMenu_start = true;
		}
	}
	if((timebar->getVisible() == true) &&(g_player->getStatus() == Player::READY_TO_FULL_ANSWER)) { 
		if(timecheck.stopwatch(300)) {
			timebar->setCurrentFrame(timebar->getCurrentFrame()-1);
		}
		if(timebar->getCurrentFrame()==0) {
			timebar->setCurrentFrame(59);
			g_player->setStatus(Player::LOSED);
			g_player->answer(keyboard,quiz);
			g_player->end_play(Player::LOSED);
			scenePlayerMenu_start = true;
		}
	}
}

void Scene::updateGiftMouseButton() {
	list<Gift*>::iterator iter;
	iter = giftlist.begin();
	Gift* gift;
	while(iter!=giftlist.end()) {
		gift = *iter;
		if(gift->isPosition()==true) {
			if(gift->getStatus() == Button::BUTTON_NORMAL)
				g_player->setGift(gift->getLabel());	
			gift->pressed();
		}	
		++iter;
	}
}
void Scene::updateMouseButton() {
	Scene::wheel->updateMouseButton();
	Scene::wheel_special->updateMouseButton();
	Scene::menu->updateMouseButton();
	Scene::updateGiftMouseButton();
	if(PlayerMenu_Spin->isPosition() == true)
		PlayerMenu_Spin->pressed();
	if(PlayeMenu_Guess->isPosition() == true)
		PlayeMenu_Guess->pressed();
	if(Next_Stage->isPosition() == true)
		Next_Stage->pressed();
	if(button_ok->isPosition() == true)
		button_ok->pressed();
	if(button_ready->isPosition() == true)
		button_ready->pressed();
	if(buttoncheck.stopwatch(96)) {
		ss = g_player->answer(keyboard,quiz);
		chose += ss;
		if((ss!= "") && (g_player->getStatus() != Player::READY_TO_FULL_ANSWER) && (g_player->getStatus() != Player::FULL_SPECIAl) && (g_player->getStatus() != Player::BEGIN_SPECIAL)) {
			Scene::scenePlayerMenu_start = true;
			Scene::timebar->setCurrentFrame(59);
		}
	}
	if(g_player->getStatus() == Player::WIN_GAME)
		Scene::scenePlayerMenu_start = false;
}

void Scene::updateGiftMouseMove() {
	list<Gift*>::iterator iter;
	iter = giftlist.begin();
	Gift* gift;
	while(iter!=giftlist.end()) {
		gift = *iter;
		gift->setCheckPosition(false);
		++iter;
	}
}

void Scene::updateMouseMove(double delta_x,double delta_y,double fx,double fy) {
	Scene::wheel->updateMouseMove(delta_x,delta_y,fx,fy);
	Scene::wheel_special->updateMouseMove(delta_x,delta_y,fx,fy);
	Scene::menu->updateMouseMove();
	Scene::keyboard->updateMouseMove(cursor);
	Scene::updateGiftMouseMove();
	PlayerMenu_Spin->updateMouseButton(cursor);
	PlayeMenu_Guess->updateMouseButton(cursor);
	button_ok->updateMouseButton(cursor);
	button_ready->updateMouseButton(cursor);
	Next_Stage->updateMouseButton(cursor);
	if((g_player->getStatus() == Player::READY_TO_FULL_ANSWER) || (g_player->getStatus() == Player::FULL_SPECIAl))
		keyboard->reset();
}

void Scene::release() {
	deletePlayer();
	deleteGift();
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
	delete timebar;
	delete cursor;
}