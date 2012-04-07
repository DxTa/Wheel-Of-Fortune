#pragma once

#include "Menu.h"
#include "wheel.h"
#include "Keyboard.h"
#include "Quiz.h"
#include "Engine/Advanced2D.h"

ostringstream heee;

namespace Scene {
	std::list<Player*> playerlist;
	string chose;
	string ss;
	Timer timecheck;
	Player* g_player;
	Menu* menu;
	Sprite* background;
	Texture* background_image;
	Sprite* arrow;
	Wheel* wheel;
	Keyboard* keyboard;
	Quiz* quiz;
	Button* PlayerMenu_Spin;
	Button* PlayeMenu_Guess;
	Button* Next_Stage;
	bool sceneplay_start = false;
	bool scenePlayerMenu_start = false;
	bool sceneplay_on = false;
	bool scenePlayerMenu_on = false;
	bool checkNextStage = false;
	bool cleartemp= true;
	void sceneplay();
	void scenePlayerMenu();

	void init();
	void update();
	void updateMouseButton();
	void updateMouseMove(double delta_x,double delta_y,double fx,double fy);
	void release();

	void newPlayer();
	void updatePlayer();
	void deletePlayer();
	void resetPlayer();
	void nextStage();

	bool isEndStage();
	bool isNextStage();
}

void g_exit() {
	g_engine->Close();
}

void spin() {
	Scene::wheel->setStatus(Wheel::WAIT);
	Scene::scenePlayerMenu_start  = false;
	Scene::scenePlayerMenu_on = false;
	Scene::PlayerMenu_Spin->setVisible(false);
	Scene::PlayerMenu_Spin->setCollidable(false);
	Scene::PlayeMenu_Guess->setVisible(false);
	Scene::PlayeMenu_Guess->setCollidable(false);
}

void guess() {
	Scene::keyboard->saveState();
	Scene::g_player->setStatus(Player::READY_TO_FULL_ANSWER);
	Scene::scenePlayerMenu_start  = false;
	Scene::scenePlayerMenu_on = false;
	Scene::PlayerMenu_Spin->setVisible(false);
	Scene::PlayerMenu_Spin->setCollidable(false);
	Scene::PlayeMenu_Guess->setVisible(false);
	Scene::PlayeMenu_Guess->setCollidable(false);	
}

void Scene::newPlayer() {
	Player* player = new Player();
	player->setID(Player::getNumPlayer());
	playerlist.push_back(player);
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
				if((g_player->getStatus()!= Player::LOSED) && (g_player->getStatus()!= Player::READY_TO_ANSWER) && (g_player->getStatus()!= Player::SPINNING)&& (g_player->getStatus()!= Player::READY_TO_FULL_ANSWER) && (g_player->getStatus() == Player::WIN_STAGE))
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
	if(phase > (Player::getNumPlayer()+1))
		g_engine->Close();
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
				break;
			}
			++count;
			++iter;
		}
		Player::setCurrentPlayer(count);
	}
	else 
		Player::setCurrentPlayer(phase);
	scenePlayerMenu_start = true;
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

	wheel = new Wheel();
	wheel->loadImage("wheel.png");
	wheel->setPosition(0,300);
	wheel->setOR(wheel->getX() + wheel->getWidth()/2,wheel->getY() + wheel->getHeight()/2,(wheel->getHeight()/2));
	wheel->setObjectType(Wheel::WHEEL_POS);
	wheel->setVisible(false);
	wheel->setCollidable(false);
	g_engine->addEntity(wheel);

	arrow = new Sprite();
	arrow->loadImage("arrow.png");
	arrow->setRotation(g_engine->math->toRadians(90));
	arrow->setPosition(220,230);
	arrow->setVisible(false);
	arrow->setCollidable(false);
	g_engine->addEntity(arrow);

	keyboard = new Keyboard();
	keyboard->setPosition(380,0);
	keyboard->addEntity();

	PlayerMenu_Spin = new Button("spin_button");
	PlayerMenu_Spin->setCallback(spin);
	PlayerMenu_Spin->setCollidable(false);
	PlayerMenu_Spin->setVisible(false);
	PlayerMenu_Spin->setPosition(400,150);
	g_engine->addEntity(PlayerMenu_Spin);

	PlayeMenu_Guess = new Button("guess_button");
	PlayeMenu_Guess->setCallback(guess);
	PlayeMenu_Guess->setCollidable(false);
	PlayeMenu_Guess->setVisible(false);
	PlayeMenu_Guess->setPosition(520,150);
	g_engine->addEntity(PlayeMenu_Guess);

	Next_Stage = new Button("NextStage_button");
	Next_Stage->setCallback(nextStage);
	Next_Stage->setCollidable(false);
	Next_Stage->setVisible(false);
	Next_Stage->setPosition(g_engine->getScreenWidth()-Next_Stage->getWidth(),g_engine->getScreenHeight()-Next_Stage->getHeight());
	g_engine->addEntity(Next_Stage);

	Scene::newPlayer();
	Scene::newPlayer();
	Scene::newPlayer();
	Player::setCurrentPlayer(1);

	quiz = new Quiz();
	quiz->setPosition(0,0);
	quiz->setWidth(380);
	quiz->setHeight(200);
	quiz->inputLog();
	quiz->change(0,Player::getNumPlayer());
	Scene::scenePlayerMenu_start = true;
	Scene::checkNextStage = false;
}

void Scene::sceneplay() {
	if(sceneplay_on == true)
		return;
	if((sceneplay_start == true) && (sceneplay_on == false)) {
		menu->close();
		background_image->Release();
		background_image->Load("scene1.png");
		wheel->setVisible(true);
		wheel->setCollidable(true);
		arrow->setVisible(true);
		sceneplay_on = true;
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

void Scene::update() {
	Scene::background->setImage(Scene::background_image);
	Scene::updatePlayer();
	sceneplay();
	scenePlayerMenu();
	Scene::menu->update();
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
}

void Scene::updateMouseButton() {
	Scene::wheel->updateMouseButton();
	Scene::menu->updateMouseButton();
	if(PlayerMenu_Spin->isPosition() == true)
		PlayerMenu_Spin->pressed();
	if(PlayeMenu_Guess->isPosition() == true)
		PlayeMenu_Guess->pressed();
	if(Next_Stage->isPosition() ==true)
		Next_Stage->pressed();
	if(timecheck.stopwatch(96)) {
		ss = g_player->answer(keyboard,quiz);
		chose += ss;
		if((ss!= "") && (g_player->getStatus() != Player::READY_TO_FULL_ANSWER)) {
			Scene::scenePlayerMenu_start = true;
		}
	}
}

void Scene::updateMouseMove(double delta_x,double delta_y,double fx,double fy) {
	Scene::wheel->updateMouseMove(delta_x,delta_y,fx,fy);
	Scene::menu->updateMouseMove();
	Scene::keyboard->updateMouseMove();
	PlayerMenu_Spin->setCheckPosition(false);
	PlayeMenu_Guess->setCheckPosition(false);
	Next_Stage->setCheckPosition(false);
	if(g_player->getStatus() == Player::READY_TO_FULL_ANSWER)
		keyboard->reset();
}

void Scene::release() {
	deletePlayer();
	Letters::release();
	delete menu;
	delete background;
	delete background_image;
	delete arrow;
	delete wheel;
	delete keyboard;
	delete quiz;
	delete PlayerMenu_Spin;
	delete PlayeMenu_Guess;
	delete Next_Stage;
}